//@HEADER140
//
// ************************************************************************
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Part of Kokkos, under the Apache License v2.0 with LLVM Exceptions.
// See https://kokkos.org/LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//@HEADER

// Modified by Zach Frye at LLNL
// Contact: frye7@llnl.gov
// Organization: CASC at LLNL

#include <stdio.h>
#include <inttypes.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <string>
#include <regex>
#include <cxxabi.h>
#include <dlfcn.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
int power = -1;
int power1 = 0;
int power2 = 0;
long long time1 = 0;
long long time2=0;
std::vector<float> gpu_powers;
std::vector<float> gpu_powers2;
std::vector<float> time_vec1;
std::vector<float> time_vec2;
// variorum trial run
extern "C" {
#include <variorum.h>
#include <jansson.h>
}

#include "kp_core.hpp"

#if USE_MPI
#include <mpi.h>
#endif

namespace KokkosTools {
namespace VariorumConnector {

bool filterKernels;
uint64_t nextKernelID;
std::vector<std::regex> kernelNames;
std::unordered_set<uint64_t> activeKernels;

typedef void (*initFunction)(const int, const uint64_t, const uint32_t, void*);
typedef void (*finalizeFunction)();
typedef void (*beginFunction)(const char*, const uint32_t, uint64_t*);
typedef void (*endFunction)(uint64_t);

static initFunction initProfileLibrary         = NULL;
static finalizeFunction finalizeProfileLibrary = NULL;
static beginFunction beginForCallee            = NULL;
static beginFunction beginScanCallee           = NULL;
static beginFunction beginReduceCallee         = NULL;
static endFunction endForCallee                = NULL;
static endFunction endScanCallee               = NULL;
static endFunction endReduceCallee             = NULL;

// the output path for ranekd output files
std::string printpath = "./";

// variables for simple timer
time_t start_time;

int type_of_profiling =
    0;  // 0 is for both print power & json, 1 is for print power, 2 is for json
bool usingMPI     = false;
bool verbosePrint = true;
bool mpiOutPut    = false;

// Function: variorum_print_power_call
// Description: Prints out power data in two ways: Verbose and non verbose.
//              verbose will print out each component of the systems power draw
//              in the sierra architecture. non-verbose will print the node
//              power.
// Pre: None
// Post: Will print an error message if variorum print power fails. No return
// value.
std::string variorum_print_power_call() {
  
  std::string outputString;
  char *s = NULL;
  double power_node, power_sock0, power_mem0, power_gpu0;
  double power_sock1, power_mem1, power_gpu1;
  int ret;
  ret = variorum_get_power_json(&s);
  if (ret != 0) {
    return "Print power failed!\n";
  }
  json_t *power_obj = json_loads(s, JSON_DECODE_ANY, NULL);
  // total node measurment
  power_node = json_real_value(json_object_get(power_obj, "power_node"));
  const char* hostnameChar =
      json_string_value(json_object_get(power_obj, "hostname"));
 // std::string hostname(hostnameChar);
  // print informatin to screen
  if (verbosePrint) {
    // socket 1 measurements
    power_sock0 =
        json_real_value(json_object_get(power_obj, "power_cpu_socket_0"));
    power_mem0 =
        json_real_value(json_object_get(power_obj, "power_mem_socket_0"));
    power_gpu0 =
        json_real_value(json_object_get(power_obj, "power_gpu_socket_0"));
    // socket 2 measurements
    power_sock1 =
        json_real_value(json_object_get(power_obj, "power_cpu_socket_1"));
    power_mem1 =
        json_real_value(json_object_get(power_obj, "power_mem_socket_1"));
    power_gpu1 =
        json_real_value(json_object_get(power_obj, "power_gpu_socket_1"));

    //outputString += "HostName " + hostname + "\n";
    outputString += "Total Node Power: " + std::to_string(power_node);
    outputString += "\n Socket 1 Power";
    outputString += "\n CPU Socket 1: " + std::to_string(power_sock0);
    outputString += "\n Mem Socket 1: " + std::to_string(power_mem0);
    outputString += "\n GPU Socket 1: " + std::to_string(power_gpu0);
    outputString += "\n Socket 2 Power";
    outputString += "\n CPU Socket 2: " + std::to_string(power_sock1);
    outputString += "\n Mem Socket 2: " + std::to_string(power_mem1);
    outputString += "\n GPU Socket 2: " + std::to_string(power_gpu1) + "\n";
  } else {
    //outputString += hostname + ": " + std::to_string(power_node) + "\n";
  }

  return outputString;
}

// Function: variorum_json_call()
// Description: function that will call variorum print json and handle the
// execution errors Pre: None Post: Will print an error message if variorum
// print json fails. No return value.
char* variorum_json_call() {
  int ret;
  char *s = NULL;
  json_t* my_power_obj = NULL;
  my_power_obj         = json_object();
  ret = variorum_get_power_json(&s);
  if (ret != 0) {
    printf("First run: JSON get node power failed!\n");
  }
  return s;
}

// Function: variorum_call_mpi
// Description: This function will call the variourm helper functions and either
// write them to
//              output files or to std::cout depending on what options are
//              selected
// Pre: None
// Post: An output message if variourum returned an error or if it functioned
// correctly
void variorum_call_mpi() {
#if USE_MPI
  if (usingMPI == true) {
    int rank;
    std::string output;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::ofstream file;
    std::ofstream pfile;
    if (type_of_profiling ==
        0) {  // if both print power and json options selected
      if (mpiOutPut) {
        std::string filenamejson = printpath + "variorum-output-mpi-rank-" +
                                   std::to_string(rank) + "-json.txt";
        std::string filenameprintp = printpath + "variorum-output-mpi-rank-" +
                                     std::to_string(rank) + ".txt";

        file.open(filenamejson, std::ios_base::app);
        std::ofstream pfile;
        pfile.open(filenameprintp, std::ios_base::app);
        char* s = variorum_json_call();
        file << s;
        pfile << variorum_print_power_call();
      } else {
        std::cout << "MPI Rank " << rank << "\n";
        output  = variorum_print_power_call();
        char* s = variorum_json_call();
        puts(s);
        std::cout << s << std::endl;
      }

    } else if (type_of_profiling == 1) {  // if only print power is selected
      if (mpiOutPut) {
        std::string filenameprintp = printpath + "variorum-output-mpi-rank-" +
                                     std::to_string(rank) + ".txt";
        std::ofstream pfile;
        pfile.open(filenameprintp, std::ios_base::app);
        pfile << variorum_print_power_call();
      } else {
        std::cout << "MPI Rank " << rank << "\n";
        output = variorum_print_power_call();
        std::cout << output << std::endl;
      }
    } else if (type_of_profiling == 2) {  // if only json is selecte
      if (mpiOutPut) {
        std::string filenamejson = printpath + "variorum-output-mpi-rank-" +
                                   std::to_string(rank) + "-json.txt";
        std::ofstream file;
        file.open(filenamejson, std::ios_base::app);
        char* s = variorum_json_call();
        file << s;
      } else {
        std::cout << "MPI Rank " << rank << "\n";
        char* s = variorum_json_call();
        puts(s);
        std::cout << s << std::endl;
      }
    }
    file.close();
  }
#endif
}

// Function: variorum_call
// Description: The function determines what profiling options are selected and
// prints the profoiling data out to std out Pre: None Post: An output message
// if variourum returned an error or if it functioned correctly
void variorum_call() {
  std::string output;
 // std::cout << "Print 1 " << std::endl;
  int temp=0; 
    
    
    
    
    
    
  int num_sockets=0;

  if (type_of_profiling == 0) {
    output  = variorum_print_power_call();
    char* s = variorum_json_call();
   //std::cout << s << "\n";
        char* timestamp_pos = strstr(s, "\"timestamp\":");
    if (timestamp_pos != nullptr) {
        timestamp_pos += strlen("\"timestamp\":");
        char* timestamp_end_pos = strchr(timestamp_pos, ',');
        if (timestamp_end_pos == nullptr) {
            timestamp_end_pos = strchr(timestamp_pos, '}');
        }
        if (timestamp_end_pos != nullptr) {
            std::string timestamp_str(timestamp_pos, timestamp_end_pos - timestamp_pos);
            long long timestamp = std::stoll(timestamp_str);
           // std::cout << "Timestamp: " << timestamp << std::endl;
	    if(power==-1||time2!=0){
		time1=timestamp;
		//std::cout << ""
        }else{
		time2=timestamp;
	}
    }
    }
            char* num_sockets_pos = strstr(s, "\"num_gpus_per_socket\":");
    if (num_sockets_pos != nullptr) {
        num_sockets_pos += strlen("\"num_gpus_per_socket\":");

        char* num_sockets_end_pos = strchr(num_sockets_pos, ',');
        if (num_sockets_end_pos == nullptr) {
            num_sockets_end_pos = strchr(num_sockets_pos, '}');
        }
	if (num_sockets_end_pos != nullptr) {

            std::string num_sockets_str(num_sockets_pos, num_sockets_end_pos - num_sockets_pos);

            num_sockets = std::stoll(num_sockets_str);
  //          std::cout << "Number of Sockets: " << num_sockets << std::endl;
        }
    }
            char* power_gpu_watts_pos = strstr(s, "\"power_gpu_watts\":");




if (power_gpu_watts_pos != nullptr) {
    for (int gpu_num = 0; gpu_num <= num_sockets; ++gpu_num) {
        std::string gpu_key = "\"GPU_" + std::to_string(gpu_num) + "\":";
        char* gpu_pos = strstr(power_gpu_watts_pos, gpu_key.c_str());
        if (gpu_pos != nullptr) {
            gpu_pos += gpu_key.length();
            char* gpu_end_pos = strchr(gpu_pos, ',');
            if (gpu_end_pos == nullptr) {
                gpu_end_pos = strchr(gpu_pos, '}');
            }
            if (gpu_end_pos != nullptr) {
                std::string gpu_power_str(gpu_pos, gpu_end_pos - gpu_pos);
                float gpu_power = std::stof(gpu_power_str);
                std::cout << "GPU_" << gpu_num << " Power: " << gpu_power << " Watts  " ;
		if(power==0){gpu_powers.push_back(gpu_power);}else{gpu_powers2.push_back(gpu_power);}
		if(power==-1){
		power1+=gpu_power;
	//	std::cout << power1 <<" power currently \n" << std::endl;
	//	std::cout << power << "   power \n" << std::endl;
            }else{
		power2+=gpu_power;
		//std::cout << power << "   power \n" << std::endl;
        }
	  // std::cout << power << "  power1 \n"<< std::endl;
	  }
    }
}power+=1;
if(power2!=0){
//	std::cout << "times" << time2 << "  "<< time1 << std::endl;
	
	std::cout << "Energy Estimation\t" << ((power2+power1)/2)*((time2-time1)*.001) << std::endl;
}
//std::cout << "power 1 and 2 " << power1 << " " << power2;
if(power2!=0&&power1!=0){
	power1=0;
	power2=0;
	power =-1;
	temp=time2;
	time2=0;
}
}
for(int gpu_num = 0; gpu_num <num_sockets;++gpu_num){
	if(power!=0){
		std::cout << time2 <<"   Energy Estimation\t" << ((gpu_powers[gpu_num]+gpu_powers2[gpu_num])/2)*((time2-time1)*.001) << std::endl;
	}
}
if(power!=0){
gpu_powers.clear();
gpu_powers2.clear();}
  } else if (type_of_profiling == 1) {
    output = variorum_print_power_call();
    std::cout << output << std::endl;
  } else if (type_of_profiling == 2) {
    char* s = variorum_json_call();
    std::cout << s << std::endl;
  }


}

void kokkosp_init_library(const int loadSeq, const uint64_t interfaceVer,
                          const uint32_t devInfoCount,
                          Kokkos_Profiling_KokkosPDeviceInfo* deviceInfo) {
  char* outputPathChar;
  try {
    outputPathChar = getenv("VARIORUM_OUTPUT_PATH");
    if (outputPathChar == NULL) {
      throw 10;
    }
    std::string outputPathStr(outputPathChar);
    printpath = outputPathStr;
    std::cout << "Output Path set to" << outputPathChar << "\n";
  } catch (int e) {
    if (e == 10) {
      printpath = "./";
      std::cout << "No output path provided, the application will output to "
                   "the default path \n";
    }
  }

  // Profiling options are read in from their enviornment variables and the
  // options are set
  char* profiling_type;
  try {
    profiling_type = getenv("KOKKOS_VARIORUM_FUNC_TYPE");
    if (profiling_type == NULL) {
      throw 10;
    }
    if (strcmp(profiling_type, "ppower") == 0) {
      type_of_profiling = 1;
      std::cout << "Variorum print power will be called\n";
      if (verbosePrint == true) {
        std::cout
            << "Power Format: \n Hostname: total node power, cpuSocket1, "
               "memScoket1, gpuSocket1, cpuSocket2, memScoket2, gpuSocket2 \n";
      }
    } else if (strcmp(profiling_type, "json") == 0) {
      type_of_profiling = 2;
    } else if (strcmp(profiling_type, "both") == 0) {
      type_of_profiling = 0;
    }
  } catch (int e) {
    if (e == 10) {
      type_of_profiling = 0;
      std::cout << "No profiling options provided, profiling tool will call "
                   "variorum print power and json \n";
    }
  }
  try {
    char* verbosePrintStr = getenv("VERBOSE");
    if (verbosePrintStr == NULL) {
      throw 10;
    }
    if (strcmp(verbosePrintStr, "false") == 0 ||
        strcmp(verbosePrintStr, "") == 0) {
      throw 20;
    } else if (strcmp(verbosePrintStr, "true") == 0) {
      std::cout << "Verbose power option set"
                << "\n";
      verbosePrint = true;
    }
  } catch (int e) {
    verbosePrint = false;
    std::cout << "No verbose options provided, power information outut will "
                 "not be verbose \n Format - Hosntame : Node power value\n";
  }
  try {
    char* usingMPIstr = getenv("VARIORUM_USING_MPI");
    if (usingMPIstr == NULL) {
      throw 10;
    }
    if (strcmp(usingMPIstr, "false") == 0 || strcmp(usingMPIstr, "") == 0) {
      throw 20;
    }
    if (strcmp(usingMPIstr, "true") == 0) {
#if USE_MPI
      usingMPI = true;
      try {
        char* perRankOutput = getenv("RANKED_OUTPUT");
        if (strcmp(perRankOutput, "false") == 0 ||
            strcmp(perRankOutput, "") == 0) {
          mpiOutPut = false;
        } else if (strcmp(perRankOutput, "true") == 0) {
          mpiOutPut = true;
        } else {
          mpiOutPut = false;
        }

      } catch (int f) {
        std::cout << "Ranked output will no be used, error setting paramters"
                  << std::endl;
        mpiOutPut = false;
      }
#else
      usingMPI = false;
      std::cout << "Ignoring MPI enabled in Variorum: the connector was built "
                   "without MPI support"
                << std::endl;
#endif
    }
  } catch (int e) {
    std::cout << "No MPI Option provided, not using per rank output"
              << std::endl;
    usingMPI = false;
  }
  // Simple timer code to keep track of the general amount of time the
  // application ran for.
  time(&start_time);
  std::cout << "Start Time: " << start_time << "\n";

  // variorum_call();
}

void kokkosp_finalize_library() {
/*	std::cout << "kokkos library call\n" << std::endl;
  if (usingMPI) {
    variorum_call_mpi();
  } else {
   variorum_call();
  }
  time_t total_time;
  time_t end_time;
  time(&end_time);
  std::cout << "End Time: " << end_time << "\nStart Time: " << start_time
            << "\n";
  total_time = end_time - start_time;

  std::cout << "The kokkos library was alive for " << total_time << " seconds."
            << std::endl;*/
}


std::ostream& operator<<(std::ostream& os, const Kokkos::Tools::Experimental::DeviceType& deviceType) {
    // Example of mapping enum values to strings
    switch (deviceType) {
            case Kokkos::Tools::Experimental::DeviceType::Serial:
            os << "CPU";
            break;
        case Kokkos::Tools::Experimental::DeviceType::OpenMP:
            os << "OpenMP";
            break;
        case Kokkos::Tools::Experimental::DeviceType::Cuda:
            os << "cuda";
            break;
        case Kokkos::Tools::Experimental::DeviceType::HIP:
            os << "hip";
            break;
        case Kokkos::Tools::Experimental::DeviceType::OpenMPTarget:
            os << "openmptarget";
            break;
        case Kokkos::Tools::Experimental::DeviceType::HPX:
            os << "hpx";
            break;
        case Kokkos::Tools::Experimental::DeviceType::Threads:
            os << "threads";
            break;
            case Kokkos::Tools::Experimental::DeviceType::SYCL:
            os << "sycl";
            break;
            case Kokkos::Tools::Experimental::DeviceType::OpenACC:
            os << "openacc";
            break;
            case Kokkos::Tools::Experimental::DeviceType::Unknown:
            os << "Unkown";
            break;

        default:
            os << "Unknown Device Type";
            break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Kokkos::Tools::Experimental::ExecutionSpaceIdentifier& identifier) {
    os << "Execution Space Identifier:\n";
    os << "Type: " << identifier.type << "\n";
    os << "Device ID: " << identifier.device_id << "\n";
    os << "Instance ID: " << identifier.instance_id << "\n";
    return os;
}

void kokkosp_begin_parallel_for(const char* name, const uint32_t devID,
                                uint64_t* kID) {
  std::cout << "Device ID: " << devID << "\n";
  std::cout << "kernel ID" << kID << "\n";
  std::cout << "name" << name << "\n";
  auto result2 = Kokkos::Tools::Experimental::identifier_from_devid(devID);  
  std::cout << result2 << std::endl;
  if (usingMPI) {
    variorum_call_mpi();
  } else {
    variorum_call();
  }
}

void kokkosp_end_parallel_for(const uint64_t kID) {
  if (usingMPI) {
   variorum_call_mpi();
  } else {
   variorum_call();
  }
  
}

void kokkosp_begin_parallel_scan(const char* name, const uint32_t devID,
                                 uint64_t* kID) {
  std::cout << "Device ID: " << devID << "\n";
  //std::cout << "kernel ID"
  std::cout << "kernel ID2" << kID << "\n";
  std::cout << "name" << name << "\n";
  auto result1 = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  std::cout << result1 << std::endl;
  if (usingMPI) {
   variorum_call_mpi();
  } else {
   variorum_call();
  }
}

void kokkosp_end_parallel_scan(const uint64_t kID) {
  if (usingMPI) {
   variorum_call_mpi();
  } else {
   variorum_call();
  }
}

void kokkosp_begin_parallel_reduce(const char* name, const uint32_t devID,
                                   uint64_t* kID) {
       	std::cout << "Device ID: " << devID << "\n";
        std::cout << "kernel ID33" << kID << "\n";
	std::cout << "name" << name << "\n";
//	Kokkos::Tools::Experimental::ExecutionSpaceIdentifier result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
//	std::cout << result << std::endl;
	auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
	std::cout << result << std::endl;  
//	printExecutionSpaceIdentifier(result);
  if (usingMPI) {
    variorum_call_mpi();
  } else {
    variorum_call();
  }
  //  std::cout << "power1 and two" << power1 << " " << power2 << std::endl;

}

void kokkosp_end_parallel_reduce(const uint64_t kID) {
  if (usingMPI) {
    variorum_call_mpi();
  } else {
    variorum_call();
  }
 // std::cout << "power1 and two" << power1 << " " << power2 << std::endl;
}

Kokkos::Tools::Experimental::EventSet get_event_set() {
  Kokkos::Tools::Experimental::EventSet my_event_set;
  memset(&my_event_set, 0,
         sizeof(my_event_set));  // zero any pointers not set here
  my_event_set.init                  = kokkosp_init_library;
  my_event_set.finalize              = kokkosp_finalize_library;
  my_event_set.begin_parallel_for    = kokkosp_begin_parallel_for;
  my_event_set.begin_parallel_reduce = kokkosp_begin_parallel_reduce;
  my_event_set.begin_parallel_scan   = kokkosp_begin_parallel_scan;
  my_event_set.end_parallel_for      = kokkosp_end_parallel_for;
  my_event_set.end_parallel_reduce   = kokkosp_end_parallel_reduce;
  my_event_set.end_parallel_scan     = kokkosp_end_parallel_scan;
  return my_event_set;
}

}  // namespace VariorumConnector
}  // namespace KokkosTools

extern "C" {

namespace impl = KokkosTools::VariorumConnector;

EXPOSE_INIT(impl::kokkosp_init_library)
EXPOSE_FINALIZE(impl::kokkosp_finalize_library)
EXPOSE_BEGIN_PARALLEL_FOR(impl::kokkosp_begin_parallel_for)
EXPOSE_END_PARALLEL_FOR(impl::kokkosp_end_parallel_for)
EXPOSE_BEGIN_PARALLEL_SCAN(impl::kokkosp_begin_parallel_scan)
EXPOSE_END_PARALLEL_SCAN(impl::kokkosp_end_parallel_scan)
EXPOSE_BEGIN_PARALLEL_REDUCE(impl::kokkosp_begin_parallel_reduce)
EXPOSE_END_PARALLEL_REDUCE(impl::kokkosp_end_parallel_reduce)

}  // extern "C"

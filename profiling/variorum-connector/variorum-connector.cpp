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
//#include <cxxabi.h>
//#include <dlfcn.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
int power       = -1;
int power1      = 0;
int power2      = 0;
int filemake    = -1;
long long time1 = 0;
long long time2 = 0;
uint32_t gdevID = -1;
std::vector<float> gpu_powers;
std::vector<float> gpu_powers2;
std::string output   = "";
std::string filename = "variorumoutput.txt";
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
// bool usingMPI     = false;
bool verbosePrint = true;
// bool mpiOutPut    = false;

void kokkosp_parse_args(int argc, char** argv) {
  // See description in original PR.
  // argc will always be at least 1 (exe)
  if (argc == 1) {
    // No argument, use the default
  } else if (argc == 2) {
    // User specified a threshold
    //  output_threshold = strtod(argv[1], 0);
  } else {
    // Too many args
    //   kokkosp_print_help(argv[0]);
    exit(1);
  }
}
void printFile() {
  //  std::string filename = "variorumoutput.txt";

  /*    char* parsed_output_file = getenv("KOKKOS_TOOLS_VARIORUM_OUTPUT_FILE");
  if (!parsed_output_file) {
    std::cerr << "Couldn't parse KOKKOS_TOOLS_VARIORUM_OUTPUT_FILE environment
  variable!\n"; std::abort();
    }
  std::string filename = parsed_output_file;
  */

  std::ifstream infile(filename);
  if (infile.good()) {
    infile.close();
    std::ofstream file(filename, std::ios::trunc);
  } else {
    std::ofstream file(filename, std::ios::app);
  }
  infile.close();
}
void writeToFile(const std::string& filename, const std::string& data) {
  std::ofstream file(filename, std::ios::app);
  if (file) {
    file << data;
  }
}

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
  char* s = NULL;
  double power_node, power_sock0, power_mem0, power_gpu0;
  double power_sock1, power_mem1, power_gpu1;
  int ret;
  ret = variorum_get_power_json(&s);
  if (ret != 0) {
    return "Print power failed!\n";
  }
  json_t* power_obj = json_loads(s, JSON_DECODE_ANY, NULL);
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

    // outputString += "HostName " + hostname + "\n";
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
    // outputString += hostname + ": " + std::to_string(power_node) + "\n";
  }

  return outputString;
}

// Function: variorum_json_call()
// Description: function that will call variorum print json and handle the
// execution errors Pre: None Post: Will print an error message if variorum
// print json fails. No return value.
char* variorum_json_call() {
  int ret;
  char* s              = NULL;
  json_t* my_power_obj = NULL;
  my_power_obj         = json_object();
  ret                  = variorum_get_power_json(&s);
  if (ret != 0) {
    printf("First run: JSON get node power failed!\n");
  }
  return s;
}

// Function: variorum_call
// Description: The function determines what profiling options are selected and
// prints the profoiling data out to std out Pre: None Post: An output message
// if variourum returned an error or if it functioned correctly
void variorum_call() {
  // std::cout <<output  << std::endl;
  long long temp = 0;

  int num_sockets = 0;

  if (type_of_profiling == 0) {
    output  = variorum_print_power_call();
    char* s = variorum_json_call();

    power += 1;

    if (power != 0) {
      gpu_powers.clear();
      gpu_powers2.clear();
    }
    // std::cout<< s << std::endl;

    json_error_t error;
    json_t* root            = NULL;
    json_t* socket_0        = NULL;
    json_t* timestamp_value = NULL;
    json_t* power_gpu_watts = NULL;
    json_t* gpu_0_value     = NULL;

    // Parse JSON string into a json_t object
    root = json_loads(s, 0, &error);
    if (!root) {
      fprintf(stderr, "Error parsing JSON: %s\n", error.text);
    }

    const char* key;
    json_t* value;
    json_object_foreach(root, key, value) {
      if (json_is_object(value)) {
        json_t* socket_object = json_object_get(value, "socket_0");
        if (socket_object && json_is_object(socket_object)) {
          socket_0        = socket_object;
          timestamp_value = json_object_get(value, "timestamp");
          if (time1 == 0) {
            time1 = (long long)json_integer_value(timestamp_value);
          } else {
            time2 = (long long)json_integer_value(timestamp_value);
          }
          break;
        }
      }
    }

    if (!socket_0 || !timestamp_value) {
      fprintf(stderr, "Failed to find 'socket_0' object or 'timestamp'.\n");
    }

    // Access power_gpu_watts within socket_0
    power_gpu_watts = json_object_get(socket_0, "power_gpu_watts");
    if (!json_is_object(power_gpu_watts)) {
      fprintf(stderr, "Expected 'power_gpu_watts' to be an object.\n");
    }
    if (gdevID == 0) {
      // Access GPU_0 value within power_gpu_watts
      gpu_0_value = json_object_get(power_gpu_watts, "GPU_0");
      if (!json_is_real(gpu_0_value)) {
        fprintf(stderr, "Expected 'GPU_0' to be a number.\n");
      }
      double gpu_0 = json_real_value(gpu_0_value);
      if (power1 == 0) {
        power1 = gpu_0;
      } else {
        power2 = gpu_0;
      }
    }

    if (gdevID == 0) {
      switch (gdevID) {
        case 1: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_1");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);
            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 2: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_2");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 3: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_3");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 4: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_4");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 5: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_5");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 6: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_6");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
        case 7: {
          json_t* gpu_value = json_object_get(power_gpu_watts, "GPU_7");
          if (json_is_real(gpu_value)) {
            double gpu_power = json_real_value(gpu_value);

            if (power1 == 0) {
              power1 = gpu_power;
            } else {
              power2 = gpu_power;
            }
          }
          break;
        }
      }
    }

    if (power2 != 0 && power1 != 0) {
      temp = time2;

      output = "  Energy Estimation " + std::to_string(((power1 + power2) / 2) *
                                                       ((temp - time1) * .001));

      writeToFile(filename, output);
      power1 = 0;
      power2 = 0;
      power  = -1;
      time2  = 0;
    }
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
    }
  } catch (int e) {
    std::cout << "No MPI Option provided, not using per rank output"
              << std::endl;
    //  usingMPI = false;
  }
  // Simple timer code to keep track of the general amount of time the
  // application ran for.
  time(&start_time);
  std::cout << "Start Time: " << start_time << "\n";

  // variorum_call();
}

std::string deviceTypeToString(
    const Kokkos::Tools::Experimental::DeviceType& deviceType) {
  switch (deviceType) {
    case Kokkos::Tools::Experimental::DeviceType::Serial:
      return "SERIAL";
      break;
    case Kokkos::Tools::Experimental::DeviceType::OpenMP:
      return "OPENMP";
      break;
    case Kokkos::Tools::Experimental::DeviceType::Cuda: return "CUDA"; break;
    case Kokkos::Tools::Experimental::DeviceType::HIP: return "HIP"; break;
    case Kokkos::Tools::Experimental::DeviceType::OpenMPTarget:
      return "OPENMPTARGET";
      break;
    case Kokkos::Tools::Experimental::DeviceType::HPX: return "HPX"; break;
    case Kokkos::Tools::Experimental::DeviceType::Threads:
      return "THREADS";
      break;
    case Kokkos::Tools::Experimental::DeviceType::SYCL: return "SYCL"; break;
    case Kokkos::Tools::Experimental::DeviceType::OpenACC:
      return "OPENACC";
      break;
    default: return "UNKOWN";
  }
}

std::ostream& operator<<(
    std::ostream& os,
    const Kokkos::Tools::Experimental::ExecutionSpaceIdentifier& identifier) {
  gdevID = identifier.device_id;

  output +=
      " Device ID: " + std::to_string(identifier.device_id) +
      " Instance ID: " + std::to_string(identifier.instance_id) +
      " DeviceType: " +
      deviceTypeToString(static_cast<Kokkos::Tools::Experimental::DeviceType>(
          identifier.device_id));

  writeToFile(filename, output);
  return os;
}

void kokkosp_begin_parallel_for(const char* name, const uint32_t devID,
                                uint64_t* kID) {
  if (filemake == -1) {
    printFile();
    filemake++;
  }

  writeToFile(filename, "name: " + std::string(name));
  gdevID      = devID;
  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  std::cout << result;
  variorum_call();
}

void kokkosp_end_parallel_for(const uint64_t kID) { variorum_call(); }

void kokkosp_begin_parallel_scan(const char* name, const uint32_t devID,
                                 uint64_t* kID) {
  if (filemake == -1) {
    printFile();
    filemake++;
  }
  writeToFile(filename, "name: " + std::string(name));
  gdevID      = devID;
  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  std::cout << result;
  variorum_call();
}

void kokkosp_end_parallel_scan(const uint64_t kID) { variorum_call(); }

void kokkosp_begin_parallel_reduce(const char* name, const uint32_t devID,
                                   uint64_t* kID) {
  if (filemake == -1) {
    printFile();
    filemake++;
  }
  writeToFile(filename, "name: " + std::string(name));
  gdevID      = devID;
  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  std::cout << result;
  variorum_call();
}

void kokkosp_end_parallel_reduce(const uint64_t kID) { variorum_call(); }

Kokkos::Tools::Experimental::EventSet get_event_set() {
  Kokkos::Tools::Experimental::EventSet my_event_set;
  memset(&my_event_set, 0,
         sizeof(my_event_set));  // zero any pointers not set here
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

EXPOSE_BEGIN_PARALLEL_FOR(impl::kokkosp_begin_parallel_for)
EXPOSE_END_PARALLEL_FOR(impl::kokkosp_end_parallel_for)
EXPOSE_BEGIN_PARALLEL_SCAN(impl::kokkosp_begin_parallel_scan)
EXPOSE_END_PARALLEL_SCAN(impl::kokkosp_end_parallel_scan)
EXPOSE_BEGIN_PARALLEL_REDUCE(impl::kokkosp_begin_parallel_reduce)
EXPOSE_END_PARALLEL_REDUCE(impl::kokkosp_end_parallel_reduce)

}  // extern "C"

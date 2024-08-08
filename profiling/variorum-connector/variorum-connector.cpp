//@HEADER
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
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
int power = -1;  // This variable is used to keep track of if variorum call has
                 // been called twice to check if the variables need to be reset
                 // for the next kernel
double power1 =
    0;  // This variable is used to obtain the initial power for calculation
double power2 =
    0;  // This variable is used to obtain the final power for calculation
int filemake = -1;  // This variable is to check if the file has already been
                    // made earlier in the program
long long time1 =
    0;  // This variable is used to obtain the initial time for calculation
long long time2 =
    0;  // This variable is used to obtain the final time for calculation
uint32_t gdevID = -1;  // This variable is used to access the device ID from any
                       // function throughout the program
std::string filename;  // This variable is to access the file from any function
                       // throughout the project
// This is a global output string that can be used to place variables to be
// printed to the output file
std::string output = "";
// This variable is a file name variorum output for the output of the variorum
// call
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

inline std::string getFile(const char* env_var_name) {
  char* parsed_output_file = getenv(env_var_name);
  if (!parsed_output_file) {
    std::cerr << "Couldn't parse KOKKOS_TOOLS_VARIORUM_OUTPUT_FILE environment "
                 "variable! Printed to variorumoutput.txt\n";
    //  parsed_output_file = "variorumoutput.txt";
    char vararr[19]    = "variorumoutput.txt";
    parsed_output_file = vararr;
  }
  return std::string(parsed_output_file);
}

void printFile() {
  filename = getFile("KOKKOS_TOOLS_VARIORUM_OUTPUT_FILE");

  std::ifstream infile(filename);
  if (infile.good()) {
    infile.close();
    std::ofstream file(filename, std::ios::trunc);
  } else {
    std::ofstream file(filename, std::ios::app);
  }
}
inline void writeToFile(const std::string& filename, const std::string& data) {
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
  char* s = nullptr;
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
  long long temp = 0;

  int num_sockets = 0;

  if (type_of_profiling == 0) {
    output  = variorum_print_power_call();
    char* s = variorum_json_call();

    power += 1;

    json_error_t error;
    json_t* root            = nullptr;
    json_t* socket_0        = nullptr;
    json_t* timestamp_value = nullptr;
    json_t* power_gpu_watts = nullptr;
    json_t* gpu_0_value     = nullptr;

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
    // FIXME We assume that all GPUs are on socket 0 for now.
    if (!socket_0 || !timestamp_value) {
      fprintf(stderr, "Failed to find 'socket_0' object or 'timestamp'.\n");
    }

    // Access power_gpu_watts within socket_0
    power_gpu_watts = json_object_get(socket_0, "power_gpu_watts");
    if (!json_is_object(power_gpu_watts)) {
      fprintf(stderr, "Expected 'power_gpu_watts' to be an object.\n");
    }

    std::string gpu_key = "GPU_" + std::to_string(gdevID);
    json_t* power_value = json_object_get(power_gpu_watts, gpu_key.c_str());
    if (json_is_number(power_value)) {
      double power_val = json_number_value(power_value);
      if (power1 == 0) {
        power1 = power_val;
      } else {
        power2 = power_val;
      }

    } else {
      std::cerr << "Error: GPU key " << gpu_key << " not found or not a number"
                << std::endl;
    }

    if (power2 != 0 && power1 != 0) {
      temp = time2;
      std::string calc =
          std::to_string((((power1 + power2) / 2) * ((temp - time1) * .001)));

      writeToFile(filename, " Energy Estimation in Joules " + calc);
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
    }
  } catch (int e) {
    std::cout << "No verbose options provided, power information outut will "
                 "not be verbose \n Format - Hosntame : Node power value\n";
  }
  // Simple timer code to keep track of the general amount of time the
  // application ran for.
  time(&start_time);
  std::cout << "Start Time: " << start_time << "\n";
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

void kokkosp_begin_parallel_for(const char* name, const uint32_t devID,
                                uint64_t* kID) {
  if (filemake == -1) {
    printFile();
    filemake++;
  }

  writeToFile(filename, "name: " + std::string(name) + "  ");

  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  output +=
      " Device ID: " + std::to_string(result.device_id) +
      " Instance ID: " + std::to_string(result.instance_id) + " DeviceType: " +
      deviceTypeToString(static_cast<Kokkos::Tools::Experimental::DeviceType>(
          result.device_id));
  gdevID = result.device_id;
  writeToFile(filename, output);

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

  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  output +=
      " Device ID: " + std::to_string(result.device_id) +
      " Instance ID: " + std::to_string(result.instance_id) + " DeviceType: " +
      deviceTypeToString(static_cast<Kokkos::Tools::Experimental::DeviceType>(
          result.device_id));
  gdevID = result.device_id;
  writeToFile(filename, output);
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

  auto result = Kokkos::Tools::Experimental::identifier_from_devid(devID);
  output +=
      " Device ID: " + std::to_string(result.device_id) +
      " Instance ID: " + std::to_string(result.instance_id) + " DeviceType: " +
      deviceTypeToString(static_cast<Kokkos::Tools::Experimental::DeviceType>(
          result.device_id));
  writeToFile(filename, output);
  gdevID = result.device_id;
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

# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kuw/kokkos-tools

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kuw/kokkos-tools/build

# Include any dependencies generated for this target.
include profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/compiler_depend.make

# Include the progress variables for this target.
include profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/progress.make

# Include the compile flags for this target's objects.
include profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/flags.make

profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o: profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/flags.make
profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o: ../profiling/simple-kernel-timer/kp_reader.cpp
profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o: profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o"
	cd /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o -MF CMakeFiles/kp_reader.dir/kp_reader.cpp.o.d -o CMakeFiles/kp_reader.dir/kp_reader.cpp.o -c /home/kuw/kokkos-tools/profiling/simple-kernel-timer/kp_reader.cpp

profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kp_reader.dir/kp_reader.cpp.i"
	cd /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kuw/kokkos-tools/profiling/simple-kernel-timer/kp_reader.cpp > CMakeFiles/kp_reader.dir/kp_reader.cpp.i

profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kp_reader.dir/kp_reader.cpp.s"
	cd /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kuw/kokkos-tools/profiling/simple-kernel-timer/kp_reader.cpp -o CMakeFiles/kp_reader.dir/kp_reader.cpp.s

# Object files for target kp_reader
kp_reader_OBJECTS = \
"CMakeFiles/kp_reader.dir/kp_reader.cpp.o"

# External object files for target kp_reader
kp_reader_EXTERNAL_OBJECTS =

profiling/simple-kernel-timer/kp_reader: profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/kp_reader.cpp.o
profiling/simple-kernel-timer/kp_reader: profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/build.make
profiling/simple-kernel-timer/kp_reader: profiling/simple-kernel-timer/libkp_kernel_timer.so
profiling/simple-kernel-timer/kp_reader: profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kp_reader"
	cd /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kp_reader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/build: profiling/simple-kernel-timer/kp_reader
.PHONY : profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/build

profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/clean:
	cd /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer && $(CMAKE_COMMAND) -P CMakeFiles/kp_reader.dir/cmake_clean.cmake
.PHONY : profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/clean

profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/depend:
	cd /home/kuw/kokkos-tools/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuw/kokkos-tools /home/kuw/kokkos-tools/profiling/simple-kernel-timer /home/kuw/kokkos-tools/build /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer /home/kuw/kokkos-tools/build/profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : profiling/simple-kernel-timer/CMakeFiles/kp_reader.dir/depend

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
include profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/compiler_depend.make

# Include the progress variables for this target.
include profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/progress.make

# Include the compile flags for this target's objects.
include profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/flags.make

profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o: profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/flags.make
profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o: ../profiling/chrome-tracing/kp_chrome_tracing.cpp
profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o: profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o"
	cd /home/kuw/kokkos-tools/build/profiling/chrome-tracing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o -MF CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o.d -o CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o -c /home/kuw/kokkos-tools/profiling/chrome-tracing/kp_chrome_tracing.cpp

profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.i"
	cd /home/kuw/kokkos-tools/build/profiling/chrome-tracing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kuw/kokkos-tools/profiling/chrome-tracing/kp_chrome_tracing.cpp > CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.i

profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.s"
	cd /home/kuw/kokkos-tools/build/profiling/chrome-tracing && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kuw/kokkos-tools/profiling/chrome-tracing/kp_chrome_tracing.cpp -o CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.s

# Object files for target kp_chrome_tracing
kp_chrome_tracing_OBJECTS = \
"CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o"

# External object files for target kp_chrome_tracing
kp_chrome_tracing_EXTERNAL_OBJECTS =

profiling/chrome-tracing/libkp_chrome_tracing.so: profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/kp_chrome_tracing.cpp.o
profiling/chrome-tracing/libkp_chrome_tracing.so: profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/build.make
profiling/chrome-tracing/libkp_chrome_tracing.so: profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libkp_chrome_tracing.so"
	cd /home/kuw/kokkos-tools/build/profiling/chrome-tracing && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kp_chrome_tracing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/build: profiling/chrome-tracing/libkp_chrome_tracing.so
.PHONY : profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/build

profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/clean:
	cd /home/kuw/kokkos-tools/build/profiling/chrome-tracing && $(CMAKE_COMMAND) -P CMakeFiles/kp_chrome_tracing.dir/cmake_clean.cmake
.PHONY : profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/clean

profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/depend:
	cd /home/kuw/kokkos-tools/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuw/kokkos-tools /home/kuw/kokkos-tools/profiling/chrome-tracing /home/kuw/kokkos-tools/build /home/kuw/kokkos-tools/build/profiling/chrome-tracing /home/kuw/kokkos-tools/build/profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : profiling/chrome-tracing/CMakeFiles/kp_chrome_tracing.dir/depend

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
include profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/compiler_depend.make

# Include the progress variables for this target.
include profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/progress.make

# Include the compile flags for this target's objects.
include profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/flags.make

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/flags.make
profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o: ../profiling/perfetto-connector/libperfetto-connector.cpp
profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o -MF CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o.d -o CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o -c /home/kuw/kokkos-tools/profiling/perfetto-connector/libperfetto-connector.cpp

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.i"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kuw/kokkos-tools/profiling/perfetto-connector/libperfetto-connector.cpp > CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.i

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.s"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kuw/kokkos-tools/profiling/perfetto-connector/libperfetto-connector.cpp -o CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.s

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/flags.make
profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o: ../profiling/perfetto-connector/perfetto/perfetto.cc
profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o -MF CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o.d -o CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o -c /home/kuw/kokkos-tools/profiling/perfetto-connector/perfetto/perfetto.cc

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.i"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kuw/kokkos-tools/profiling/perfetto-connector/perfetto/perfetto.cc > CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.i

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.s"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kuw/kokkos-tools/profiling/perfetto-connector/perfetto/perfetto.cc -o CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.s

# Object files for target kp_perfetto_connector
kp_perfetto_connector_OBJECTS = \
"CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o" \
"CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o"

# External object files for target kp_perfetto_connector
kp_perfetto_connector_EXTERNAL_OBJECTS =

profiling/perfetto-connector/libkp_perfetto_connector.so: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/libperfetto-connector.cpp.o
profiling/perfetto-connector/libkp_perfetto_connector.so: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/perfetto/perfetto.cc.o
profiling/perfetto-connector/libkp_perfetto_connector.so: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/build.make
profiling/perfetto-connector/libkp_perfetto_connector.so: profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kuw/kokkos-tools/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libkp_perfetto_connector.so"
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kp_perfetto_connector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/build: profiling/perfetto-connector/libkp_perfetto_connector.so
.PHONY : profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/build

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/clean:
	cd /home/kuw/kokkos-tools/build/profiling/perfetto-connector && $(CMAKE_COMMAND) -P CMakeFiles/kp_perfetto_connector.dir/cmake_clean.cmake
.PHONY : profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/clean

profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/depend:
	cd /home/kuw/kokkos-tools/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kuw/kokkos-tools /home/kuw/kokkos-tools/profiling/perfetto-connector /home/kuw/kokkos-tools/build /home/kuw/kokkos-tools/build/profiling/perfetto-connector /home/kuw/kokkos-tools/build/profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : profiling/perfetto-connector/CMakeFiles/kp_perfetto_connector.dir/depend

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
CMAKE_SOURCE_DIR = //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build

# Include any dependencies generated for this target.
include CMakeFiles/test_alex.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_alex.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_alex.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_alex.dir/flags.make

CMakeFiles/test_alex.dir/test/unittest_main.cpp.o: CMakeFiles/test_alex.dir/flags.make
CMakeFiles/test_alex.dir/test/unittest_main.cpp.o: ../test/unittest_main.cpp
CMakeFiles/test_alex.dir/test/unittest_main.cpp.o: CMakeFiles/test_alex.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=//home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_alex.dir/test/unittest_main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_alex.dir/test/unittest_main.cpp.o -MF CMakeFiles/test_alex.dir/test/unittest_main.cpp.o.d -o CMakeFiles/test_alex.dir/test/unittest_main.cpp.o -c //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/test/unittest_main.cpp

CMakeFiles/test_alex.dir/test/unittest_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_alex.dir/test/unittest_main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/test/unittest_main.cpp > CMakeFiles/test_alex.dir/test/unittest_main.cpp.i

CMakeFiles/test_alex.dir/test/unittest_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_alex.dir/test/unittest_main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/test/unittest_main.cpp -o CMakeFiles/test_alex.dir/test/unittest_main.cpp.s

# Object files for target test_alex
test_alex_OBJECTS = \
"CMakeFiles/test_alex.dir/test/unittest_main.cpp.o"

# External object files for target test_alex
test_alex_EXTERNAL_OBJECTS =

test_alex: CMakeFiles/test_alex.dir/test/unittest_main.cpp.o
test_alex: CMakeFiles/test_alex.dir/build.make
test_alex: CMakeFiles/test_alex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=//home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_alex"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_alex.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_alex.dir/build: test_alex
.PHONY : CMakeFiles/test_alex.dir/build

CMakeFiles/test_alex.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_alex.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_alex.dir/clean

CMakeFiles/test_alex.dir/depend:
	cd //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build //home/pbecket1/Documents/CS6530Project/projectFinal/ALEX/build/CMakeFiles/test_alex.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_alex.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_SOURCE_DIR = /outOfSpaceShaders

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /outOfSpaceShaders/build

# Include any dependencies generated for this target.
include CMakeFiles/CG.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CG.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CG.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CG.dir/flags.make

CMakeFiles/CG.dir/src/main.cpp.o: CMakeFiles/CG.dir/flags.make
CMakeFiles/CG.dir/src/main.cpp.o: /outOfSpaceShaders/src/main.cpp
CMakeFiles/CG.dir/src/main.cpp.o: CMakeFiles/CG.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/outOfSpaceShaders/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CG.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CG.dir/src/main.cpp.o -MF CMakeFiles/CG.dir/src/main.cpp.o.d -o CMakeFiles/CG.dir/src/main.cpp.o -c /outOfSpaceShaders/src/main.cpp

CMakeFiles/CG.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CG.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /outOfSpaceShaders/src/main.cpp > CMakeFiles/CG.dir/src/main.cpp.i

CMakeFiles/CG.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CG.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /outOfSpaceShaders/src/main.cpp -o CMakeFiles/CG.dir/src/main.cpp.s

# Object files for target CG
CG_OBJECTS = \
"CMakeFiles/CG.dir/src/main.cpp.o"

# External object files for target CG
CG_EXTERNAL_OBJECTS =

CG: CMakeFiles/CG.dir/src/main.cpp.o
CG: CMakeFiles/CG.dir/build.make
CG: /usr/lib/x86_64-linux-gnu/libGLEW.so
CG: /usr/lib/x86_64-linux-gnu/libOpenGL.so
CG: /usr/lib/x86_64-linux-gnu/libGLX.so
CG: /usr/lib/x86_64-linux-gnu/libGLU.so
CG: CMakeFiles/CG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/outOfSpaceShaders/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CG"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CG.dir/build: CG
.PHONY : CMakeFiles/CG.dir/build

CMakeFiles/CG.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CG.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CG.dir/clean

CMakeFiles/CG.dir/depend:
	cd /outOfSpaceShaders/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /outOfSpaceShaders /outOfSpaceShaders /outOfSpaceShaders/build /outOfSpaceShaders/build /outOfSpaceShaders/build/CMakeFiles/CG.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/CG.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cs18/Desktop/Ass/Assignment2/Assignment2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cs18/Desktop/Ass/Assignment2/Assignment2/build

# Include any dependencies generated for this target.
include CMakeFiles/cross.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cross.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cross.dir/flags.make

CMakeFiles/cross.dir/cross_product.cpp.o: CMakeFiles/cross.dir/flags.make
CMakeFiles/cross.dir/cross_product.cpp.o: ../cross_product.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cs18/Desktop/Ass/Assignment2/Assignment2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/cross.dir/cross_product.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/cross.dir/cross_product.cpp.o -c /home/cs18/Desktop/Ass/Assignment2/Assignment2/cross_product.cpp

CMakeFiles/cross.dir/cross_product.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/cross.dir/cross_product.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cs18/Desktop/Ass/Assignment2/Assignment2/cross_product.cpp > CMakeFiles/cross.dir/cross_product.cpp.i

CMakeFiles/cross.dir/cross_product.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/cross.dir/cross_product.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cs18/Desktop/Ass/Assignment2/Assignment2/cross_product.cpp -o CMakeFiles/cross.dir/cross_product.cpp.s

CMakeFiles/cross.dir/cross_product.cpp.o.requires:

.PHONY : CMakeFiles/cross.dir/cross_product.cpp.o.requires

CMakeFiles/cross.dir/cross_product.cpp.o.provides: CMakeFiles/cross.dir/cross_product.cpp.o.requires
	$(MAKE) -f CMakeFiles/cross.dir/build.make CMakeFiles/cross.dir/cross_product.cpp.o.provides.build
.PHONY : CMakeFiles/cross.dir/cross_product.cpp.o.provides

CMakeFiles/cross.dir/cross_product.cpp.o.provides.build: CMakeFiles/cross.dir/cross_product.cpp.o


# Object files for target cross
cross_OBJECTS = \
"CMakeFiles/cross.dir/cross_product.cpp.o"

# External object files for target cross
cross_EXTERNAL_OBJECTS =

cross: CMakeFiles/cross.dir/cross_product.cpp.o
cross: CMakeFiles/cross.dir/build.make
cross: CMakeFiles/cross.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cs18/Desktop/Ass/Assignment2/Assignment2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable cross"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cross.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cross.dir/build: cross

.PHONY : CMakeFiles/cross.dir/build

CMakeFiles/cross.dir/requires: CMakeFiles/cross.dir/cross_product.cpp.o.requires

.PHONY : CMakeFiles/cross.dir/requires

CMakeFiles/cross.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cross.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cross.dir/clean

CMakeFiles/cross.dir/depend:
	cd /home/cs18/Desktop/Ass/Assignment2/Assignment2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cs18/Desktop/Ass/Assignment2/Assignment2 /home/cs18/Desktop/Ass/Assignment2/Assignment2 /home/cs18/Desktop/Ass/Assignment2/Assignment2/build /home/cs18/Desktop/Ass/Assignment2/Assignment2/build /home/cs18/Desktop/Ass/Assignment2/Assignment2/build/CMakeFiles/cross.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cross.dir/depend


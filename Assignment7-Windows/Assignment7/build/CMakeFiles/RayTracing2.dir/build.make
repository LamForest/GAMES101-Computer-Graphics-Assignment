# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build

# Include any dependencies generated for this target.
include CMakeFiles/RayTracing2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RayTracing2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RayTracing2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RayTracing2.dir/flags.make

CMakeFiles/RayTracing2.dir/main.cpp.obj: CMakeFiles/RayTracing2.dir/flags.make
CMakeFiles/RayTracing2.dir/main.cpp.obj: ../main.cpp
CMakeFiles/RayTracing2.dir/main.cpp.obj: CMakeFiles/RayTracing2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RayTracing2.dir/main.cpp.obj"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing2.dir/main.cpp.obj -MF CMakeFiles\RayTracing2.dir\main.cpp.obj.d -o CMakeFiles\RayTracing2.dir\main.cpp.obj -c H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\main.cpp

CMakeFiles/RayTracing2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing2.dir/main.cpp.i"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\main.cpp > CMakeFiles\RayTracing2.dir\main.cpp.i

CMakeFiles/RayTracing2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing2.dir/main.cpp.s"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\main.cpp -o CMakeFiles\RayTracing2.dir\main.cpp.s

CMakeFiles/RayTracing2.dir/Vector.cpp.obj: CMakeFiles/RayTracing2.dir/flags.make
CMakeFiles/RayTracing2.dir/Vector.cpp.obj: ../Vector.cpp
CMakeFiles/RayTracing2.dir/Vector.cpp.obj: CMakeFiles/RayTracing2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RayTracing2.dir/Vector.cpp.obj"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing2.dir/Vector.cpp.obj -MF CMakeFiles\RayTracing2.dir\Vector.cpp.obj.d -o CMakeFiles\RayTracing2.dir\Vector.cpp.obj -c H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Vector.cpp

CMakeFiles/RayTracing2.dir/Vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing2.dir/Vector.cpp.i"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Vector.cpp > CMakeFiles\RayTracing2.dir\Vector.cpp.i

CMakeFiles/RayTracing2.dir/Vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing2.dir/Vector.cpp.s"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Vector.cpp -o CMakeFiles\RayTracing2.dir\Vector.cpp.s

CMakeFiles/RayTracing2.dir/Scene.cpp.obj: CMakeFiles/RayTracing2.dir/flags.make
CMakeFiles/RayTracing2.dir/Scene.cpp.obj: ../Scene.cpp
CMakeFiles/RayTracing2.dir/Scene.cpp.obj: CMakeFiles/RayTracing2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RayTracing2.dir/Scene.cpp.obj"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing2.dir/Scene.cpp.obj -MF CMakeFiles\RayTracing2.dir\Scene.cpp.obj.d -o CMakeFiles\RayTracing2.dir\Scene.cpp.obj -c H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Scene.cpp

CMakeFiles/RayTracing2.dir/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing2.dir/Scene.cpp.i"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Scene.cpp > CMakeFiles\RayTracing2.dir\Scene.cpp.i

CMakeFiles/RayTracing2.dir/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing2.dir/Scene.cpp.s"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Scene.cpp -o CMakeFiles\RayTracing2.dir\Scene.cpp.s

CMakeFiles/RayTracing2.dir/BVH.cpp.obj: CMakeFiles/RayTracing2.dir/flags.make
CMakeFiles/RayTracing2.dir/BVH.cpp.obj: ../BVH.cpp
CMakeFiles/RayTracing2.dir/BVH.cpp.obj: CMakeFiles/RayTracing2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RayTracing2.dir/BVH.cpp.obj"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing2.dir/BVH.cpp.obj -MF CMakeFiles\RayTracing2.dir\BVH.cpp.obj.d -o CMakeFiles\RayTracing2.dir\BVH.cpp.obj -c H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\BVH.cpp

CMakeFiles/RayTracing2.dir/BVH.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing2.dir/BVH.cpp.i"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\BVH.cpp > CMakeFiles\RayTracing2.dir\BVH.cpp.i

CMakeFiles/RayTracing2.dir/BVH.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing2.dir/BVH.cpp.s"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\BVH.cpp -o CMakeFiles\RayTracing2.dir\BVH.cpp.s

CMakeFiles/RayTracing2.dir/Renderer.cpp.obj: CMakeFiles/RayTracing2.dir/flags.make
CMakeFiles/RayTracing2.dir/Renderer.cpp.obj: ../Renderer.cpp
CMakeFiles/RayTracing2.dir/Renderer.cpp.obj: CMakeFiles/RayTracing2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RayTracing2.dir/Renderer.cpp.obj"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing2.dir/Renderer.cpp.obj -MF CMakeFiles\RayTracing2.dir\Renderer.cpp.obj.d -o CMakeFiles\RayTracing2.dir\Renderer.cpp.obj -c H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Renderer.cpp

CMakeFiles/RayTracing2.dir/Renderer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RayTracing2.dir/Renderer.cpp.i"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Renderer.cpp > CMakeFiles\RayTracing2.dir\Renderer.cpp.i

CMakeFiles/RayTracing2.dir/Renderer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RayTracing2.dir/Renderer.cpp.s"
	"C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\Renderer.cpp -o CMakeFiles\RayTracing2.dir\Renderer.cpp.s

# Object files for target RayTracing2
RayTracing2_OBJECTS = \
"CMakeFiles/RayTracing2.dir/main.cpp.obj" \
"CMakeFiles/RayTracing2.dir/Vector.cpp.obj" \
"CMakeFiles/RayTracing2.dir/Scene.cpp.obj" \
"CMakeFiles/RayTracing2.dir/BVH.cpp.obj" \
"CMakeFiles/RayTracing2.dir/Renderer.cpp.obj"

# External object files for target RayTracing2
RayTracing2_EXTERNAL_OBJECTS =

RayTracing2.exe: CMakeFiles/RayTracing2.dir/main.cpp.obj
RayTracing2.exe: CMakeFiles/RayTracing2.dir/Vector.cpp.obj
RayTracing2.exe: CMakeFiles/RayTracing2.dir/Scene.cpp.obj
RayTracing2.exe: CMakeFiles/RayTracing2.dir/BVH.cpp.obj
RayTracing2.exe: CMakeFiles/RayTracing2.dir/Renderer.cpp.obj
RayTracing2.exe: CMakeFiles/RayTracing2.dir/build.make
RayTracing2.exe: CMakeFiles/RayTracing2.dir/linklibs.rsp
RayTracing2.exe: CMakeFiles/RayTracing2.dir/objects1.rsp
RayTracing2.exe: CMakeFiles/RayTracing2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable RayTracing2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\RayTracing2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RayTracing2.dir/build: RayTracing2.exe
.PHONY : CMakeFiles/RayTracing2.dir/build

CMakeFiles/RayTracing2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\RayTracing2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/RayTracing2.dir/clean

CMakeFiles/RayTracing2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7 H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7 H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build H:\Graphics\101\GAMES101-Computer-Graphics-Assignment\Assignment7-Windows\Assignment7\build\CMakeFiles\RayTracing2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RayTracing2.dir/depend


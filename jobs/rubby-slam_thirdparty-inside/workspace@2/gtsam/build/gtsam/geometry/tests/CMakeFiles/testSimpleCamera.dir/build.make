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
CMAKE_SOURCE_DIR = /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build

# Include any dependencies generated for this target.
include gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/depend.make

# Include the progress variables for this target.
include gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/progress.make

# Include the compile flags for this target's objects.
include gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/flags.make

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/flags.make
gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o: ../gtsam/geometry/tests/testSimpleCamera.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests && /usr/bin/c++  $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/geometry/tests/testSimpleCamera.cpp

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.i"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/geometry/tests/testSimpleCamera.cpp > CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.i

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.s"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/geometry/tests/testSimpleCamera.cpp -o CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.s

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.requires:

.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.requires

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.provides: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.requires
	$(MAKE) -f gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/build.make gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.provides.build
.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.provides

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.provides.build: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o


# Object files for target testSimpleCamera
testSimpleCamera_OBJECTS = \
"CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o"

# External object files for target testSimpleCamera
testSimpleCamera_EXTERNAL_OBJECTS =

gtsam/geometry/tests/testSimpleCamera: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o
gtsam/geometry/tests/testSimpleCamera: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/build.make
gtsam/geometry/tests/testSimpleCamera: CppUnitLite/libCppUnitLite.a
gtsam/geometry/tests/testSimpleCamera: gtsam/libgtsam.so.4.1.0
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_thread.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_regex.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_timer.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libboost_system.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libtbb.so
gtsam/geometry/tests/testSimpleCamera: /usr/lib/aarch64-linux-gnu/libtbbmalloc.so
gtsam/geometry/tests/testSimpleCamera: gtsam/3rdparty/metis/libmetis/libmetis-gtsam.so
gtsam/geometry/tests/testSimpleCamera: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testSimpleCamera"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testSimpleCamera.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/build: gtsam/geometry/tests/testSimpleCamera

.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/build

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/requires: gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/testSimpleCamera.cpp.o.requires

.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/requires

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests && $(CMAKE_COMMAND) -P CMakeFiles/testSimpleCamera.dir/cmake_clean.cmake
.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/clean

gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/geometry/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gtsam/geometry/tests/CMakeFiles/testSimpleCamera.dir/depend


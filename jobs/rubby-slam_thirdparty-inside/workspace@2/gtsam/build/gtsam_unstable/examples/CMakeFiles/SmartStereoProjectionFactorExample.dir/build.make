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
include gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/depend.make

# Include the progress variables for this target.
include gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/progress.make

# Include the compile flags for this target's objects.
include gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/flags.make

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/flags.make
gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o: ../gtsam_unstable/examples/SmartStereoProjectionFactorExample.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples && /usr/bin/c++  $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/examples/SmartStereoProjectionFactorExample.cpp

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.i"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/examples/SmartStereoProjectionFactorExample.cpp > CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.i

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.s"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/examples/SmartStereoProjectionFactorExample.cpp -o CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.s

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.requires:

.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.requires

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.provides: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.requires
	$(MAKE) -f gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/build.make gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.provides.build
.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.provides

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.provides.build: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o


# Object files for target SmartStereoProjectionFactorExample
SmartStereoProjectionFactorExample_OBJECTS = \
"CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o"

# External object files for target SmartStereoProjectionFactorExample
SmartStereoProjectionFactorExample_EXTERNAL_OBJECTS =

gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o
gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/build.make
gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam_unstable/libgtsam_unstable.so.4.1.0
gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam/libgtsam.so.4.1.0
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_thread.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_regex.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_timer.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libboost_system.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libtbb.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: /usr/lib/aarch64-linux-gnu/libtbbmalloc.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam/3rdparty/metis/libmetis/libmetis-gtsam.so
gtsam_unstable/examples/SmartStereoProjectionFactorExample: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SmartStereoProjectionFactorExample"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SmartStereoProjectionFactorExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/build: gtsam_unstable/examples/SmartStereoProjectionFactorExample

.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/build

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/requires: gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/SmartStereoProjectionFactorExample.cpp.o.requires

.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/requires

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples && $(CMAKE_COMMAND) -P CMakeFiles/SmartStereoProjectionFactorExample.dir/cmake_clean.cmake
.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/clean

gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/examples /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gtsam_unstable/examples/CMakeFiles/SmartStereoProjectionFactorExample.dir/depend


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
CMAKE_SOURCE_DIR = /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build

# Include any dependencies generated for this target.
include CMakeFiles/RpgoReadG2oIncremental.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/RpgoReadG2oIncremental.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RpgoReadG2oIncremental.dir/flags.make

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o: CMakeFiles/RpgoReadG2oIncremental.dir/flags.make
CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o: ../examples/RpgoReadG2oIncremental.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/examples/RpgoReadG2oIncremental.cpp

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/examples/RpgoReadG2oIncremental.cpp > CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.i

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/examples/RpgoReadG2oIncremental.cpp -o CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.s

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.requires:

.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.requires

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.provides: CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.requires
	$(MAKE) -f CMakeFiles/RpgoReadG2oIncremental.dir/build.make CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.provides.build
.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.provides

CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.provides.build: CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o


# Object files for target RpgoReadG2oIncremental
RpgoReadG2oIncremental_OBJECTS = \
"CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o"

# External object files for target RpgoReadG2oIncremental
RpgoReadG2oIncremental_EXTERNAL_OBJECTS =

RpgoReadG2oIncremental: CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o
RpgoReadG2oIncremental: CMakeFiles/RpgoReadG2oIncremental.dir/build.make
RpgoReadG2oIncremental: libKimeraRPGO.so
RpgoReadG2oIncremental: /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/lib/lib/libgtsam_unstable.so.4.1.0
RpgoReadG2oIncremental: /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/lib/lib/libgtsam.so.4.1.0
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_thread.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_regex.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_timer.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
RpgoReadG2oIncremental: /usr/lib/aarch64-linux-gnu/libboost_system.so
RpgoReadG2oIncremental: /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/lib/lib/libmetis-gtsam.so
RpgoReadG2oIncremental: CMakeFiles/RpgoReadG2oIncremental.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable RpgoReadG2oIncremental"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RpgoReadG2oIncremental.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RpgoReadG2oIncremental.dir/build: RpgoReadG2oIncremental

.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/build

CMakeFiles/RpgoReadG2oIncremental.dir/requires: CMakeFiles/RpgoReadG2oIncremental.dir/examples/RpgoReadG2oIncremental.cpp.o.requires

.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/requires

CMakeFiles/RpgoReadG2oIncremental.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/RpgoReadG2oIncremental.dir/cmake_clean.cmake
.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/clean

CMakeFiles/RpgoReadG2oIncremental.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/Kimera-RPGO/build/CMakeFiles/RpgoReadG2oIncremental.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/RpgoReadG2oIncremental.dir/depend


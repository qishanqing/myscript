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
include tests/CMakeFiles/testPreconditioner.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/testPreconditioner.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/testPreconditioner.dir/flags.make

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o: tests/CMakeFiles/testPreconditioner.dir/flags.make
tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o: ../tests/testPreconditioner.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests && /usr/bin/c++  $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/tests/testPreconditioner.cpp

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.i"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/tests/testPreconditioner.cpp > CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.i

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.s"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/tests/testPreconditioner.cpp -o CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.s

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.requires:

.PHONY : tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.requires

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.provides: tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.requires
	$(MAKE) -f tests/CMakeFiles/testPreconditioner.dir/build.make tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.provides.build
.PHONY : tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.provides

tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.provides.build: tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o


# Object files for target testPreconditioner
testPreconditioner_OBJECTS = \
"CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o"

# External object files for target testPreconditioner
testPreconditioner_EXTERNAL_OBJECTS =

tests/testPreconditioner: tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o
tests/testPreconditioner: tests/CMakeFiles/testPreconditioner.dir/build.make
tests/testPreconditioner: CppUnitLite/libCppUnitLite.a
tests/testPreconditioner: gtsam/libgtsam.so.4.1.0
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_thread.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_regex.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_timer.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libboost_system.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libtbb.so
tests/testPreconditioner: /usr/lib/aarch64-linux-gnu/libtbbmalloc.so
tests/testPreconditioner: gtsam/3rdparty/metis/libmetis/libmetis-gtsam.so
tests/testPreconditioner: tests/CMakeFiles/testPreconditioner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testPreconditioner"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testPreconditioner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/testPreconditioner.dir/build: tests/testPreconditioner

.PHONY : tests/CMakeFiles/testPreconditioner.dir/build

tests/CMakeFiles/testPreconditioner.dir/requires: tests/CMakeFiles/testPreconditioner.dir/testPreconditioner.cpp.o.requires

.PHONY : tests/CMakeFiles/testPreconditioner.dir/requires

tests/CMakeFiles/testPreconditioner.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/testPreconditioner.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/testPreconditioner.dir/clean

tests/CMakeFiles/testPreconditioner.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/tests/CMakeFiles/testPreconditioner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/testPreconditioner.dir/depend


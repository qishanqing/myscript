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
include gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/depend.make

# Include the progress variables for this target.
include gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/progress.make

# Include the compile flags for this target's objects.
include gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/flags.make

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/flags.make
gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o: ../gtsam_unstable/discrete/tests/testLoopyBelief.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests && /usr/bin/c++  $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/discrete/tests/testLoopyBelief.cpp

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.i"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/discrete/tests/testLoopyBelief.cpp > CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.i

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.s"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/discrete/tests/testLoopyBelief.cpp -o CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.s

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.requires:

.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.requires

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.provides: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.requires
	$(MAKE) -f gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/build.make gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.provides.build
.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.provides

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.provides.build: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o


# Object files for target testLoopyBelief
testLoopyBelief_OBJECTS = \
"CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o"

# External object files for target testLoopyBelief
testLoopyBelief_EXTERNAL_OBJECTS =

gtsam_unstable/discrete/tests/testLoopyBelief: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o
gtsam_unstable/discrete/tests/testLoopyBelief: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/build.make
gtsam_unstable/discrete/tests/testLoopyBelief: CppUnitLite/libCppUnitLite.a
gtsam_unstable/discrete/tests/testLoopyBelief: gtsam_unstable/libgtsam_unstable.so.4.1.0
gtsam_unstable/discrete/tests/testLoopyBelief: gtsam/libgtsam.so.4.1.0
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_thread.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_regex.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_timer.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libboost_system.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libtbb.so
gtsam_unstable/discrete/tests/testLoopyBelief: /usr/lib/aarch64-linux-gnu/libtbbmalloc.so
gtsam_unstable/discrete/tests/testLoopyBelief: gtsam/3rdparty/metis/libmetis/libmetis-gtsam.so
gtsam_unstable/discrete/tests/testLoopyBelief: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testLoopyBelief"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testLoopyBelief.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/build: gtsam_unstable/discrete/tests/testLoopyBelief

.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/build

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/requires: gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/testLoopyBelief.cpp.o.requires

.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/requires

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests && $(CMAKE_COMMAND) -P CMakeFiles/testLoopyBelief.dir/cmake_clean.cmake
.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/clean

gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam_unstable/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gtsam_unstable/discrete/tests/CMakeFiles/testLoopyBelief.dir/depend


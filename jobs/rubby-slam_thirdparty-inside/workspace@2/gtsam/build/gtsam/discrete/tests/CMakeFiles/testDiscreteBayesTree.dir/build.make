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
include gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/depend.make

# Include the progress variables for this target.
include gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/progress.make

# Include the compile flags for this target's objects.
include gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/flags.make

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/flags.make
gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o: ../gtsam/discrete/tests/testDiscreteBayesTree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && /usr/bin/c++  $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o -c /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/discrete/tests/testDiscreteBayesTree.cpp

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.i"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/discrete/tests/testDiscreteBayesTree.cpp > CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.i

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.s"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && /usr/bin/c++ $(CXX_DEFINES) -DTOPSRCDIR=\"/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/discrete/tests/testDiscreteBayesTree.cpp -o CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.s

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.requires:

.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.requires

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.provides: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.requires
	$(MAKE) -f gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/build.make gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.provides.build
.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.provides

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.provides.build: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o


# Object files for target testDiscreteBayesTree
testDiscreteBayesTree_OBJECTS = \
"CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o"

# External object files for target testDiscreteBayesTree
testDiscreteBayesTree_EXTERNAL_OBJECTS =

gtsam/discrete/tests/testDiscreteBayesTree: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o
gtsam/discrete/tests/testDiscreteBayesTree: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/build.make
gtsam/discrete/tests/testDiscreteBayesTree: CppUnitLite/libCppUnitLite.a
gtsam/discrete/tests/testDiscreteBayesTree: gtsam/libgtsam.so.4.1.0
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_thread.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_regex.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_timer.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libboost_system.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libtbb.so
gtsam/discrete/tests/testDiscreteBayesTree: /usr/lib/aarch64-linux-gnu/libtbbmalloc.so
gtsam/discrete/tests/testDiscreteBayesTree: gtsam/3rdparty/metis/libmetis/libmetis-gtsam.so
gtsam/discrete/tests/testDiscreteBayesTree: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable testDiscreteBayesTree"
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testDiscreteBayesTree.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/build: gtsam/discrete/tests/testDiscreteBayesTree

.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/build

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/requires: gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/testDiscreteBayesTree.cpp.o.requires

.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/requires

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && $(CMAKE_COMMAND) -P CMakeFiles/testDiscreteBayesTree.dir/cmake_clean.cmake
.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/clean

gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteBayesTree.dir/depend


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

# Utility rule file for testDiscreteMarginals.valgrind.

# Include the progress variables for this target.
include gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/progress.make

gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind: gtsam/discrete/tests/testDiscreteMarginals
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && valgrind --error-exitcode=1 /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests/testDiscreteMarginals

testDiscreteMarginals.valgrind: gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind
testDiscreteMarginals.valgrind: gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/build.make

.PHONY : testDiscreteMarginals.valgrind

# Rule to build all files generated by this target.
gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/build: testDiscreteMarginals.valgrind

.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/build

gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/clean:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests && $(CMAKE_COMMAND) -P CMakeFiles/testDiscreteMarginals.valgrind.dir/cmake_clean.cmake
.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/clean

gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/depend:
	cd /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/gtsam/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests /home/jenkins/jenkins_home/jobs/rubby-slam_thirdparty-inside/workspace@2/gtsam/build/gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gtsam/discrete/tests/CMakeFiles/testDiscreteMarginals.valgrind.dir/depend


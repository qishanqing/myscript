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
CMAKE_SOURCE_DIR = /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build

# Include any dependencies generated for this target.
include plugins/system/test/CMakeFiles/test_sys_plugin.dir/depend.make

# Include the progress variables for this target.
include plugins/system/test/CMakeFiles/test_sys_plugin.dir/progress.make

# Include the compile flags for this target's objects.
include plugins/system/test/CMakeFiles/test_sys_plugin.dir/flags.make

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o: plugins/system/test/CMakeFiles/test_sys_plugin.dir/flags.make
plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o: ../plugins/system/test/src/SystemNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o -c /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/SystemNode.cpp

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.i"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/SystemNode.cpp > CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.i

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.s"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/SystemNode.cpp -o CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.s

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.requires:

.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.requires

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.provides: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.requires
	$(MAKE) -f plugins/system/test/CMakeFiles/test_sys_plugin.dir/build.make plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.provides.build
.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.provides

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.provides.build: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o


plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o: plugins/system/test/CMakeFiles/test_sys_plugin.dir/flags.make
plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o: ../plugins/system/test/src/log/LogManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o -c /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/log/LogManager.cpp

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.i"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/log/LogManager.cpp > CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.i

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.s"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test/src/log/LogManager.cpp -o CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.s

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.requires:

.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.requires

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.provides: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.requires
	$(MAKE) -f plugins/system/test/CMakeFiles/test_sys_plugin.dir/build.make plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.provides.build
.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.provides

plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.provides.build: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o


# Object files for target test_sys_plugin
test_sys_plugin_OBJECTS = \
"CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o" \
"CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o"

# External object files for target test_sys_plugin
test_sys_plugin_EXTERNAL_OBJECTS =

../lib/arm/libtest_sys_plugin.so: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o
../lib/arm/libtest_sys_plugin.so: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o
../lib/arm/libtest_sys_plugin.so: plugins/system/test/CMakeFiles/test_sys_plugin.dir/build.make
../lib/arm/libtest_sys_plugin.so: plugins/system/test/CMakeFiles/test_sys_plugin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../../../lib/arm/libtest_sys_plugin.so"
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_sys_plugin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
plugins/system/test/CMakeFiles/test_sys_plugin.dir/build: ../lib/arm/libtest_sys_plugin.so

.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/build

plugins/system/test/CMakeFiles/test_sys_plugin.dir/requires: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/SystemNode.cpp.o.requires
plugins/system/test/CMakeFiles/test_sys_plugin.dir/requires: plugins/system/test/CMakeFiles/test_sys_plugin.dir/src/log/LogManager.cpp.o.requires

.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/requires

plugins/system/test/CMakeFiles/test_sys_plugin.dir/clean:
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test && $(CMAKE_COMMAND) -P CMakeFiles/test_sys_plugin.dir/cmake_clean.cmake
.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/clean

plugins/system/test/CMakeFiles/test_sys_plugin.dir/depend:
	cd /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/plugins/system/test /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test/CMakeFiles/test_sys_plugin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : plugins/system/test/CMakeFiles/test_sys_plugin.dir/depend


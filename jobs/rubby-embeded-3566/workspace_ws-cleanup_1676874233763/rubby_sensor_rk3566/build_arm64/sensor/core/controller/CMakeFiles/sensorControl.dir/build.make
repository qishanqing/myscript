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
CMAKE_SOURCE_DIR = /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64

# Include any dependencies generated for this target.
include sensor/core/controller/CMakeFiles/sensorControl.dir/depend.make

# Include the progress variables for this target.
include sensor/core/controller/CMakeFiles/sensorControl.dir/progress.make

# Include the compile flags for this target's objects.
include sensor/core/controller/CMakeFiles/sensorControl.dir/flags.make

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o: sensor/core/controller/CMakeFiles/sensorControl.dir/flags.make
sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o: ../sensor/core/controller/Controller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sensorControl.dir/Controller.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/controller/Controller.cpp

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sensorControl.dir/Controller.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/controller/Controller.cpp > CMakeFiles/sensorControl.dir/Controller.cpp.i

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sensorControl.dir/Controller.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/controller/Controller.cpp -o CMakeFiles/sensorControl.dir/Controller.cpp.s

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.requires:

.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.requires

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.provides: sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.requires
	$(MAKE) -f sensor/core/controller/CMakeFiles/sensorControl.dir/build.make sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.provides.build
.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.provides

sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.provides.build: sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o


# Object files for target sensorControl
sensorControl_OBJECTS = \
"CMakeFiles/sensorControl.dir/Controller.cpp.o"

# External object files for target sensorControl
sensorControl_EXTERNAL_OBJECTS =

../install/lib/arm/libsensorControl.so.1.0.2: sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o
../install/lib/arm/libsensorControl.so.1.0.2: sensor/core/controller/CMakeFiles/sensorControl.dir/build.make
../install/lib/arm/libsensorControl.so.1.0.2: ../mind_os/packs/v1.4.2/3566/mind_os/lib/libmind_os.so
../install/lib/arm/libsensorControl.so.1.0.2: ../install/lib/arm/libemb_common.so.1.0.2
../install/lib/arm/libsensorControl.so.1.0.2: ../mind_os/packs/v1.4.2/3566/mind_os/lib/libmind_os.so
../install/lib/arm/libsensorControl.so.1.0.2: sensor/core/controller/CMakeFiles/sensorControl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../../install/lib/arm/libsensorControl.so"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && rm -f /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/install/lib/arm/libsensorControl.so*
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sensorControl.dir/link.txt --verbose=$(VERBOSE)
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && $(CMAKE_COMMAND) -E cmake_symlink_library ../../../../install/lib/arm/libsensorControl.so.1.0.2 ../../../../install/lib/arm/libsensorControl.so.1.0.2 ../../../../install/lib/arm/libsensorControl.so

../install/lib/arm/libsensorControl.so: ../install/lib/arm/libsensorControl.so.1.0.2
	@$(CMAKE_COMMAND) -E touch_nocreate ../install/lib/arm/libsensorControl.so

# Rule to build all files generated by this target.
sensor/core/controller/CMakeFiles/sensorControl.dir/build: ../install/lib/arm/libsensorControl.so

.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/build

sensor/core/controller/CMakeFiles/sensorControl.dir/requires: sensor/core/controller/CMakeFiles/sensorControl.dir/Controller.cpp.o.requires

.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/requires

sensor/core/controller/CMakeFiles/sensorControl.dir/clean:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller && $(CMAKE_COMMAND) -P CMakeFiles/sensorControl.dir/cmake_clean.cmake
.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/clean

sensor/core/controller/CMakeFiles/sensorControl.dir/depend:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/controller /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/controller/CMakeFiles/sensorControl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensor/core/controller/CMakeFiles/sensorControl.dir/depend


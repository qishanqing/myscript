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
include sensor/core/keyboard/CMakeFiles/KeyBoard.dir/depend.make

# Include the progress variables for this target.
include sensor/core/keyboard/CMakeFiles/KeyBoard.dir/progress.make

# Include the compile flags for this target's objects.
include sensor/core/keyboard/CMakeFiles/KeyBoard.dir/flags.make

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/flags.make
sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o: ../sensor/core/keyboard/KeyBoard.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/keyboard/KeyBoard.cpp

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KeyBoard.dir/KeyBoard.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/keyboard/KeyBoard.cpp > CMakeFiles/KeyBoard.dir/KeyBoard.cpp.i

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KeyBoard.dir/KeyBoard.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/keyboard/KeyBoard.cpp -o CMakeFiles/KeyBoard.dir/KeyBoard.cpp.s

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.requires:

.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.requires

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.provides: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.requires
	$(MAKE) -f sensor/core/keyboard/CMakeFiles/KeyBoard.dir/build.make sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.provides.build
.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.provides

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.provides.build: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o


sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/flags.make
sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o: ../sensor/core/drivers/serial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp > CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.i

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp -o CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.s

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.requires:

.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.requires

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.provides: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.requires
	$(MAKE) -f sensor/core/keyboard/CMakeFiles/KeyBoard.dir/build.make sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.provides.build
.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.provides

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.provides.build: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o


# Object files for target KeyBoard
KeyBoard_OBJECTS = \
"CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o" \
"CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o"

# External object files for target KeyBoard
KeyBoard_EXTERNAL_OBJECTS =

../install/lib/arm/libKeyBoard.so.1.0.2: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o
../install/lib/arm/libKeyBoard.so.1.0.2: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o
../install/lib/arm/libKeyBoard.so.1.0.2: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/build.make
../install/lib/arm/libKeyBoard.so.1.0.2: ../mind_os/packs/v1.4.2/3566/mind_os/lib/libmind_os.so
../install/lib/arm/libKeyBoard.so.1.0.2: ../install/lib/arm/libemb_common.so.1.0.2
../install/lib/arm/libKeyBoard.so.1.0.2: ../mind_os/packs/v1.4.2/3566/mind_os/lib/libmind_os.so
../install/lib/arm/libKeyBoard.so.1.0.2: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../../../install/lib/arm/libKeyBoard.so"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && rm -f /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/install/lib/arm/libKeyBoard.so*
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/KeyBoard.dir/link.txt --verbose=$(VERBOSE)
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && $(CMAKE_COMMAND) -E cmake_symlink_library ../../../../install/lib/arm/libKeyBoard.so.1.0.2 ../../../../install/lib/arm/libKeyBoard.so.1.0.2 ../../../../install/lib/arm/libKeyBoard.so

../install/lib/arm/libKeyBoard.so: ../install/lib/arm/libKeyBoard.so.1.0.2
	@$(CMAKE_COMMAND) -E touch_nocreate ../install/lib/arm/libKeyBoard.so

# Rule to build all files generated by this target.
sensor/core/keyboard/CMakeFiles/KeyBoard.dir/build: ../install/lib/arm/libKeyBoard.so

.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/build

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/requires: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/KeyBoard.cpp.o.requires
sensor/core/keyboard/CMakeFiles/KeyBoard.dir/requires: sensor/core/keyboard/CMakeFiles/KeyBoard.dir/__/drivers/serial.cpp.o.requires

.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/requires

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/clean:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard && $(CMAKE_COMMAND) -P CMakeFiles/KeyBoard.dir/cmake_clean.cmake
.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/clean

sensor/core/keyboard/CMakeFiles/KeyBoard.dir/depend:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/keyboard /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/keyboard/CMakeFiles/KeyBoard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensor/core/keyboard/CMakeFiles/KeyBoard.dir/depend


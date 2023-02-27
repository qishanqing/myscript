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
include sensor/core/mcudata/CMakeFiles/McuData.dir/depend.make

# Include the progress variables for this target.
include sensor/core/mcudata/CMakeFiles/McuData.dir/progress.make

# Include the compile flags for this target's objects.
include sensor/core/mcudata/CMakeFiles/McuData.dir/flags.make

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o: sensor/core/mcudata/CMakeFiles/McuData.dir/flags.make
sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o: ../sensor/core/mcudata/McuData.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/McuData.dir/McuData.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuData.cpp

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McuData.dir/McuData.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuData.cpp > CMakeFiles/McuData.dir/McuData.cpp.i

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McuData.dir/McuData.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuData.cpp -o CMakeFiles/McuData.dir/McuData.cpp.s

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.requires:

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.requires

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.provides: sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.requires
	$(MAKE) -f sensor/core/mcudata/CMakeFiles/McuData.dir/build.make sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.provides.build
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.provides

sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.provides.build: sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o


sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o: sensor/core/mcudata/CMakeFiles/McuData.dir/flags.make
sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o: ../sensor/core/mcudata/McuDataDriver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/McuData.dir/McuDataDriver.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataDriver.cpp

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McuData.dir/McuDataDriver.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataDriver.cpp > CMakeFiles/McuData.dir/McuDataDriver.cpp.i

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McuData.dir/McuDataDriver.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataDriver.cpp -o CMakeFiles/McuData.dir/McuDataDriver.cpp.s

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.requires:

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.requires

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.provides: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.requires
	$(MAKE) -f sensor/core/mcudata/CMakeFiles/McuData.dir/build.make sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.provides.build
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.provides

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.provides.build: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o


sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o: sensor/core/mcudata/CMakeFiles/McuData.dir/flags.make
sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o: ../sensor/core/mcudata/McuDataProtocol.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/McuData.dir/McuDataProtocol.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataProtocol.cpp

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McuData.dir/McuDataProtocol.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataProtocol.cpp > CMakeFiles/McuData.dir/McuDataProtocol.cpp.i

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McuData.dir/McuDataProtocol.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata/McuDataProtocol.cpp -o CMakeFiles/McuData.dir/McuDataProtocol.cpp.s

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.requires:

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.requires

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.provides: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.requires
	$(MAKE) -f sensor/core/mcudata/CMakeFiles/McuData.dir/build.make sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.provides.build
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.provides

sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.provides.build: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o


sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o: sensor/core/mcudata/CMakeFiles/McuData.dir/flags.make
sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o: ../sensor/core/drivers/serial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/McuData.dir/__/drivers/serial.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp

sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McuData.dir/__/drivers/serial.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp > CMakeFiles/McuData.dir/__/drivers/serial.cpp.i

sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McuData.dir/__/drivers/serial.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/drivers/serial.cpp -o CMakeFiles/McuData.dir/__/drivers/serial.cpp.s

sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.requires:

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.requires

sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.provides: sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.requires
	$(MAKE) -f sensor/core/mcudata/CMakeFiles/McuData.dir/build.make sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.provides.build
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.provides

sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.provides.build: sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o


# Object files for target McuData
McuData_OBJECTS = \
"CMakeFiles/McuData.dir/McuData.cpp.o" \
"CMakeFiles/McuData.dir/McuDataDriver.cpp.o" \
"CMakeFiles/McuData.dir/McuDataProtocol.cpp.o" \
"CMakeFiles/McuData.dir/__/drivers/serial.cpp.o"

# External object files for target McuData
McuData_EXTERNAL_OBJECTS =

../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o
../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o
../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o
../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o
../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/build.make
../install/lib/arm/libMcuData.so.1.0.2: ../install/lib/arm/libmonitor_node.so
../install/lib/arm/libMcuData.so.1.0.2: ../install/lib/arm/libKeyBoard.so.1.0.2
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_aruco.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_bgsegm.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_bioinspired.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_ccalib.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_freetype.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_fuzzy.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_hfs.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_img_hash.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_line_descriptor.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_optflow.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_reg.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_rgbd.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_saliency.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_sfm.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_stereo.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_structured_light.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_phase_unwrapping.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_surface_matching.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_tracking.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_video.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_plot.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_xfeatures2d.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_ximgproc.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_calib3d.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_features2d.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_flann.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_highgui.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_videoio.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_imgcodecs.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_xphoto.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_imgproc.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: /usr/local/lib/libopencv_core.so.3.4.3
../install/lib/arm/libMcuData.so.1.0.2: ../install/lib/arm/libemb_common.so.1.0.2
../install/lib/arm/libMcuData.so.1.0.2: sensor/core/mcudata/CMakeFiles/McuData.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ../../../../install/lib/arm/libMcuData.so"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && rm -f /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/install/lib/arm/libMcuData.so*
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/McuData.dir/link.txt --verbose=$(VERBOSE)
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && $(CMAKE_COMMAND) -E cmake_symlink_library ../../../../install/lib/arm/libMcuData.so.1.0.2 ../../../../install/lib/arm/libMcuData.so.1.0.2 ../../../../install/lib/arm/libMcuData.so

../install/lib/arm/libMcuData.so: ../install/lib/arm/libMcuData.so.1.0.2
	@$(CMAKE_COMMAND) -E touch_nocreate ../install/lib/arm/libMcuData.so

# Rule to build all files generated by this target.
sensor/core/mcudata/CMakeFiles/McuData.dir/build: ../install/lib/arm/libMcuData.so

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/build

sensor/core/mcudata/CMakeFiles/McuData.dir/requires: sensor/core/mcudata/CMakeFiles/McuData.dir/McuData.cpp.o.requires
sensor/core/mcudata/CMakeFiles/McuData.dir/requires: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataDriver.cpp.o.requires
sensor/core/mcudata/CMakeFiles/McuData.dir/requires: sensor/core/mcudata/CMakeFiles/McuData.dir/McuDataProtocol.cpp.o.requires
sensor/core/mcudata/CMakeFiles/McuData.dir/requires: sensor/core/mcudata/CMakeFiles/McuData.dir/__/drivers/serial.cpp.o.requires

.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/requires

sensor/core/mcudata/CMakeFiles/McuData.dir/clean:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata && $(CMAKE_COMMAND) -P CMakeFiles/McuData.dir/cmake_clean.cmake
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/clean

sensor/core/mcudata/CMakeFiles/McuData.dir/depend:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/mcudata /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/mcudata/CMakeFiles/McuData.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensor/core/mcudata/CMakeFiles/McuData.dir/depend


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
include sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/depend.make

# Include the progress variables for this target.
include sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/progress.make

# Include the compile flags for this target's objects.
include sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/flags.make

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/flags.make
sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o: ../sensor/core/multi_module/src/MultiModule.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/src/MultiModule.cpp

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/src/MultiModule.cpp > CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.i

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/src/MultiModule.cpp -o CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.s

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.requires:

.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.requires

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.provides: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.requires
	$(MAKE) -f sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/build.make sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.provides.build
.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.provides

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.provides.build: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o


sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/flags.make
sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o: ../sensor/core/multi_module/sample/multi_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/multi_main.dir/multi_main.cpp.o -c /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/sample/multi_main.cpp

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/multi_main.dir/multi_main.cpp.i"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/sample/multi_main.cpp > CMakeFiles/multi_main.dir/multi_main.cpp.i

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/multi_main.dir/multi_main.cpp.s"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/sample/multi_main.cpp -o CMakeFiles/multi_main.dir/multi_main.cpp.s

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.requires:

.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.requires

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.provides: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.requires
	$(MAKE) -f sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/build.make sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.provides.build
.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.provides

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.provides.build: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o


# Object files for target multi_main
multi_main_OBJECTS = \
"CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o" \
"CMakeFiles/multi_main.dir/multi_main.cpp.o"

# External object files for target multi_main
multi_main_EXTERNAL_OBJECTS =

../install/bin/arm/multi_main: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o
../install/bin/arm/multi_main: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o
../install/bin/arm/multi_main: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/build.make
../install/bin/arm/multi_main: ../install/lib/arm/libmonitor_node.so
../install/bin/arm/multi_main: ../install/lib/arm/libemb_common.so.1.0.2
../install/bin/arm/multi_main: /usr/local/lib/libopencv_aruco.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_bgsegm.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_bioinspired.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_ccalib.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_freetype.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_fuzzy.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_hfs.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_img_hash.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_line_descriptor.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_optflow.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_reg.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_rgbd.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_saliency.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_sfm.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_stereo.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_structured_light.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_phase_unwrapping.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_surface_matching.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_tracking.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_video.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_plot.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_xfeatures2d.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_ximgproc.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_calib3d.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_features2d.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_flann.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_highgui.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_videoio.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_imgcodecs.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_xphoto.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_imgproc.so.3.4.3
../install/bin/arm/multi_main: /usr/local/lib/libopencv_core.so.3.4.3
../install/bin/arm/multi_main: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../../../../install/bin/arm/multi_main"
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multi_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/build: ../install/bin/arm/multi_main

.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/build

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/requires: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/__/src/MultiModule.cpp.o.requires
sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/requires: sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/multi_main.cpp.o.requires

.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/requires

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/clean:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample && $(CMAKE_COMMAND) -P CMakeFiles/multi_main.dir/cmake_clean.cmake
.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/clean

sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/depend:
	cd /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/sensor/core/multi_module/sample /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64 /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample /var/jenkins_home/jobs/rubby-embeded-3566/workspace/rubby_sensor_rk3566/build_arm64/sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sensor/core/multi_module/sample/CMakeFiles/multi_main.dir/depend


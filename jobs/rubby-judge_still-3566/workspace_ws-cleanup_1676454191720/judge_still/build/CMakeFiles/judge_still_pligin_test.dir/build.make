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
CMAKE_SOURCE_DIR = /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build

# Include any dependencies generated for this target.
include CMakeFiles/judge_still_pligin_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/judge_still_pligin_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/judge_still_pligin_test.dir/flags.make

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o: CMakeFiles/judge_still_pligin_test.dir/flags.make
CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o: ../sample/judge_still_pligin_test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o -c /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/sample/judge_still_pligin_test.cc

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/sample/judge_still_pligin_test.cc > CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.i

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/sample/judge_still_pligin_test.cc -o CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.s

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.requires:

.PHONY : CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.requires

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.provides: CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.requires
	$(MAKE) -f CMakeFiles/judge_still_pligin_test.dir/build.make CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.provides.build
.PHONY : CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.provides

CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.provides.build: CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o


# Object files for target judge_still_pligin_test
judge_still_pligin_test_OBJECTS = \
"CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o"

# External object files for target judge_still_pligin_test
judge_still_pligin_test_EXTERNAL_OBJECTS =

../lib/libjudge_still_pligin_test.so: CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o
../lib/libjudge_still_pligin_test.so: CMakeFiles/judge_still_pligin_test.dir/build.make
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_timer.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_regex.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_serialization.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_thread.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_system.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_filesystem.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_chrono.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_date_time.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libboost_atomic.so
../lib/libjudge_still_pligin_test.so: /usr/lib/aarch64-linux-gnu/libpthread.so
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_aruco.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_bgsegm.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_bioinspired.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_ccalib.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_freetype.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_fuzzy.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_hfs.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_img_hash.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_line_descriptor.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_optflow.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_reg.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_rgbd.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_saliency.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_sfm.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_stereo.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_structured_light.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_surface_matching.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_tracking.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_xfeatures2d.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_ximgproc.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_xphoto.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_calib3d.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_features2d.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_highgui.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_videoio.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_imgcodecs.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_phase_unwrapping.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_flann.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_video.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_plot.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_imgproc.so.3.4.3
../lib/libjudge_still_pligin_test.so: /usr/local/lib/libopencv_core.so.3.4.3
../lib/libjudge_still_pligin_test.so: CMakeFiles/judge_still_pligin_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../lib/libjudge_still_pligin_test.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/judge_still_pligin_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/judge_still_pligin_test.dir/build: ../lib/libjudge_still_pligin_test.so

.PHONY : CMakeFiles/judge_still_pligin_test.dir/build

CMakeFiles/judge_still_pligin_test.dir/requires: CMakeFiles/judge_still_pligin_test.dir/sample/judge_still_pligin_test.cc.o.requires

.PHONY : CMakeFiles/judge_still_pligin_test.dir/requires

CMakeFiles/judge_still_pligin_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/judge_still_pligin_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/judge_still_pligin_test.dir/clean

CMakeFiles/judge_still_pligin_test.dir/depend:
	cd /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build /var/jenkins_home/jobs/rubby-judge_still-405/workspace/judge_still/build/CMakeFiles/judge_still_pligin_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/judge_still_pligin_test.dir/depend


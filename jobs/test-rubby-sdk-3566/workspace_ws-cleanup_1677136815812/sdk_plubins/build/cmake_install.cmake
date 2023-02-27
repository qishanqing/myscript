# Install script for directory: /var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/hmi/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/state/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/bag_player/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/bag_recorder/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/monitor_resource/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/monitor_topic/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/integration/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/system/test/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/remote_hmi/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/work_log/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/map/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/hmi_simulator/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/sensor_simulator/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/led_manager/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/senmatic_convert/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/task/cmake_install.cmake")
  include("/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/plugins/ros/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/var/jenkins_home/jobs/test-rubby-sdk-3566/workspace/sdk_plubins/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")

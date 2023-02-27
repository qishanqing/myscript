cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
)

find_package(catkin REQUIRED COMPONENTS geometry_msgs
    roscpp
    geometry_msgs
    std_msgs
    tf
    nav_msgs
    sensor_msgs
    std_srvs
    rospack
    roslib
)

include_directories(
    ${catkin_INCLUDE_DIRS}
)

add_library(ros_subscriber_plugin SHARED
    RosSubscriberNode.cpp
)

target_link_libraries(ros_subscriber_plugin
  pthread
  boost_system
  glog
  mind_os
  ${catkin_LIBRARIES}
)

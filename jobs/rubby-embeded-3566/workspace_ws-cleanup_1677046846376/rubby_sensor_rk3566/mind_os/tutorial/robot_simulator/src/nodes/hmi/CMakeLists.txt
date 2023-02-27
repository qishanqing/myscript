cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../util
    ../../../msgs/msg
    ../../../msgs/srv
)

add_library(hmi_plugin SHARED
    HMISimulator.cpp
)

target_link_libraries(hmi_plugin
    pthread
    boost_system
    glog
    mind_os
)

cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../util
    ../../../msgs/msg
    ../../../msgs/srv
)

add_library(depends SHARED
    depends/Depends.cpp
)

target_link_libraries(depends
    boost_system
    glog
)

add_library(slam_plugin SHARED
    SlamSimulator.cpp
)

target_link_libraries(slam_plugin
    pthread
    boost_system
    glog
    mind_os
    depends
)

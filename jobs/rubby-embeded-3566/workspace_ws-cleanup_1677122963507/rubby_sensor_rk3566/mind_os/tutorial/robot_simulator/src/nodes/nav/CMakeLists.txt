cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../util
    ../../../msgs/msg
    ../../../msgs/srv
)

add_library(nav_plugin SHARED
    NavigationSimulator.cpp
)

target_link_libraries(nav_plugin
    pthread
    boost_system
    glog
    mind_os
)

cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../types
)

add_library(application_plugin SHARED
    ApplicationNode.cpp
)

target_link_libraries(application_plugin
    pthread
    boost_system
    glog
    mind_os
)

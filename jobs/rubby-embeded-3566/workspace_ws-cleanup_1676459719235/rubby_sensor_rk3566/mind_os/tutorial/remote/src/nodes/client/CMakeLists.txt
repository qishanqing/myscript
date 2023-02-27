cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../../
)

add_library(client_plugin SHARED
    ClientNode.cpp
)

target_link_libraries(client_plugin
    pthread
    boost_system
    glog
    mind_os
)

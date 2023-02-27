cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../../
)

add_library(publisher_plugin SHARED
    PublisherNode.cpp
    A.cpp
    B.cpp
)

target_link_libraries(publisher_plugin
    pthread
    boost_system
    glog
    mind_os
)

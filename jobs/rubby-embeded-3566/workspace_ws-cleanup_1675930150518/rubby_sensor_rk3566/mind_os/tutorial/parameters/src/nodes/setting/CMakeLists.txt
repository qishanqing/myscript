cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

include_directories(
    ../../types
)

add_library(setting_plugin SHARED
    SettingNode.cpp
)

target_link_libraries(setting_plugin
    pthread
    boost_system
    glog
    mind_os
)

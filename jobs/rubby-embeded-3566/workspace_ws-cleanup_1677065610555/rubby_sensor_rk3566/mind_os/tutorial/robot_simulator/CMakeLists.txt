cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(src/nodes/system)
add_subdirectory(src/nodes/hmi)
add_subdirectory(src/nodes/nav)
add_subdirectory(src/nodes/sensor)
add_subdirectory(src/nodes/slam)

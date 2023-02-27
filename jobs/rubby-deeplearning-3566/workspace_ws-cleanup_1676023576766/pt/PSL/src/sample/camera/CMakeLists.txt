cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME camera_test)
set(LIBRARY_VERSION 1.0.0)
project(${PROJECT_NAME})
add_definitions(-std=c++11)
option(BUILD_CAMERA "image from camera" OFF)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(.)
include_directories(/usr/local/include/I18RPublicBaseTypes ${ROOT_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_FILES ${ROOT_DIR})
include(${ROOT_DIR}/cmake/find_lib_files.cmake)
list(APPEND SRC_DIR ${ROOT_DIR}/src/sample/camera)
include(${ROOT_DIR}/cmake/find_device.cmake)
include_directories(${SRC_FILES})

find_source_file(SRC_FILES "*.h" "*.cpp" "*.c" "*.cc" "*.hpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${LIBRARY_VERSION} SOVERSION 1)
target_link_libraries(${PROJECT_NAME} -lglog ${OpenCV_LIBS} -ldl)

# gpu/npu
target_link_libraries(${PROJECT_NAME} ${CNN_LIBRARY})

# yaml
include(${ROOT_DIR}/cmake/find_yaml.cmake)
include_directories(${YAML_CPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${YAMLCPP_LIBRARY})

# glog
target_link_libraries(${PROJECT_NAME} -lpthread -lglog)

#camera driver
file(GLOB DRIVER_LIB ${CAMERA_DIR}/lib/lib*.so)
include_directories(${CAMERA_DIR}/include)
target_link_libraries(${PROJECT_NAME} ${DRIVER_LIB})


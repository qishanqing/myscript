cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME unit_test)
set(LIBRARY_VERSION 1.0.2)
project(${PROJECT_NAME})
add_definitions(-std=c++11)
option(BUILD_TEST "build test" OFF)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(/usr/local/include/I18RPublicBaseTypes ${ROOT_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_FILES ${ROOT_DIR})
include(${ROOT_DIR}/cmake/find_lib_files.cmake)
list(APPEND SRC_DIR ${ROOT_DIR}/src/test)
list(APPEND SRC_DIR ${ROOT_DIR}/src/test/map)
list(APPEND SRC_DIR ${ROOT_DIR}/src/test/module)
list(APPEND SRC_DIR ${ROOT_DIR}/src/test/validate)
list(APPEND SRC_DIR ${ROOT_DIR}/src/test/instance)
include(${ROOT_DIR}/cmake/find_device.cmake)
include_directories(${SRC_FILES})

find_source_file(SRC_FILES "*.h" "*.cpp" "*.c" "*.cc" "*.hpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${LIBRARY_VERSION} SOVERSION 1)
target_link_libraries(${PROJECT_NAME} -lglog ${OpenCV_LIBS} -ldl)

# gpu/npu_aml
target_link_libraries(${PROJECT_NAME} ${CNN_LIBRARY})

# yaml
include(${ROOT_DIR}/cmake/find_yaml.cmake)
include_directories(${YAML_CPP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${YAMLCPP_LIBRARY})

# glog
target_link_libraries(${PROJECT_NAME} -lpthread -lglog)

# gtest
find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES})

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/)
execute_process(COMMAND cp -r ${ROOT_DIR}/map/bind.txt ${CMAKE_BINARY_DIR})

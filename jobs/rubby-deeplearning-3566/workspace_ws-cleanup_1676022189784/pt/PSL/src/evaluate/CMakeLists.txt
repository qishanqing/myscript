cmake_minimum_required(VERSION 3.0.0)
set(PROJECT_NAME evaluate)
set(LIBRARY_VERSION 1.0.0)
project(${PROJECT_NAME})
add_definitions(-std=c++11)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../)
include(${ROOT_DIR}/cmake/find_device.cmake)

# find_package(yaml-cpp REQUIRED) # not work and not break
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(/usr/local/include/I18RPublicBaseTypes ${ROOT_DIR}/include /usr/share/npu/sdk/include/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

function(find_source_file SRC_FILES arg)
    set(file_list "")
    message(STATUS "[*] searching file: ${arg};${ARGN}")

    list(LENGTH ARGV argv_len)
    set(i 1)
    while (i LESS ${argv_len})
        list(GET ARGV ${i} argv_value)
        foreach (file_path ${SRC_DIR})
            file(GLOB file ${file_path}/${argv_value})
            set(file_list ${file_list} ${file})
        endforeach ()

        math(EXPR i "${i} + 1")
    endwhile ()

    set(${SRC_FILES} ${file_list} PARENT_SCOPE)
endfunction()

set(SRC_FILES ${ROOT_DIR})
set(SRC_DIR
        ${ROOT_DIR}/sample/utils
        ${ROOT_DIR}/src
        ${ROOT_DIR}/src/utils
        ${ROOT_DIR}/src/module
        ${ROOT_DIR}/src/module/vnn
        ${ROOT_DIR}/src/data
        ${ROOT_DIR}/src/evaluate
        )
include_directories(${SRC_FILES})

find_source_file(SRC_FILES "*.h" "*.cpp" "*.c" "*.cc" "*.hpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION ${LIBRARY_VERSION} SOVERSION 1)
target_link_libraries(${PROJECT_NAME} -lglog ${OpenCV_LIBS} -ldl -lyaml-cpp ${NPU_LIBRARIES})

# glog
target_link_libraries(${PROJECT_NAME} -lpthread -lglog)

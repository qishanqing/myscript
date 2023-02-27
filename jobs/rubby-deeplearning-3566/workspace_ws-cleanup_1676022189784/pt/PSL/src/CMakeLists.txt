cmake_minimum_required(VERSION 3.0.0)
set(LIBRARY_NAME PSL)
set(LIBRARY_VERSION 1.5.6)
project(${LIBRARY_NAME})
add_definitions(-std=c++11)
option(BUILD_TEST "build test" OFF)
option(BUILD_EVAL "build evaluate" OFF)
option(NPU "inference with npu" "")
option(USE_GPU "inference with gpu" OFF)
option(BUILD_CAMERA "image from camera" OFF)

set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../)

message(STATUS ${CNN_LIBRARY})

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)
# find_package(glog REQUIRED NO_MODULE)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${ROOT_DIR}/include)
include_directories(${ROOT_DIR}/3rdparty)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC_FILES ${ROOT_DIR})
include(${ROOT_DIR}/cmake/find_lib_files.cmake)
list(APPEND SRC_DIR ${sub_dirs})

include(${ROOT_DIR}/cmake/find_device.cmake)
include_directories(${SRC_FILES})
find_source_file(SRC_FILES "*.h" "*.cpp" "*.c" "*.cc" "*.hpp")
add_library(${LIBRARY_NAME} SHARED ${SRC_FILES} utils/box.cpp utils/box.h)

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES VERSION ${LIBRARY_VERSION} SOVERSION 1)
target_link_libraries(${LIBRARY_NAME} ${OpenCV_LIBS} -ldl)

# gpu/npu_aml
target_link_libraries(${LIBRARY_NAME} ${CNN_LIBRARY})

# glog
target_link_libraries(${LIBRARY_NAME} -lpthread -lglog)

# yaml
include(${ROOT_DIR}/cmake/find_yaml.cmake)
include_directories(${YAML_CPP_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} ${YAMLCPP_LIBRARY})

# gtest
if(BUILD_TEST)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()

# evaluate
if(BUILD_EVAL)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/evaluate)
endif()

if(BUILD_CAMERA)
    add_subdirectory(sample/camera)
endif()

set(PROJECT_NAME_SYNC sample_sync)
file(GLOB file ${ROOT_DIR}/sample/utils/*)
add_executable(${PROJECT_NAME_SYNC} sample/main.cpp ${file} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME_SYNC} -lpthread -lglog)
target_link_libraries(${PROJECT_NAME_SYNC} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME_SYNC}  ${CNN_LIBRARY})
target_link_libraries(${PROJECT_NAME_SYNC} ${YAMLCPP_LIBRARY})
set_target_properties(${PROJECT_NAME_SYNC} PROPERTIES LINK_FLAGS -Wl,--no-as-needed)

add_definitions(-w)

# make install
if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(LIB_GENERAET "lib/x64")
elseif(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    set(LIB_GENERAET "lib/arm")
endif()

install(TARGETS ${LIBRARY_NAME}
        RUNTIME DESTINATION bin ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${LIB_GENERAET} ${CMAKE_INSTALL_LIBDIR}
#        PUBLIC_HEADER DESTINATION include ${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
        )
#set(INSTALL_HEAD_FILES ${LIBRARY_NAME}.h)
#install(FILES ${INSTALL_HEAD_FILES} DESTINATION  ${CMAKE_INSTALL_PREFIX}/include)
#install(FILES ${ROOT_DIR}/src/perception.h DESTINATION  ${CMAKE_INSTALL_PREFIX}/include)
#install(DIRECTORY ${ROOT_DIR}/include DESTINATION  ${CMAKE_INSTALL_PREFIX}/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_SYNC} DESTINATION  ../)

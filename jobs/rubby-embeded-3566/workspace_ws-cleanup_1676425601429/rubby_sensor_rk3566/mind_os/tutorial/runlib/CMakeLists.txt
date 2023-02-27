cmake_minimum_required(VERSION 3.0)

project(mind_os VERSION 1.0)

add_definitions(-std=c++17)

add_executable(example_join1
    src/ExampleJoin1.cpp
)

target_link_libraries(example_join1
    pthread
    boost_system
    mind_os
    glog
    yaml-cpp
)

add_executable(example_join2
    src/ExampleJoin2.cpp
)

target_link_libraries(example_join2
    pthread
    boost_system
    mind_os
    glog
    yaml-cpp
)

add_executable(example_detach1
    src/ExampleDetach1.cpp
)

target_link_libraries(example_detach1
    pthread
    boost_system
    mind_os
    glog
    yaml-cpp
)

add_executable(example_detach2
    src/ExampleDetach2.cpp
)

target_link_libraries(example_detach2
    pthread
    boost_system
    mind_os
    glog
    yaml-cpp
)

cmake_minimum_required(VERSION 3.10.2)
include_directories(4_reactive_and_asynchronous_behaviors)
project(reactive_and_asynchronous_behaviors)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(${PROJECT_NAME} "reactive_and_asynchronous_behaviors.cpp" "dummy_nodes.cpp")
target_link_libraries(${PROJECT_NAME} BT::behaviortree_cpp)
cmake_minimum_required(VERSION 3.10.2)
include_directories(6_remapping_ports_of_a_subTrees)
project(remapping_ports_of_a_subTrees)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(${PROJECT_NAME} "remapping_ports_of_a_subTrees.cpp" "dummy_nodes.cpp")
target_link_libraries(${PROJECT_NAME} BT::behaviortree_cpp)
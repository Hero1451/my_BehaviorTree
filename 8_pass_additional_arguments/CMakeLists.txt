cmake_minimum_required(VERSION 3.10.2)
include_directories(8_lpass_additional_arguments)
project(pass_additional_arguments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(${PROJECT_NAME} "pass_additional_arguments.cpp" "dummy_nodes.cpp")
target_link_libraries(${PROJECT_NAME} BT::behaviortree_cpp)
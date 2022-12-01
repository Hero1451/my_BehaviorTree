cmake_minimum_required(VERSION 3.10.2)
project(ports_with_generic_types)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(${PROJECT_NAME} "ports_with_generic_types.cpp")
target_link_libraries(${PROJECT_NAME} BT::behaviortree_cpp)
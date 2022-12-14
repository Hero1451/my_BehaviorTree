cmake_minimum_required(VERSION 3.10.2)
include_directories(10_use_xml_scripting)
project(use_xml_scripting)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(${PROJECT_NAME} "use_xml_scripting.cpp" "dummy_nodes.cpp")
target_link_libraries(${PROJECT_NAME} BT::behaviortree_cpp)
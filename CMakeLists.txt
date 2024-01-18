cmake_minimum_required(VERSION 3.5)

project(btcpp_sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(behaviortree_cpp REQUIRED)

# add_executable(btcpp_sample main.cpp)

# target_link_libraries(btcpp_sample BT::behaviortree_cpp)
add_subdirectory(demo)
add_subdirectory(tutorial_1)
add_subdirectory(tutorial_2)


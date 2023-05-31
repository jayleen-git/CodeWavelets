cmake_minimum_required(VERSION 3.10)
project(UnitTests)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wpedantic")

# add_executable(my_tests test.cpp)
add_executable(my_tests test_dwt.cpp ../dwt.cpp)

# add the binary tree to the search path for include files
include_directories(/home/ubuntu/lib/Catch2)

# enable CTest testing
enable_testing()
cmake_minimum_required(VERSION 3.0)
project(consensus_simulator)

# options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++1z")
# Why not work C++1z? I want to use nested namespace!
# Check option DEBUG, RELEASE...

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(cmake/dependencies.cmake)
include(cmake/macros.cmake)

include_directories(.)

# subdirectories
add_subdirectory(consensus)
add_subdirectory(main)
add_subdirectory(repository)

# test
add_subdirectory(test)

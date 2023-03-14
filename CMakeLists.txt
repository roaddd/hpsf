cmake_minimum_required(VERSION 3.22)

project(hpfs CXX)

set(CMAKE_CXX_FLAGS "-g -pthread -std=c++20 -fcoroutines")
set(CMAKE_CXX_COMPLIER "g++")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(coroutine)
add_subdirectory(test)
cmake_minimum_required(VERSION 2.6)

project(hpfs CXX)

set(CMAKE_CXX_FLAGS -g)
set(CMAKE_CXX_COMPLIER "g++")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(hpfs test.cpp TcpServer.cpp Channel.cpp Acceptor.cpp TcpConnection.cpp)
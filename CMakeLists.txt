cmake_minimum_required(VERSION 2.6)

project(hpfs CXX)

set(CMAKE_CXX_FLAGS "-g -pthread")
set(CMAKE_CXX_COMPLIER "g++")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(hpfs test.cpp 
Acceptor.cpp 
Buffer.cpp
BlockingQueue.hpp
Channel.cpp 
Condition.hpp
CurrentThread.hpp
Epoll.cpp 
EventLoop.cpp 
EchoServer.cpp
Mutex.hpp
Task.cpp
TcpServer.cpp 
TcpConnection.cpp 
TimerQueue.cpp
Timestamp.cpp
Thread.cpp
ThreadPool.cpp
)
cmake_minimum_required(VERSION 3.13)
project(socketplusplus)

add_executable(test main.cpp src/server.cpp src/request.cpp src/scheduler.cpp http.cpp)


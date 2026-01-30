
cmake_minimum_required(VERSION 3.13)
project(socketpluplus)


add_library(socketplusplus SHARED 
    src/server.cpp 
    src/request.cpp  
    src/scheduler.cpp
)

 target_include_directories(socketplusplus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
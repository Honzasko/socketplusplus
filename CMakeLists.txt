cmake_minimum_required(VERSION 3.13)
project(test)

set(LIB "lib/include")
set(LIB_DIR "lib/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_library(SOCKET_LIB 
    NAMES socketplusplus
    PATHS ${LIB_DIR}
    NO_DEFAULT_PATH
)



add_executable(test main.cpp http.cpp)

target_link_libraries(test PRIVATE ${SOCKET_LIB})
target_include_directories(test PRIVATE ${LIB})
cmake_minimum_required(VERSION 3.13)
project(HW3)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
set(TARGET_LIB
        "/usr/lib/x86_64-linux-gnu/libGL.so"
        "/usr/lib/x86_64-linux-gnu/libGLU.so"
        "/usr/lib/x86_64-linux-gnu/libglut.so"
        )

add_executable(HW3 main.cpp)

target_link_libraries(HW3 ${TARGET_LIB})


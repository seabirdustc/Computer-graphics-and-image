cmake_minimum_required(VERSION 3.13)
project(HW2)

set(CMAKE_CXX_STANDARD 14)

# 添加源文件
set(SOURCE_FILES glad.c main.cpp)
add_executable(HW2 ${SOURCE_FILES})
target_link_libraries(HW2 glfw3 GL m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor)
cmake_minimum_required(VERSION 3.7)
project(testTaskTekon)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c)
add_executable(testTaskTekon ${SOURCE_FILES})
target_link_libraries(testTaskTekon m)
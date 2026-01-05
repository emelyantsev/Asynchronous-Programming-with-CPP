cmake_minimum_required(VERSION 3.16)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(program_3_06 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 20)
endif()

add_executable(program_3_06 main.cpp)
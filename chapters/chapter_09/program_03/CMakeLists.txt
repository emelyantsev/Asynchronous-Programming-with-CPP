cmake_minimum_required(VERSION 3.16)

# Если проект запускается напрямую
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(program_3_02 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 20)
    find_package(Boost COMPONENTS thread REQUIRED)
endif()

add_executable(program_3_02 main.cpp)
target_link_libraries(program_3_02  Boost::thread)
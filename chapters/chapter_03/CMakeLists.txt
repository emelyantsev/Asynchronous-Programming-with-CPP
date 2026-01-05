cmake_minimum_required(VERSION 3.16)

# Если эта папка собрана напрямую (без корня) → делаем отдельный проект
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(chapter03 LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 20)
    find_package(Boost COMPONENTS thread REQUIRED)
endif()

project(chapter03 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(program_3_01)
add_subdirectory(program_3_02)
add_subdirectory(program_3_06)
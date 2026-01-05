cmake_minimum_required(VERSION 3.16)
project(CppBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS thread REQUIRED)

add_subdirectory(chapters/chapter_03)

enable_testing()
add_subdirectory(tests)
cmake_minimum_required(VERSION 3.10)
project(MyBenchmark)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark REQUIRED)

add_executable(run_bench source.cpp)

target_link_libraries(run_bench benchmark::benchmark)
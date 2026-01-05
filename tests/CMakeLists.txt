cmake_minimum_required(VERSION 3.16)

# Если собираем только тесты напрямую
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    project(Tests LANGUAGES CXX)
    set(CMAKE_CXX_STANDARD 17)
endif()

add_executable(run_all_tests
    chapter_03/test_01/thread_test.cpp
    chapter_03/test_02/thread_output_test.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(run_all_tests PRIVATE GTest::gtest_main pthread)

include(GoogleTest)
gtest_discover_tests(run_all_tests)
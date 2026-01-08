project(H2OTest)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

enable_testing()

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_tests water_test.cpp water_test_2.cpp)

target_link_libraries(unit_tests 
    GTest::gtest 
    GTest::gtest_main 
    Threads::Threads
)

gtest_discover_tests(unit_tests)
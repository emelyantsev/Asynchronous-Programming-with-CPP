#include <gtest/gtest.h>

#include "thread_examples.h"

TEST(ThreadTests, AllThreadsIncrementCounter) {

    run_threads();
    EXPECT_EQ(counter.load(), 6) << "All 6 threads should increment counter";
}


TEST(ThreadTests, MultipleRunsAccumulate) {
    run_threads();
    int first = counter.load();
    EXPECT_EQ(first, 6);

    run_threads();
    int second = counter.load();
    EXPECT_EQ(second, 6) << "Counter should reset to 6 each run";
}
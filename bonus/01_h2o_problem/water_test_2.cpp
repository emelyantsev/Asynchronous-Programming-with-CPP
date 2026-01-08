#include <gtest/gtest.h>

#include <thread>
#include <vector>
#include <string>
#include <algorithm>

#include "h2o_sem.h"

class H2OParamTest : public testing::TestWithParam<int> {

    protected:
        H2O molecule;
};


TEST_P(H2OParamTest, SynthesisCorrectness) {
    
    int n = GetParam(); 
    
    std::string result = "";
    std::mutex result_mtx;
    std::vector<std::thread> threads;

    auto releaseH = [&]() { 
        std::lock_guard<std::mutex> lock(result_mtx);
        result += "H"; 
    };
    auto releaseO = [&]() { 
        std::lock_guard<std::mutex> lock(result_mtx);
        result += "O"; 
    };


    for (int i = 0; i < n; ++i) threads.emplace_back(&H2O::oxygen, &molecule, releaseO);
    for (int i = 0; i < 2 * n; ++i) threads.emplace_back(&H2O::hydrogen, &molecule, releaseH);

    for (auto& t : threads) t.join();

  
    ASSERT_EQ(result.length(), n * 3);

    for (int i = 0; i < result.length(); i += 3) {
    
        std::string group = result.substr(i, 3);
        std::sort(group.begin(), group.end());
        EXPECT_EQ(group, "HHO");
    }
}


INSTANTIATE_TEST_SUITE_P(
    WaterTests,              
    H2OParamTest,            
    testing::Values(1, 10, 100) 
);
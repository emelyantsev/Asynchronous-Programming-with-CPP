#include <gtest/gtest.h>
#include <thread>
#include <sstream>
#include <iostream>
#include <syncstream>

void func(std::ostream& os) {
    // Create local sync wrapper for thread-safe atomic write
    std::osyncstream(os) << "Using function pointer\n";
}

class FuncObjectClass {
public:
    void operator()(std::ostream& os) {
        std::osyncstream(os) << "Using function object class\n";
    }
};

class Obj {
public:
    void func(std::ostream& os) {
        std::osyncstream(os) << "Using a non-static member function\n";
    }
    static void static_func(std::ostream& os) {
        std::osyncstream(os) << "Using a static member function\n";
    }
};

TEST(ThreadOutputTests, VariousThreads) {
    std::ostringstream oss;

    std::thread t1(func, std::ref(oss));

    auto lambda_func = [&]( ) {
        // Wrap oss locally in the lambda
        std::osyncstream(oss) << "Using lambda function\n";
    };
    std::thread t2(lambda_func);

    std::thread t3([&]() {
        std::osyncstream(oss) << "Using embedded lambda function\n";
    });

    std::thread t4(FuncObjectClass(), std::ref(oss));

    Obj obj;
    std::thread t5(&Obj::func, &obj, std::ref(oss));

    std::thread t6(&Obj::static_func, std::ref(oss));

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    std::string output = oss.str();
    std::cout << output << std::endl;

    EXPECT_NE(output.find("Using function pointer"), std::string::npos);
    EXPECT_NE(output.find("Using lambda function"), std::string::npos);
    EXPECT_NE(output.find("Using embedded lambda function"), std::string::npos);
    EXPECT_NE(output.find("Using function object class"), std::string::npos);
    EXPECT_NE(output.find("Using a non-static member function"), std::string::npos);
    EXPECT_NE(output.find("Using a static member function"), std::string::npos);
}

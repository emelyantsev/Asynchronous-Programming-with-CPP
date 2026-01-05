#pragma once
#include <atomic>
#include <thread>

inline std::atomic<int> counter = 0;

inline void func() {
    counter++;
}

class FuncObjectClass {
   public:
    void operator()() {
        counter++;
    }
};

class Obj {
   public:
    void func() {
        counter++;
    }
    static void static_func() {
        counter++;
    }
};

inline void run_threads() {
    
    counter = 0;

    std::thread t1(func);

    auto lambda_func = []() { counter++; };
    std::thread t2(lambda_func);

    std::thread t3([]() { counter++; });

    std::thread t4{FuncObjectClass()};

    Obj obj2;
    std::thread t5(&Obj::func, &obj2);

    std::thread t6(&Obj::static_func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}
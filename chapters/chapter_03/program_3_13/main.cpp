#include <chrono>

#include "thread_wrapper.h"

using namespace std::chrono_literals;

void func(const std::string& name) {

    sync_cout << "Thread " << name << " starting..." << std::endl;
    std::this_thread::sleep_for(1s);
    sync_cout << "Thread " << name << " finishing..." << std::endl;
}

int main() {

    JthreadWrapper t1(func, "t1");
    JthreadWrapper t2(func, "t2");
    JthreadWrapper t3(func, "t3");

    std::this_thread::sleep_for(2s);

    // t1, t2, t3 will be destroyed when main exits
    sync_cout << "Main thread exiting..." << std::endl;

    return 0;
}
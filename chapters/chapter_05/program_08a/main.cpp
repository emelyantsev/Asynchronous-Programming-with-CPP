#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>


std::atomic<std::string *> msg{nullptr};
int data{0};


void producer() {

    auto *p = new std::string("Hello");
    msg.store( p, std::memory_order_release );
    data = 42;
}

void consumer() {

    std::string *p2;

    while ( !( p2 = msg.load(std::memory_order_acquire) ) ) {}

    std::cout << *p2 << std::endl;
    std::cout << "The answer is " << data << std::endl;
}



int main() {

    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join();
    t2.join();

    return 0;
}
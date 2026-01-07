#include <chrono>
#include <format>
#include <iostream>
#include <thread>
#include <syncstream>

#include "semaphore_queue.h"

#ifndef sync_cout
#define sync_cout std::osyncstream(std::cout)
#endif

void push_pop(std::size_t capacity, int items, int producer_wait, int consumer_wait) {
    
    async_prog::SemaphoreQueue<int> queue(capacity);

    std::jthread t1( [&] {

        for (int i = 0; i < items; ++i) {
        
            queue.push(i);
        //    sync_cout << std::format("Produced item: {0}\n", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(producer_wait));
        }
    });

    std::jthread t2( [&] {

        for (int i = 0; i < items; ++i) {
        
            int item{};
            queue.pop(item);
        //    sync_cout << std::format("Consumed item: {0}\n", item);
            std::this_thread::sleep_for(std::chrono::milliseconds(consumer_wait));
        }
    });
}

void try_push_pop( std::size_t capacity, int items, int producer_wait, int consumer_wait ) {
    
    async_prog::SemaphoreQueue<int> queue(capacity);

    std::jthread t1( [&] {

        int i{};

        while (i < items) {

            if ( !queue.try_push(i) ) {
                
                std::this_thread::sleep_for(std::chrono::milliseconds(producer_wait));
            } else {

            //    sync_cout << std::format("Produced item: {0}\n", i);
                ++i;
            }
        }
    });

    std::jthread t2( [&] {

        int i{};
        
        while (i < items) {
        
            int item{};
        
            if ( !queue.try_pop(item) ) {
                
                std::this_thread::sleep_for(std::chrono::milliseconds(consumer_wait));
            } 
            else {                
            //    sync_cout << std::format("Consumed item: {0}\n", item);
                ++i;
            }
        }
    });
}

int main() {

    auto pre = std::chrono::high_resolution_clock::now();
//    push_pop(5, 1000, 1, 5);
    auto post = std::chrono::high_resolution_clock::now();

    auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(post - pre);
 //   std::cout << std::format("push/pop took {0} to process {1} items\n", millisec, 1000);

    pre = std::chrono::high_resolution_clock::now();
    try_push_pop(5, 1000, 1, 5);
    post = std::chrono::high_resolution_clock::now();

    millisec = std::chrono::duration_cast<std::chrono::milliseconds>(post - pre);
    std::cout << std::format("try_push/try_pop took {0} to process {1} items\n", millisec, 1000);

    return 0;
}
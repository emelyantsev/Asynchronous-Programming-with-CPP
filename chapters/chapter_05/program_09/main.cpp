#include <iostream>
#include <thread>

#include <syncstream>

#ifndef sync_out
#define sync_cout std::osyncstream(std::cout)
#endif


#include "spsc_lock_free_queue.h"


constexpr int NUM_STEPS {100'000'000} ;
constexpr int MOD {1'000'000'007} ;


void producer(spsc_lock_free_queue<int> &queue) {

    for (int i = 0; i < NUM_STEPS; ++i) {
    
        while ( !queue.push(i) ) {}
    }
}

void consumer(spsc_lock_free_queue<int> &queue) {

    int res_sum = 0;

    for (int i = 0; i < NUM_STEPS; ++i) {
    
        int data;
    
        while ( !queue.pop(data) ) {}

        res_sum = (res_sum + data) % MOD ;
    }

    sync_cout << "RESULT SUM = " << res_sum << std::endl; 
}

int main() {

    spsc_lock_free_queue<int> queue(8);

    std::thread t1( producer, std::ref(queue) );
    std::thread t2( consumer, std::ref(queue) );

    t1.join();
    t2.join();

    return 0;
}
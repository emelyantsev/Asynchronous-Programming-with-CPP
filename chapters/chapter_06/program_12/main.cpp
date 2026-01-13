#include <iostream>

#include "thread_safe_queue.h"


constexpr unsigned VALUE_RANGE = 1000; 
constexpr unsigned RESULTS_TO_PRODUCE = 10;  // Numbers of items to produce.
constexpr unsigned MAX_WAIT = 500;           // Maximum waiting time (ms) when producing items.


using TaskQueue = ThreadSafeQueue<std::future<int>>;


void producer(TaskQueue& queue, int val) {

    std::promise<int> prom;
    auto fut = prom.get_future();

    queue.push(std::move(fut));

    std::this_thread::sleep_for( std::chrono::milliseconds(rand() % MAX_WAIT) );

    prom.set_value(val);
}

void consumer(TaskQueue& queue) {

    std::this_thread::sleep_for( std::chrono::milliseconds(rand() % MAX_WAIT) );

    std::future<int> fut = queue.pop();

    try {

        int result = fut.get();
        std::cout << "Result: " << result << std::endl;

    } catch ( const std::exception& e ) {

        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

int main() {

    TaskQueue queue;

    auto producerFunc = []( TaskQueue& queue ) {
    
        auto n = RESULTS_TO_PRODUCE;
    
        while ( n-- > 0 ) {
    
            int val = rand() % VALUE_RANGE;
            std::cout << "Producer: Sending value " << val << std::endl;
            producer( queue, val );
        }
    };

    auto consumerFunc = []( TaskQueue& queue ) {
        
        auto n = RESULTS_TO_PRODUCE;
        
        while ( n-- > 0 ) {
        
            std::cout << "Consumer: Receiving value" << std::endl;
            consumer( queue );
        }
    };

    std::jthread producerThread( producerFunc, std::ref( queue ) );
    std::jthread consumerThread( consumerFunc, std::ref( queue ) );

    return 0;
}
#include <algorithm>
#include <barrier>
#include <format>
#include <iostream>
#include <latch>
#include <numeric>
#include <thread>
#include <vector>
#include <syncstream>


#ifndef sync_cout
#define sync_cout std::osyncstream(std::cout)
#endif


void multiply_add_barrier() {

    const int NUM_THREADS{3};

    std::vector<int> sum(3, 0);
    std::vector<int> numbers( 3000 );
    std::iota( numbers.begin(), numbers.end(), 0);

    std::barrier map_barrier{NUM_THREADS};

    auto worker_thread = [&]( std::vector<int>& numbers, int start, int end, int id ) {
    
        sync_cout << std::format("Thread {0} is starting...\n", id);

        for (int i = start; i < end; ++i) {
            numbers[i] *= 2;
        }

        map_barrier.arrive_and_wait();

        for (int i = start; i < end; ++i) {
            sum[id] += numbers[i];
        }

        auto tk = map_barrier.arrive();
        (void) tk;
    };

    std::vector<std::jthread> workers;

    for (int i = 0; i < NUM_THREADS; ++i) {
    
        workers.emplace_back( worker_thread, std::ref(numbers), 1000 * i, 1000 * (i + 1), i );
    }

    for ( auto& t : workers ) {

        t.join();
    }

    std::cout << std::format( "All threads finished. The total sum is: {0}\n", std::accumulate( sum.begin(), sum.end(), 0 ) );
}

int main() {

    std::cout << "Multiplying and reducing vector using barriers..." << std::endl;    
    multiply_add_barrier();

    return 0;
}
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "spin_lock.h"


constexpr uint32_t NUM_THREADS{4};
std::atomic<uint32_t> atomic_counter{0};


int main() {

    std::vector<std::thread> threads(NUM_THREADS);

    auto start = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < NUM_THREADS; ++i) {

        threads[i] = std::thread( []() {
        
            for (int i = 0; i < 200'000'000; ++i) {
                atomic_counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }

    for (auto &t: threads) {
        t.join();
    }
    
    auto stop = std::chrono::high_resolution_clock::now();
    auto elapsed = stop - start;

    std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    std::cout << atomic_counter << std::endl;

    return EXIT_SUCCESS;
}


#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "spin_lock.h"


constexpr uint32_t NUM_THREADS{4};
uint32_t counter{0};


int main() {

    std::vector<std::thread> threads(NUM_THREADS);

    spin_lock sl;    
    counter = 0;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (uint32_t i = 0; i < NUM_THREADS; ++i) {
        
        threads[i] = std::thread( [&sl] {
        
            for (int i = 0; i < 200'000'000; ++i) {
                sl.lock();
                counter++;
                sl.unlock();
            }
        });
    }

    for (auto &t: threads) {
    
        t.join();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = stop - start;
    
    std::cout << "Elapsed time: " << elapsed.count() << std::endl;
    std::cout << counter << std::endl;

    return EXIT_SUCCESS;
}


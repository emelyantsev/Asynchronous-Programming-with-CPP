#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "spin_lock.h"


constexpr uint32_t NUM_THREADS{4};
uint32_t counter{0};


int main() {

    std::mutex m;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;

    for (uint32_t i = 0; i < NUM_THREADS; ++i) {

        threads.emplace_back( [&m] {
        
            for (int i = 0; i < 200'000'000; ++i) {
                m.lock();
                counter++;
                m.unlock();
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


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


void multiply_add_latch() {

    const int NUM_THREADS{3};

    std::latch map_latch{NUM_THREADS};
    std::latch reduce_latch{1};

    std::vector<int> numbers(3000);
    int sum{};
    std::iota( numbers.begin(), numbers.end(), 0);

    auto map_thread = [&]( std::vector<int>& numbers, int start, int end) {

        sync_cout << "map thread started" << std::endl;

        for (int i = start; i < end; ++i) {
            numbers[i] *= 2;
        }

        map_latch.count_down();

        sync_cout << "map thread finished" << std::endl;
    };

    auto reduce_thread = [&]( const std::vector<int>& numbers, int& sum) {

        map_latch.wait();
        sync_cout << "reduce thread started" << std::endl;

        sum = std::accumulate( numbers.begin(), numbers.end(), 0 );

        reduce_latch.count_down();
    };

    std::vector<std::jthread> mul_treads;

    for (int i = 0; i < NUM_THREADS; ++i) {

        std::jthread t( map_thread, std::ref(numbers), 1000 * i, 1000 * (i + 1) );

        mul_treads.push_back( std::move(t) ) ;
        //mul_treads.emplace_back( map_thread, std::ref(numbers), 1000 * i, 1000 * (i + 1) ) ;
    }

    std::jthread t( reduce_thread, std::ref(numbers), std::ref(sum) );

    reduce_latch.wait();

    std::cout << "All threads finished. The total sum is: " << sum << '\n';
}


int main() {

    std::cout << "Multiplying and reducing vector using latches..." << std::endl;
    multiply_add_latch();

    return 0;
}
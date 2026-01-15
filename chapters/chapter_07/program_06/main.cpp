#include <chrono>
#include <functional>
#include <future>
#include <iostream>

#include "common.h"


std::string format_ns(long long ns) {

    std::string s = std::to_string( ns );

    int n = s.length() - 3;

    while (n > 0) {

        s.insert(n, "'");
        n -= 3;
    }

    return s;
}


unsigned func(unsigned x, bool verbose = false) {

    using namespace std::chrono;
    
    if ( verbose )  {
        auto start = high_resolution_clock::now();
        auto start_ns = duration_cast<nanoseconds>(start.time_since_epoch()).count();

        sync_cout << "Task [" << x << "] started at " << format_ns(start_ns) << std::endl;
    }
    
    std::this_thread::sleep_for(10ms);

    if ( verbose )  {
        auto end = high_resolution_clock::now();
        auto end_ns = duration_cast<nanoseconds>(end.time_since_epoch()).count();

        sync_cout << "Task [" << x << "] ended at " << format_ns(end_ns) << std::endl;
    }
    
    return 2 * x;
}

int main() {


    constexpr unsigned NUM_TASKS = 32;

    auto duration_from = []( auto start ) {

        auto dur = std::chrono::high_resolution_clock::now() - start;
        return std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    };

    auto show_results = []( const std::vector<unsigned>& res ) {

        sync_cout << "Results [" << res.size() << "] ";
        
        for ( auto r : res ) {
            sync_cout << r << " ";
        }        
        sync_cout << '\n';
    };


    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < NUM_TASKS; i++) {

        std::async(std::launch::async, func, i, false);
    }

    sync_cout << "Discarding futures: " << duration_from(start) << '\n';



    // Reusing a future, discarding previous tasks.
    start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < NUM_TASKS; i++) {

        auto fut = std::async(std::launch::async, func, i, false);
    }

    sync_cout << "In-place futures: " << duration_from(start) << '\n';

    // Reusing a future, discarding previous tasks.
    std::future<unsigned> fut;

    start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < NUM_TASKS; i++) {

        fut = std::async(std::launch::async, func, i, true);
    }

    sync_cout << "Reusing future: " << duration_from(start) << '\n';

    // Same as before but waiting for tasks to finish.
    std::vector<unsigned> res;

    start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < NUM_TASKS; i++) {

        auto fut = std::async(std::launch::async, func, i, false);

        res.push_back(fut.get());
    }

    sync_cout << "Reused future and storing results: " << duration_from(start) << '\n';
    show_results(res);

    // Running several tasks storing a future per task in a vector and waiting for futures afterwards.
    std::vector<std::future<unsigned>> futsVec;
    res.clear();
    start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < NUM_TASKS; i++) {
        futsVec.emplace_back(std::async(std::launch::async, func, i, false));
    }

    for (unsigned i = 0; i < NUM_TASKS; i++) {
        res.push_back( futsVec[i].get() );
    }

    sync_cout << "Futures vector and storing results: " << duration_from(start) << '\n';
    show_results(res);

    return 0;
}
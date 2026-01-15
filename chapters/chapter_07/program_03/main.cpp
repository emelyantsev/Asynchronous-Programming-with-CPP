#include <chrono>
#include <cmath>
#include <future>
#include <iostream>
#include <thread>
#include <vector>
#include <syncstream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;


int compute(unsigned taskId, int x, int y) {

    std::this_thread::sleep_for( std::chrono::milliseconds( rand() % 200 ) );
    
    sync_cout << "Running task " << taskId << '\n';
    
    return std::pow(x, y);
}


int main() {

    std::vector<std::future<int>> futVec;

    for (int i = 0; i <= 10; i++) {

        futVec.emplace_back( std::async( compute, i+1, 2, i ) );
    }

    sync_cout << "Waiting in main thread\n";
    std::this_thread::sleep_for(1s);

    std::vector<int> results;

    for ( auto& fut : futVec ) {

        results.push_back( fut.get() );
    }
        
    
    for (auto& res : results) {

        sync_cout  << res << ' ';
    }
        
    sync_cout  << std::endl;

    return 0;

}
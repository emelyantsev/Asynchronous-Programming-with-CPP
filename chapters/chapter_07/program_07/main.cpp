#include <future>
#include <iostream>
#include <semaphore>
#include <syncstream>
#include <vector>
#include <utility>

#include "common.h"
#include "semaphore_guard.h"


void task( int id, [[maybe_unused]] MovableSemaphoreGuard< std::counting_semaphore<> > guard ) {

    sync_cout << "Running task " << id << "...\n";
    std::this_thread::sleep_for(1s);
}

int main() {

    const int total_tasks = 32;
    const int max_concurrent_tasks = std::thread::hardware_concurrency();
    
    std::counting_semaphore<> sem(max_concurrent_tasks);

    sync_cout << "Allowing only " << max_concurrent_tasks 
              << " concurrent tasks to run " << total_tasks << " tasks.\n";

    std::vector<std::future<void>> futures;

    for (int i = 0; i < total_tasks; ++i) {

        MovableSemaphoreGuard guard(sem);

        futures.push_back( std::async( std::launch::async, task, i, std::move( guard ) ) );
    }

    for ( auto& fut : futures) {

        fut.get();
    }

    std::cout << "All tasks completed." << std::endl;
    
    return 0;
}
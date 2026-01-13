#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <syncstream>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

const int CHECK_PERIOD_MS = 100;  // Cancellation token checking period (ms)

bool long_running_task(int ms, const std::atomic_bool& cancellation_token) {

    // Run the task for an amount of milliseconds, periodically checking cancellation_token.
    while (ms > 0 ) {

        if (cancellation_token) {
            sync_cout << "task cancelled" << std::endl;
            break;
        }
    
        ms -= CHECK_PERIOD_MS;
        std::this_thread::sleep_for( std::chrono::milliseconds{CHECK_PERIOD_MS} );
    }
    
    return cancellation_token;
}

int main() {

    std::atomic_bool cancellation_token{false};
    sync_cout << "Starting long running tasks..." << std::endl;

    std::packaged_task<bool(int, const std::atomic_bool&)> task1( long_running_task );
    std::future<bool> result1 = task1.get_future();
    std::jthread t1( std::move( task1 ), 500, std::ref(cancellation_token));

    std::packaged_task<bool(int, const std::atomic_bool&)> task2( long_running_task );
    std::future<bool> result2 = task2.get_future();
    std::jthread t2( std::move( task2 ), 1000, std::ref(cancellation_token));

    sync_cout << "Cancelling tasks after 600 ms..." << std::endl;
    std::this_thread::sleep_for( 600ms );
    cancellation_token = true;

    sync_cout << "Task1, waiting for 500 ms. Cancelled = " << std::boolalpha << result1.get() << std::endl;
    sync_cout << "Task2, waiting for 1 second. Cancelled = " << std::boolalpha << result2.get() << std::endl;

}
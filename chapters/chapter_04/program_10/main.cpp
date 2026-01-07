#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <syncstream>


#ifndef sync_cout
#define sync_cout std::osyncstream(std::cout)
#endif


int counter = 0;

int main() {

    using namespace std::chrono_literals;

    std::mutex mtx;
    std::condition_variable cv;

    auto increment_counter = [&] {

        for (int i = 0; i < 20; ++i) {
        
            std::this_thread::sleep_for(500ms);
        
            mtx.lock();
        
            ++counter;
            mtx.unlock();
            cv.notify_one();
        }
    };

    auto wait_for_counter_10_sleep = [&] {

        mtx.lock();

        while ( ! ( counter == 10 )  ) {

            sync_cout << counter << " sleeping" << std::endl;
            mtx.unlock();
            std::this_thread::sleep_for(100ms);
            mtx.lock();
        }

        mtx.unlock();

        sync_cout << counter << " exiting" << std::endl;
    };

    auto wait_for_counter_10_cv = [&] {

        std::unique_lock<std::mutex> lck(mtx);

        cv.wait( lck, [] { 
        
            sync_cout << counter << (counter == 10 ? " exiting " : " waiting" ) << std::endl;
            return counter == 10; 
        });

        sync_cout << "Counter is: " << counter << std::endl;
    };

   std::thread t1( wait_for_counter_10_sleep );
//    std::thread t2( wait_for_counter_10_cv );
    std::thread t3( increment_counter );

   t1.join();
//    t2.join();
    t3.join();

    return 0;
}
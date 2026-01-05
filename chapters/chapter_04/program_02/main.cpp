#include <format>
#include <iostream>
#include <mutex>
#include <thread>

#define NUM_COUNTS 1'000'000

std::mutex mtx;
uint32_t counter{};


void function_throws() { throw std::runtime_error("A very big error"); }

int main() {

    auto worker = [] {
    
        for (int i = 0; i < NUM_COUNTS; ++i) {
    
            mtx.lock();
            counter++;
            mtx.unlock();
        }
    };

    auto worker_try = [] {

        int i = 0;
        
        while ( i < NUM_COUNTS ) {
        
            if ( mtx.try_lock() ) {
        
                ++counter;
                ++i;
                mtx.unlock();
            }
        }
    };

    auto worker_exceptions = [] {
        
        for (int i = 0; i < NUM_COUNTS; ++i) {
        
            try {

                std::lock_guard<std::mutex> lock( mtx );
                counter++;
                function_throws();                
            } 
            catch ( std::runtime_error& e ) {
                
                std::cout << e.what() << std::endl;
                return;
            } 
            catch (...) {
                
                return;
            }
        }
    };


    std::thread t1( worker );
    std::thread t2( worker_exceptions );    
    std::thread t3( worker_try );


    t1.join();
    t2.join();
    t3.join();

    std::cout << std::format("Final counter value: {0}\n", counter);

}
#include <chrono>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include <syncstream>
 
using namespace std::chrono_literals;

std::timed_mutex mutex;


int64_t current_time_count() {

    auto now = std::chrono::system_clock::now();
    auto durationSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() );
    return durationSinceEpoch.count() ;
}
 
std::string current_time() {

    auto now = std::chrono::system_clock::now();

    auto ms = duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ) % 1000;

    std::time_t timer = std::chrono::system_clock::to_time_t( now ) ;
    std::tm bt = *std::localtime( &timer );

    std::ostringstream oss;
    oss << std::put_time(&bt, "%H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count(); 

    return oss.str();
}


void job(int id) {

    std::osyncstream(std::cout) <<  "["  << current_time() << "] " << "Worker id [" << id << "] started" << std::endl;

    if ( mutex.try_lock_for(1000ms) ) {

        std::osyncstream(std::cout) <<  "["  << current_time() << "] " << "Worker id [" << id << "] success " << std::endl;
        std::this_thread::sleep_for(2000ms);
        mutex.unlock();
        std::osyncstream(std::cout) <<  "["  << current_time() << "] " << "Worker id [" << id << "] finished " << std::endl;
    }
    else {
        std::osyncstream(std::cout) <<  "["  << current_time() << "] " << "Worker id [" << id << "] failed " << std::endl;
    }
}
 
int main() {

    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i)
        threads.emplace_back(job, i);
 
    for (auto& th : threads)
        th.join();
}
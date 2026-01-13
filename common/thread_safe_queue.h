#pragma once

#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>


template <typename T>
class ThreadSafeQueue {
   
    public:

        void push(T value) {
    
            std::lock_guard<std::mutex> lock( mutex_ );
            queue_.push( std::move( value ) );
            cond_var_.notify_one();
        }

        T pop() {
    
            std::unique_lock<std::mutex> lock( mutex_ );
            
            cond_var_.wait( lock, [&] { return !queue_.empty(); } );
            T value = std::move( queue_.front() ) ;
            queue_.pop();

            return value;
        }

    private:
    
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_var_;
};
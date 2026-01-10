// SPSC_lock_free_queue

#pragma once

#include <atomic>
#include <cassert>
#include <vector>

template<typename T>
class spsc_lock_free_queue {

    public:

        explicit spsc_lock_free_queue(size_t capacity) : capacity_{capacity}, buffer_(capacity) { 
            // capacity must be power of two to avoid using modulo operator when calculating the index   

            assert( (capacity & (capacity - 1) ) == 0 && "capacity must be a power of 2" );
        }

        spsc_lock_free_queue(const spsc_lock_free_queue &) = delete;

        spsc_lock_free_queue &operator=(const spsc_lock_free_queue &) = delete;


        bool push( const T &item ) {

            std::size_t tail = tail_.load(std::memory_order_relaxed);
            std::size_t next_tail = (tail + 1) & (capacity_ - 1);
            
            if ( next_tail != head_.load( std::memory_order_acquire ) ) {

                buffer_[tail] = item;
                tail_.store( next_tail, std::memory_order_release );
                
                return true;
            }

            return false;
        }


        bool pop( T &item ) {

            std::size_t head = head_.load( std::memory_order_relaxed );
            
            if ( head == tail_.load( std::memory_order_acquire) ) {

                return false;
            }

            item = buffer_[ head ];
            head_.store( ( head + 1 ) & ( capacity_ - 1 ), std::memory_order_release );

            return true;
        }

    private:

        const std::size_t capacity_;
        std::vector<T> buffer_;
        std::atomic<std::size_t> head_{0};
        std::atomic<std::size_t> tail_{0};
};
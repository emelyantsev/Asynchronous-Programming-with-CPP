#pragma once 

#include <atomic>


class spin_lock {

    public:

        spin_lock() : flag{ATOMIC_FLAG_INIT} {}

        spin_lock(const spin_lock &) = delete;

        spin_lock &operator=(const spin_lock &) = delete;

        void lock() {

            while ( flag.test_and_set(std::memory_order_acquire)) {}
        }

        void unlock() {

            flag.clear(std::memory_order_release);
        }

    private:
    
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
};
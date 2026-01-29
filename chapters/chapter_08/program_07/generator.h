#pragma once 

#include <coroutine>
#include <iostream>

template <typename T>
struct generator {

    struct promise_type {
    
        T value;

        generator get_return_object() {
    
            return generator{ std::coroutine_handle<promise_type>::from_promise( *this ) };
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        std::suspend_always yield_value( T v ) {

            value = v;
            return {};
        }

        void return_void() {}

        void unhandled_exception() { std::terminate(); }
    };

    using handle = std::coroutine_handle<promise_type>;
    
    handle h;

    explicit generator(handle h) : h(h) {}

    ~generator() { if (h) h.destroy(); }

    bool next() {

        if (!h || h.done()) return false;
        
        h.resume();
        
        return !h.done();
    }

    T value() const {
        
        return h.promise().value;
    }
};

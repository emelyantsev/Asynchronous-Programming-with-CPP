#include <coroutine>
#include <iostream>

#include "common.h"


struct custom_return_type {

    struct promise_type {

        custom_return_type get_return_object() noexcept {

            sync_cout << "get_return_object" << std::endl;
            return custom_return_type{ *this };
        }

        void return_void() noexcept {

            sync_cout << "return_void" << std::endl;
        }

        std::suspend_always initial_suspend() noexcept {

            sync_cout << "initial_suspend" << std::endl;
            return {};
        }

        std::suspend_always final_suspend() noexcept {

            sync_cout << "final_suspend" << std::endl;
            return {};
        }

        void unhandled_exception() noexcept {

            sync_cout << "unhandled_exception" << std::endl;
        }
    };


    explicit custom_return_type( promise_type& promise ) {

        sync_cout << "return_type()" << std::endl;
    }


    ~custom_return_type() noexcept {

        sync_cout << "~return_type()" << std::endl;
    }
};


custom_return_type coro_func() {

    co_return;
}



int main() {

    auto rt = coro_func();

    
    
    return 0;
}
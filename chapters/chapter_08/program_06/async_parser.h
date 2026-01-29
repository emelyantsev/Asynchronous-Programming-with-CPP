#pragma once 

#include <coroutine>
#include <optional>

template <typename Out, typename In>
struct async_parse {

    struct promise_type {
    
        std::optional<In> input_data {};
    
        Out output_data {};

        async_parse get_return_object() noexcept {
    
            return async_parse{ *this };
        }

        void return_void() noexcept {}

        std::suspend_always initial_suspend() noexcept {
            
            return {};
        }

        std::suspend_always final_suspend() noexcept {

            return {};
        }

        void unhandled_exception() noexcept {}

        std::suspend_always yield_value(int64_t num) noexcept {
            
            output_data = num;
            return {};
        }

        auto await_transform(char) noexcept {

            struct awaiter {
            
                promise_type& promise;

                [[nodiscard]] bool await_ready() const noexcept {

                    return promise.input_data.has_value();
                }

                [[nodiscard]] char await_resume() const noexcept {

                    assert (promise.input_data.has_value());

                    return *std::exchange( promise.input_data, std::nullopt );
                }

                void await_suspend(std::coroutine_handle<promise_type>) const noexcept {}

            };

            return awaiter(*this);

        }
    };

    std::coroutine_handle<promise_type> handle{};

    explicit async_parse(promise_type& promise) : handle{ std::coroutine_handle<promise_type>::from_promise(promise) } {}

    ~async_parse() noexcept {

        if ( handle ) {
        
            handle.destroy();
        }
    }

    void put(char c) {

        handle.promise().input_data = c;
        
        if ( !handle.done() ) {
        
            handle.resume();
        }
    }

    int64_t get() {

        if ( !handle.done() ) {
        
            handle.resume();
        }

        return handle.promise().output_data;
    }
};
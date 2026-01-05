#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <syncstream>
#include <thread>

#ifndef sync_out
#define sync_cout std::osyncstream(std::cout)
#endif

using namespace std::chrono_literals;

template<typename Duration>
class Timer {

    public:
        typedef std::function<void(void)> Callback;

    Timer(const Duration interval, const Callback& callback) {

        auto value = duration_cast<std::chrono::milliseconds>(interval);
        sync_cout << "Timer: Starting with interval of " << value << std::endl;

        t = std::jthread( [&](std::stop_token stop_token) {

            while ( !stop_token.stop_requested() ) {

                sync_cout << "Timer: Running callback " << val.load() << " ...\n";
                val++;
                callback();

                sync_cout << "Timer: Sleeping...\n";
                std::this_thread::sleep_for( interval );
            }

            sync_cout << "Timer: Exit\n";
        });
    }

    void stop() {

        t.request_stop();
    }

    private:
        std::jthread t;
        std::atomic_int32_t val{0};
};
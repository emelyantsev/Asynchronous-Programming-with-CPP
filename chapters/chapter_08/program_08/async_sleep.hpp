#pragma once

#include <coroutine>
#include <queue>
#include <chrono>
#include <thread>
#include <iostream>

using Clock = std::chrono::steady_clock;

// ================= EVENT LOOP =================

struct EventLoop {
    struct Timer {
        Clock::time_point when;
        std::coroutine_handle<> h;

        bool operator>(const Timer& other) const {
            return when > other.when;
        }
    };

    std::priority_queue<Timer, std::vector<Timer>, std::greater<>> timers;

    void schedule_after(std::chrono::milliseconds d,
                        std::coroutine_handle<> h)
    {
        timers.push(Timer{Clock::now() + d, h});
    }

    void run() {
        while (!timers.empty()) {
            auto next = timers.top();
            auto now = Clock::now();

            if (next.when > now) {
                std::this_thread::sleep_for(next.when - now);
            }

            timers.pop();
            next.h.resume();
        }
    }
};

// 👇 inline global event loop
inline EventLoop loop{};

// ================= AWAITABLE =================

struct Sleep {
    std::chrono::milliseconds dur;

    bool await_ready() const noexcept {
        return dur.count() == 0;
    }

    void await_suspend(std::coroutine_handle<> h) const {
        loop.schedule_after(dur, h);
    }

    void await_resume() const noexcept {}
};

// ================= TASK =================

struct Task {
    struct promise_type {
        Task get_return_object() {
            return Task{
                std::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };

    std::coroutine_handle<promise_type> h;

    explicit Task(std::coroutine_handle<promise_type> h) : h(h) {}
    ~Task() { if (h) h.destroy(); }

    void start() { h.resume(); }
};

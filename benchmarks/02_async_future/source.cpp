#include <benchmark/benchmark.h>
#include <future>
#include <vector>
#include <numeric>
#include <thread>


#include <queue>
#include <condition_variable>
#include <functional>


class ThreadPool {

public:

    ThreadPool(size_t threads) {

        for (size_t i = 0; i < threads; ++i) {

            workers.emplace_back([this] {

                for (;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<class F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {

        using return_type = decltype(f());

        auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));

        std::future<return_type> res = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace( [task]() { (*task)(); } );
        }

        condition.notify_one();

        return res;
    }

    ~ThreadPool() {
        
        { std::unique_lock<std::mutex> lock(queue_mutex); stop = true; }
        condition.notify_all();
        for (std::thread &worker : workers) worker.join();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
};




void DoWork(unsigned int iterations) {

    volatile int sink = 0;
    
    for (unsigned int i = 0; i < iterations; ++i) {
        sink += i;
    }

    benchmark::DoNotOptimize(sink);
}

// void DoWork(int ms) {

//     std::this_thread::sleep_for(std::chrono::milliseconds(ms/100));
// }


static void BM_Async_Discarding(benchmark::State& state) {

    for (auto _ : state) {
    
        for (int i = 0; i < state.range(0); ++i) {

            std::async(std::launch::async, DoWork, 1000);
        }
    }
}
BENCHMARK(BM_Async_Discarding)->Arg(32);


static void BM_Async_Reusing(benchmark::State& state) {

    for (auto _ : state) {

        std::future<void> fut;
        
        for (int i = 0; i < state.range(0); ++i) {

            fut = std::async(std::launch::async, DoWork, 1000);
        }
    }
}
BENCHMARK(BM_Async_Reusing)->Arg(32);


static void BM_Async_Parallel(benchmark::State& state) {

    for (auto _ : state) {

        std::vector<std::future<void>> futures;
        futures.reserve(state.range(0));
        
        for (int i = 0; i < state.range(0); ++i) {
            futures.push_back(std::async(std::launch::async, DoWork, 1000));
        }
        
        for (auto& fut : futures) {
            fut.get();
        }
    }
}
BENCHMARK(BM_Async_Parallel)->Arg(32);


static void BM_ThreadPool(benchmark::State& state) {

    int num_tasks = state.range(0);
    
    static ThreadPool pool( std::thread::hardware_concurrency() );
    
    for (auto _ : state) {

        std::vector<std::future<void>> futures;
        futures.reserve(num_tasks);

        for (int i = 0; i < num_tasks; ++i) {

            futures.push_back( pool.enqueue([] { DoWork(1000); } )  );
        }

        for (auto& fut : futures) fut.get();
    }
}

BENCHMARK(BM_ThreadPool)->Arg(32);

BENCHMARK_MAIN();
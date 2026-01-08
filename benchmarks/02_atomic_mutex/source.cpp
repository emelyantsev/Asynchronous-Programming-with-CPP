#include <benchmark/benchmark.h>
#include <mutex>
#include <atomic>


std::mutex mtx;
int mutex_counter = 0;

std::atomic<int> atomic_counter{0};


static void BM_MutexContention(benchmark::State& state) {

    if (state.thread_index() == 0) mutex_counter = 0;

    for (auto _ : state) {
        mtx.lock();
        ++mutex_counter;
        mtx.unlock();
    }
}


static void BM_AtomicContention(benchmark::State& state) {

    if ( state.thread_index() == 0 ) atomic_counter = 0;

    for (auto _ : state) {
       
        atomic_counter.fetch_add(1, std::memory_order_relaxed);
    }
}


BENCHMARK(BM_MutexContention)->ThreadRange(1, 8)->UseRealTime()->Unit(benchmark::kNanosecond);
BENCHMARK(BM_AtomicContention)->ThreadRange(1, 8)->UseRealTime()->Unit(benchmark::kNanosecond);

BENCHMARK_MAIN();
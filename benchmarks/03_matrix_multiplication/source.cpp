#include <benchmark/benchmark.h>
#include <vector>
#include <future>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "thread_pool.h"

using matrix_t = std::vector<std::vector<int>>;


std::atomic<int> g_unique_threads{0};

thread_local bool is_new_thread = true;

void mark_thread() {

    if (is_new_thread) {

        g_unique_threads++;
        is_new_thread = false;
    }
}



matrix_t create_matrix(int n) {

    return matrix_t(n, std::vector<int>(n, 1));
}


static void BM_AsyncPerRow(benchmark::State& state) {

    int n = state.range(0);
    g_unique_threads = 0; 

    matrix_t A = create_matrix(n);
    matrix_t B = create_matrix(n);

    for (auto _ : state) {

        g_unique_threads = 0; 

        matrix_t res( n, std::vector<int>(n, 0) );
        std::vector<std::future<void>> futs;

        for (int i = 0; i < n; ++i) {

            futs.push_back( std::async( std::launch::async, [i, n, &A, &B, &res] {

                mark_thread();
              
                for (int j = 0; j < n; ++j) {
                
                    int sum = 0;
                    for (int k = 0; k < n; ++k) {
                        sum += A[i][k] * B[k][j];
                    }
                    res[i][j] = sum;
                }

            } ) );
        }
        for (auto& f : futs) {
            
            f.wait();
        }

        

        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();

        state.counters["ThreadsCreated"] = benchmark::Counter(g_unique_threads, benchmark::Counter::kDefaults);

        
    }

    
}

BENCHMARK(BM_AsyncPerRow)->RangeMultiplier(2)->Range(64, 512)->Unit(benchmark::kMicrosecond);


// 2. ВАРИАНТ: Thread Pool на каждую строку
static void BM_ThreadPoolPerRow(benchmark::State& state) {

    int n = state.range(0);
    g_unique_threads = 0;
    
    matrix_t A = create_matrix(n);
    matrix_t B = create_matrix(n);
    
    ThreadPool pool( std::thread::hardware_concurrency() );

    for (auto _ : state) {

        matrix_t res(n, std::vector<int>(n, 0) );
        std::vector< std::future<void> > futs;

        for (int i = 0; i < n; ++i) {

            futs.push_back( pool.enqueue( [i, n, &A, &B, &res] {

                mark_thread();
            
                for (int j = 0; j < n; ++j) {
                
                    int sum = 0;
                    for (int k = 0; k < n; ++k) {
                        sum += A[i][k] * B[k][j];
                    }
                    res[i][j] = sum;
                }

            } ) );
        }
        for (auto& f : futs) f.wait();

        

        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();
        
        state.counters["ThreadsCreated"] = benchmark::Counter(g_unique_threads, benchmark::Counter::kDefaults);

    }

    
}
BENCHMARK(BM_ThreadPoolPerRow)->RangeMultiplier(2)->Range(64, 512)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
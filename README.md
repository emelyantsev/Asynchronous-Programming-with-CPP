### Concurrent and asynchronous programming with C++:


3. [How to Create and Manage Threads in C++](https://github.com/emelyantsev/Asynchronous-Programming-with-CPP/tree/main/chapters/chapter_03)
4. [Thread Synchronization with Locks](https://github.com/emelyantsev/Asynchronous-Programming-with-CPP/tree/main/chapters/chapter_04)
5. [Atomic operations](https://github.com/emelyantsev/Asynchronous-Programming-with-CPP/tree/main/chapters/chapter_05)
6. [Promises and futures](https://github.com/emelyantsev/Asynchronous-Programming-with-CPP/tree/main/chapters/chapter_06)
7. [The async function](https://github.com/emelyantsev/Asynchronous-Programming-with-CPP/tree/main/chapters/chapter_07)



```bash
# run shell inside container
docker exec -it async_cpp bash

# build with cmake 
cmake -S . -B build_debug -D CMAKE_BUILD_TYPE=Debug
cmake --build build_debug
ctest --test-dir build_debug

# build benchmark
cmake -S . -B build  -DCMAKE_BUILD_TYPE=Release
cmake --build build

# run benchmark
./build/run_bench
./build/run_bench --benchmark_out=results.csv --benchmark_out_format=csv
./build/run_bench --benchmark_format=json > results.json
```


#### Benchmarks results

2. [Async future benchmark source](benchmarks/02_async_future/source.cpp)

![Async future benchmark](benchmarks/02_async_future/results/benchmark_plot.png)

<details>
<summary>Async future benchmark console output</summary>

```console
Run on (8 X 3696.01 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 20480 KiB (x1)
Load Average: 0.24, 0.22, 0.11
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_Async_Discarding/32    1790018 ns       987054 ns          674
BM_Async_Reusing/32        847071 ns       790595 ns          881
BM_Async_Parallel/32      1204084 ns      1190893 ns          580
BM_ThreadPool/32           480371 ns       346177 ns         2013
```
</details>
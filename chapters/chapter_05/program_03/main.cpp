#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

constexpr int NUM_ITEMS{10000};

void process(const int duration) {

    std::this_thread::sleep_for( std::chrono::milliseconds( duration ) );
}

int main() {
    
    std::atomic<int> processed_items{0};
    std::atomic<float> total_time{0.0f};
    std::atomic<double> average_time{0.0};
    std::atomic<double> m_n{0.0};
    std::atomic<double> s_n{0.0};

    std::cout << "std::atomic<int> is lock-free: " << std::boolalpha << processed_items.is_lock_free() << std::endl;
    std::cout << "std::atomic<int> is always lock-free: " << std::boolalpha << std::atomic<int>::is_always_lock_free
              << std::endl;

    std::cout << "std::atomic<float> is lock-free: " << std::boolalpha << total_time.is_lock_free() << std::endl;
    std::cout << "std::atomic<float> is always lock-free: " << std::boolalpha << std::atomic<float>::is_always_lock_free
              << std::endl;

    std::cout << "std::atomic<double> is lock-free: " << std::boolalpha << average_time.is_lock_free() << std::endl;
    std::cout << "std::atomic<double> is always lock-free: " << std::boolalpha
              << std::atomic<double>::is_always_lock_free << std::endl;

    std::thread worker( [&] {

        std::random_device rd;
        std::mt19937 gen( rd() );
        //std::uniform_int_distribution<> dis(1, 20);

        double mean = 10.5;
        double stddev = 5.77;

        std::normal_distribution<double> dis(mean, stddev);


        for (int i = 1; i <= NUM_ITEMS; ++i) {

            int duration = dis(gen);

            auto now = std::chrono::high_resolution_clock::now();
            process(duration);
            auto elapsed = std::chrono::high_resolution_clock::now() - now;
            float elapsed_s = std::chrono::duration<float>(elapsed).count();

            processed_items.fetch_add(1, std::memory_order_relaxed);
            total_time.fetch_add(elapsed_s, std::memory_order_relaxed);
            average_time.store( total_time.load() / processed_items.load(), std::memory_order_relaxed );

            elapsed_s *= 1000;

            double old_m = m_n.load(std::memory_order_relaxed);
            double new_m = old_m + (elapsed_s - old_m) / i;

            double old_s = s_n.load(std::memory_order_relaxed);
            double new_s = old_s + (elapsed_s - old_m) * (elapsed_s - new_m);

            m_n.store(new_m, std::memory_order_relaxed);
            s_n.store(new_s, std::memory_order_relaxed);

        }

    });

    while (true) {

        int items = processed_items.load(std::memory_order_relaxed);
        std::cout << "Progress: " << items << " / " << NUM_ITEMS << " ";

        float time = total_time.load(std::memory_order_relaxed);
        std::cout << "| Total time: " << time << " sec" << " ";

        double average = average_time.load(std::memory_order_relaxed);
        std::cout << "| Average time: " << average * 1000 << " ms" << " ";

        double current_mean = m_n.load(std::memory_order_relaxed);
        double current_s = s_n.load(std::memory_order_relaxed);

        double variance = (items > 1) ? current_s / (items - 1) : 0.0;
        double std_dev = std::sqrt(variance);

        std::cout << " | Mean: " << current_mean << " ms"
                  << " | StdDev: " << std_dev << " ms" << std::endl;

        if ( items == NUM_ITEMS ) {

            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    worker.join();

    return 0;
}
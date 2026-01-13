#include <iostream>
#include <syncstream>
#include <thread>
#include <vector>
#include <chrono>
#include <condition_variable>
#include <mutex>

using namespace std::chrono_literals;

#define sync_cout std::osyncstream(std::cout)

int main() {

    // Данные для синхронизации
    std::mutex mtx;
    std::condition_variable cv;
    int shared_data = 0;
    bool ready = false; // Флаг готовности данных

    std::vector<std::jthread> threads;

    for (int i = 1; i <= 5; ++i) {

        threads.emplace_back( [&, i] {
        
            // 1. Захватываем мьютекс
            std::unique_lock lk(mtx);
            
            sync_cout << "Thread " << i << " is waiting..." << std::endl;

            // 2. Ждем уведомления. 
            // Лямбда [ready] - это предикат, защищающий от ложных пробуждений.
            cv.wait( lk, [&] { return ready; } );

            // 3. Работаем с данными
            sync_cout << "Thread " << i << ": Result = " << shared_data << std::endl;
        });
    }

    // Имитируем долгую работу
    std::this_thread::sleep_for(3s);

    {
        // 4. Обновляем данные под мьютексом
        std::lock_guard lk(mtx);
        shared_data = 5;
        ready = true;
        sync_cout << "Main thread: Data is ready, notifying all..." << std::endl;
    }

    // 5. Разблокируем все ожидающие потоки
    cv.notify_all();

    // jthread сами сделают join() при выходе из области видимости
    return 0;
}
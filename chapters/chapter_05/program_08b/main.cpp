#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

std::atomic<bool> x{false};
std::atomic<bool> y{false};
std::atomic<int> z{0};


void write_x() {

    x.store(true, std::memory_order_release);
}

void write_y() {

    y.store(true, std::memory_order_release);
}

void read_x_then_y() {

    while (!x.load(std::memory_order_acquire)) {}

    if ( y.load(std::memory_order_acquire) ) {
        ++z;
    }
}

void read_y_then_x() {

    while ( !y.load( std::memory_order_acquire ) ) {}

    if ( x.load(std::memory_order_acquire) ) {
        ++z;
    }
}


int main() {

    for (int i = 0; i < 10; ++i) {

        std::thread t1( write_x );
        std::thread t2( write_y );
        std::thread t3( read_x_then_y );
        std::thread t4( read_y_then_x );

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        if (z.load() == 0) {

            std::cout << "This will never happen\n";
        }

        {
            std::cout << "This will always happen and z = " << z << "\n";
        }
    }

    return 0;
}
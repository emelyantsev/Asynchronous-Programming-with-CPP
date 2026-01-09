#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<bool> x{ false } ;
std::atomic<bool> y{ false } ;
std::atomic<int> z{ 0 } ;


std::memory_order order { std::memory_order_seq_cst } ;


void write_x() {

    x.store( true, order  ) ;
}

void write_y() {

    y.store( true, order  );
}

void read_x_then_y() {

    while ( !x.load( order ) ) {}

    if ( y.load( order ) ) { 

        ++z;
    }
}

void read_y_then_x() {

    while ( !y.load( order ) ) {}

    if ( x.load( order ) ) {
        ++z;
    }
}

int main() {

    std::thread t1( write_x );
    std::thread t2( write_y );
    std::thread t3( read_x_then_y );
    std::thread t4( read_y_then_x );

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    if ( z.load( order ) == 0 ) {

        std::cout << "This will never happen\n";
    }

    {
        std::cout << "This will always happen and z = " << z << "\n";
    }

    return 0;
}
#include <atomic>
#include <thread>
#include <assert.h>
#include <iostream>

std::atomic<bool> x{false};
std::atomic<bool> y{false};
std::atomic<int> z{0};


std::memory_order mem_order { std::memory_order_relaxed } ;

void write_x_then_read_y() {
    
    x.store( true, mem_order ); 
    
    if ( !y.load( mem_order ) ) {
        
        ++z;
    }
}

void write_y_then_read_x() {

    y.store( true, mem_order );
    
    if ( !x.load( mem_order ) ) {
        
        ++z;
    }
}

int main() {

    std::thread a( write_x_then_read_y );
    std::thread b( write_y_then_read_x );
    
    a.join();
    b.join();

    assert( z.load() != 0 ); 

    std::cout << "Z is: " << z.load() << std::endl;
}
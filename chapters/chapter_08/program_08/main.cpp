#include "async_sleep.hpp"

using namespace std::chrono_literals;

Task foo() {

    std::cout << "A\n";
    
    co_await Sleep{1000ms};
    std::cout << "B\n";
    
    co_await Sleep{500ms};
    std::cout << "C\n";
}


Task worker(int id, int delay) {

    std::cout << "start " << id << "\n";
    
    co_await Sleep{ std::chrono::milliseconds(delay) };
    
    std::cout << "done " << id << "\n";
}


int main() {

    auto t1 = foo();
    auto t2 = worker( 1, 1000 );
    auto t3 = worker( 2, 500 );

    t1.start();
    t2.start();
    t3.start();

    loop.run();
}


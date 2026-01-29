#include <iostream>

#include "generator.h"

generator<int> numbers() {

    for ( int i = 0; i < 5; ++i ) {
        co_yield i;
    }
}

int main() {
    
    auto gen = numbers();

    while (gen.next()) {
        std::cout << gen.value() << "\n";
    }
}
#include <cassert>
#include <chrono>

#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include "async_parser.h"


using namespace std::string_literals;
using namespace std::chrono_literals;





async_parse<int64_t, char> parse_string() {

    while ( true ) {
    
        char c = co_await char{ };
    
        int64_t number {};
        int64_t sign {1};

        if (c != '-' && c != '+' && !std::isdigit(c) ) {
            continue;
        }

        if (c == '-') {
            sign = -1;
        }
        else if ( std::isdigit(c) ) {
            number = number * 10 + c - '0';
        }

        while (true) {

            c = co_await char{};
            
            if ( std::isdigit(c) ) {
            
                number = number * 10 + c - '0';
            }
            else {
                break;
            }
        }

        co_yield number * sign;
    }
}

int64_t parse_string(const std::string& str) {

    int64_t num { 0 };
    int64_t sign { 1 };

    std::size_t c = 0;
    
    while ( c < str.size() ) {
    
        if ( str[c] == '-' ) {
            sign = -1;
        }
        else if ( std::isdigit( str[c] ) ) {
            
            num = num * 10ll + ( str[c] - '0' );
        }
        else if ( str[c] == '#' ) {
            
            break;
        }
        ++c;
    }

    return num * sign;
}

int main() {

    std::string num1 = "-123454321#"s;
    std::string num2 = "-123454321#98765#-20241337#+31415#"s;

    std::cout << parse_string(num1) << std::endl;


    std::size_t sz = num1.size();
    async_parse<int64_t, char> num_parser = parse_string();
    
    for (std::size_t i = 0; i < sz / 2; ++i) {

        num_parser.put( num1[i] );
    }

    std::cout << "parsing number...\n"s;

    for (std::size_t i = sz / 2; i < sz; ++i) {
        
        num_parser.put(num1[i]);
    }

    std::cout << num_parser.get() << std::endl;


    for ( char c : num2 ) {

        num_parser.put( c );

        std::this_thread::sleep_for(1s);

        if ( c == '#' ) {

            std::cout << num_parser.get() << std::endl;
        }
    }

    return 0;
}
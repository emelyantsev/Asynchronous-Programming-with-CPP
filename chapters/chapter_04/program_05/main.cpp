#include <exception>
#include <iostream>
#include <mutex>
#include <thread>
#include <syncstream>


#ifndef sync_cout
#define sync_cout std::osyncstream( std::cout )
#endif



int main() {

    std::once_flag run_once_flag;
    std::once_flag run_once_exceptions_flag;

    auto thread_function = [&] { 

        std::call_once( run_once_flag, [] { 
            sync_cout << "This must run just once\n"; 
        }); 

        sync_cout << "This is common part\n"; 
    };

    std::jthread t1(thread_function);
    std::jthread t2(thread_function);
    std::jthread t3(thread_function);

    auto function_throws = [&](bool throw_exception) {
        
        if (throw_exception) {
        
            sync_cout << "Throwing exception\n";
            throw std::runtime_error("runtime error");
        }

        sync_cout << "No exception was thrown\n";
    };

    auto thread_function_1 = [&](bool throw_exception) {
        
        try {

            std::call_once( run_once_exceptions_flag, function_throws, throw_exception) ;
        } 
        catch (...) {
            
            sync_cout << "caught exception" << std::endl;
        }
    };

    std::jthread t4(thread_function_1, true);
    std::jthread t5(thread_function_1, true);
    std::jthread t6(thread_function_1, false);

    return 0;
}
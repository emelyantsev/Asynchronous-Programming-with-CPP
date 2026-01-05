#pragma once 

#include <functional>
#include <iostream>
#include <syncstream>
#include <thread>

#ifndef sync_cout
#define sync_cout std::osyncstream(std::cout)
#endif

using namespace std::chrono_literals;

class JthreadWrapper {

    public:

        JthreadWrapper(const std::function<void(const std::string& )>& func, const std::string& str) : t(func, str), name(str) {

            sync_cout << "Thread " << name << " being created" << std::endl;
        }

        ~JthreadWrapper() {
            
            sync_cout << "Thread " << name << " being destroyed" << std::endl;
        }

    private:
        std::jthread t;
        std::string name;
};
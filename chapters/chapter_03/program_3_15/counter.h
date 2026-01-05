#include <chrono>
#include <functional>
#include <iostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

class Counter {

        using Callback = std::function<void(void)>;

    public:

        Counter(const Callback &callback) ;

        void stop() ;

    private:
        std::jthread t;
        std::atomic_bool running{true};
};
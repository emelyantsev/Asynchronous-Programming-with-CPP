#include "counter.h"

Counter::Counter(const Callback &callback) {
    
    sync_cout << "Counter: Starting..." << std::endl;

    t = std::jthread([&]() {
    
        while (running.load() == true) {
    
            sync_cout << "Counter: Running callback..." << std::endl;
            callback();
            std::this_thread::sleep_for(1s);
        }
    
        sync_cout << "Counter: Exit" << std::endl;
    } );
}

void Counter::stop() { running.store(false); }
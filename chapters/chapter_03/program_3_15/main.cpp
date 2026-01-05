#include "counter.h"


int main() {

    // Create timer executing callback function every 500ms
    sync_cout << "Main: Create Counter\n";
    
    Counter counter( []() { sync_cout << "Callback: Running...\n"; } );

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for(3s);

    // Stop counter and wait for 1 second
    counter.stop();
    
    std::this_thread::sleep_for(1s);

    sync_cout << "Main thread: Exit\n";
    
    return 0;
}
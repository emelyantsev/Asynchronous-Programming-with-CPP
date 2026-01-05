// timer

#include "timer.h"

int main() {

    // Create timer executing callback function every second
    sync_cout << "Main: Create timer\n";
    
    Timer timer( 500ms, [&] () {
        
        sync_cout << "Callback: Running...\n";
    });

    // Wait main thread for 3 seconds
    std::this_thread::sleep_for( 5s );

    
    sync_cout << "Main thread: Stop timer\n";
    timer.stop();

    // Wait main thread for 500ms while timer stops
    std::this_thread::sleep_for( 500ms );
    sync_cout << "Main thread: Exit\n";
    
    return 0;
}
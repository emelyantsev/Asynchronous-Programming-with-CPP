// thread cancellation with stop_token

#include <chrono>
#include <iostream>
#include <string_view>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

template <typename T>
void show_stop_props(std::string_view name, const T& stop_item) {

    sync_cout << std::boolalpha << name << ": stop_possible = " << stop_item.stop_possible() 
        << ", stop_requested = " << stop_item.stop_requested() << '\n';
};


void func_with_stop_token(std::stop_token stop_token) {

    // Worker thread function that listens and acts on stop requests.

    for (int i = 0; i < 10; ++i) {

        std::this_thread::sleep_for( 300ms );

        if ( stop_token.stop_requested() ) {

            sync_cout << "stop_worker: Stopping as requested\n";
            return;
        }

        sync_cout << "stop_worker: Going back to sleep\n";
    }
}


int main() {


    auto worker1 = std::jthread( func_with_stop_token );
    std::stop_token stop_token = worker1.get_stop_token();
    
    show_stop_props("stop_token", stop_token);


    std::stop_callback callback( worker1.get_stop_token(), []{
        
        sync_cout << "stop_callback for worker1 executed by thread: " << std::this_thread::get_id() << '\n';
    });

    sync_cout << "main_thread: Request stop and join worker1\n";
    worker1.request_stop();
    worker1.join();

    show_stop_props("stop_token after request", stop_token);

}
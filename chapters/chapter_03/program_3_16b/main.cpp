#include <chrono>
#include <iostream>
#include <string_view>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

using namespace std::chrono_literals;

template <typename T>
void show_stop_props(std::string_view name, const T& stop_item) {
    sync_cout << std::boolalpha
              << name << ": stop_possible = " << stop_item.stop_possible()
              << ", stop_requested = " << stop_item.stop_requested() << '\n';
};

void func_with_stop_token(std::stop_token stop_token) {
    // Worker thread function that listens and acts on stop requests.

    sync_cout << "Started function func_with_stop_token " << std::this_thread::get_id() << '\n';

    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(300ms);
        if (stop_token.stop_requested()) {
            sync_cout << "stop_worker: Stopping as requested\n";
            return;
        }
        sync_cout << "stop_worker: Going back to sleep\n";
    }
}


int main() {

    // worker2 will be requested to stop from a stopper thread from source.
    auto worker2 = std::jthread(func_with_stop_token);
    std::stop_source stop_source = worker2.get_stop_source();
    show_stop_props("stop_source", stop_source);

    // Stop_callback objects can be destroyed prematurely to prevent execution.
    // This scoped stop_callback will not execute.
    {
        std::stop_callback scoped_callback(worker2.get_stop_token(), []{
            sync_cout << "stop_callback: Scoped stop callback will not execute\n";
        });
    }


    // Worker2: Request stop from stopper thread via stop_source
    sync_cout << "main_thread: Pass source to stopper thread\n";
    auto stopper = std::thread( [](std::stop_source source) {
            std::this_thread::sleep_for(500ms);
            sync_cout << "stopper: Request stop for worker2 via source\n";
            source.request_stop();
        }, stop_source);

    stopper.join();
    std::this_thread::sleep_for(200ms);
    show_stop_props("stop_source after request", stop_source);

    // After a stop has already been requested, a new stop_callback executes immediately.
    sync_cout << "main_thread: " << std::this_thread::get_id() << '\n';

    std::stop_callback callback_after_stop( worker2.get_stop_token(), [] {
        sync_cout << "stop_callback for worker2 executed by thread: " << std::this_thread::get_id() << '\n';
    });
}
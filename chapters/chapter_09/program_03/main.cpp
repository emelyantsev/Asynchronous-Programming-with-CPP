#include <boost/asio.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <csignal>

using namespace std::chrono_literals;

int main() {

    boost::asio::io_context io_context;
    boost::asio::steady_timer timer( io_context, 3s );
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    std::function< void (const boost::system::error_code& )> timer_handler;
    
    timer_handler = [ &timer, &timer_handler ]( const boost::system::error_code& ec ) {

        if ( ec == boost::asio::error::operation_aborted) {
            
            std::cout << "Timer cancelled due to shutdown." << std::endl;
            return;
        }

    
        if ( !ec ) {
    
            std::cout << "Handler: Timer expired." << std::endl;
            timer.expires_after(1s);
            timer.async_wait(timer_handler);
        } 
        else {
    
            std::cerr << "Handler error: " << ec.message() << std::endl;
        }
    };

    signals.async_wait([&](const boost::system::error_code& ec, int signal_number) {

        if (!ec) {

            std::cout << "\nReceived signal " << signal_number 
                      << " (Ctrl+C). Initiating graceful shutdown..." << std::endl;
            
            timer.cancel();  
            
            std::cout << "Cleanup completed. Exiting..." << std::endl;
        }
    });


    
    timer.async_wait(timer_handler);
    
    io_context.run();
    
    return 0;
}
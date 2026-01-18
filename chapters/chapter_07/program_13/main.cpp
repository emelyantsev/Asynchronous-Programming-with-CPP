// changed inner type to std::shared_futire<void> !!!

#include <future>
#include <iostream>
#include <syncstream>
#include <vector>
#include <cassert>

#include "common.h"


template <typename Func>
class Task {

    public:

        Task(int id, Func& func) : id_(id), func_(func), has_dependency_(false) {

            sync_cout << "Task " << id << " constructed without dependencies.\n";
            
            fut_ = std::async( std::launch::deferred, [this] () { (*this)(); } );
        }

    template <typename... Futures>
    Task(int id, Func& func, Futures&&... futures) : id_(id), func_(func), has_dependency_(true) {

        sync_cout << "Task " << id << " constructed with dependencies.\n";

        fut_ = std::async(std::launch::deferred, [this] () { (*this) (); });
        
        add_dependencies( std::forward<Futures>(futures)... );
    }

    std::shared_future<void> get_dependency() {

        assert ( fut_.valid() ) ;

        sync_cout << "Getting future from task " << id_ << std::endl;
        return fut_;
    }

    void operator()() {

        sync_cout << "Starting task " << id_ << std::endl;
        wait_completion();
        sync_cout << "Running task " << id_ << std::endl;
        func_();
    }

    void start() {

        fut_.get();
    }

    private:

        template <typename... Futures>
        void add_dependencies(Futures&&... futures) {
 
            ( deps_.push_back(futures) , ... );
        }

        void wait_completion() {

            sync_cout << "Waiting completion for task " << id_ << std::endl;

            if ( !deps_.empty() ) {
            
                for ( auto& fut : deps_ ) {
            
                    if ( fut.valid() ) {
            
                        sync_cout << "Fut valid so getting value in task " << id_ << std::endl;
                        fut.get();
                    }
                }
            }
        }

    private:

        int id_;        
        Func& func_;
        std::shared_future<void> fut_;
        std::vector<std::shared_future<void>> deps_;
        bool has_dependency_;        
};

int main() {


    auto sleep1s = []() { std::this_thread::sleep_for(1s); };
    auto sleep2s = []() { std::this_thread::sleep_for(2s); };

    Task task1(1, sleep1s);
    Task task2(2, sleep2s, task1.get_dependency() );
    Task task3(3, sleep1s, task2.get_dependency() );
    Task task4(4, sleep2s, task2.get_dependency() );
    Task task5(5, sleep2s, task3.get_dependency(), task4.get_dependency());    


    sync_cout << "Starting the pipeline..." << std::endl;
    
    task1.start();
    task5.start();

    sync_cout << "All done!" << std::endl;
    return 0;
}
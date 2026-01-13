#pragma once

#include <future>
#include <iostream>
#include <syncstream>
#include <vector>

using namespace std::chrono_literals;

#ifndef sync_cout
#define sync_cout std::osyncstream(std::cout)
#endif

template <typename Func>
class Task {
   
    public:

        Task(int id, Func& func) : id_(id), func_(func), has_dependency_(false) {

            sync_cout << "Task " << id << " constructed without dependencies." << std::endl;
            fut_ = prom_.get_future().share();
        }


        template <typename... Futures>
        Task(int id, Func& func, Futures&&... futures) : id_(id), func_(func), has_dependency_(true) {

            sync_cout << "Task " << id << " constructed with dependencies." << std::endl;            
            fut_ = prom_.get_future().share();            
            add_dependencies( std::forward<Futures>(futures)... );
        }


        std::shared_future<void> get_dependency() {
            
            sync_cout << "Getting future from task " << id_ << std::endl;
            return fut_;
        }

        
        void operator()() {

            sync_cout << "Running task " << id_ << std::endl;

            wait_completion();
            func_();

            sync_cout << "Signaling completion of task " << id_ << std::endl;
            prom_.set_value();
        }

    private:

        template <typename... Futures>
        void add_dependencies(Futures&& ... futures) {

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

        std::promise<void> prom_;                     // Signal task completion
        std::shared_future<void> fut_;                // Own future to share with dependant tasks
        std::vector<std::shared_future<void>> deps_;  // Wait for tasks that this task depends on for completion

        bool has_dependency_;
};
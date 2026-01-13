// pipeline

#include "task.h"

int main() {

    
    auto sleep1s = []() { std::this_thread::sleep_for(1s); };
    auto sleep2s = []() { std::this_thread::sleep_for(2s); };

    
    Task task1( 1, sleep1s );
    Task task2( 2, sleep2s, task1.get_dependency() );
    Task task3( 3, sleep1s, task2.get_dependency() );
    Task task4( 4, sleep2s, task2.get_dependency() );
    Task task5( 5, sleep2s, task3.get_dependency(), task4.get_dependency() );    

    sync_cout << "Starting the pipeline..." << std::endl;
    
    task1();
    task2();
    task3();
    task4();
    task5();

    sync_cout << "Waiting for the pipeline to finish..." << std::endl;    

    auto finish_pipeline_fut = task5.get_dependency();
    finish_pipeline_fut.get();

    sync_cout << "All done!" << std::endl;

    return 0;
}
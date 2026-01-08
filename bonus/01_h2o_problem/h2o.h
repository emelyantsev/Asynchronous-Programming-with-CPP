// https://leetcode.com/problems/building-h2o/description/

#include <mutex>
#include <condition_variable>
#include <functional>
#include <cassert>

class H2O {

    public:

        H2O() {}

        void hydrogen( std::function<void()> releaseHydrogen ) {

            std::unique_lock<std::mutex> lock(mtx);
            
            cv.wait( lock, [this] { return hydrogen_count < 2; } );

            // assert( lock.owns_lock() ) ;

            releaseHydrogen();
            
            hydrogen_count++;
            
            if ( hydrogen_count == 2 ) {

                cv.notify_all();
            }
        }

        void oxygen(std::function<void()> releaseOxygen) {

            std::unique_lock<std::mutex> lock( mtx );
            
            cv.wait(lock, [this] { return hydrogen_count == 2; });

            // assert( lock.owns_lock() ) ;

            releaseOxygen();           
            hydrogen_count = 0;           
            cv.notify_all();
        }

    private:

        std::mutex mtx;
        std::condition_variable cv;
        int hydrogen_count = 0; 

};
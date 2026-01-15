#include <semaphore>
#include <functional>

class H2O {

    private:
        
        std::counting_semaphore<2> sem_h{2}; 
        std::counting_semaphore<1> sem_o{1};

        std::counting_semaphore<3> sem_hydro_done{0};
        std::counting_semaphore<3> sem_oxy_done{0};

    public:

        void hydrogen(std::function<void()> releaseHydrogen) {

            sem_h.acquire(); 
            
            releaseHydrogen();
            
            sem_hydro_done.release(); 
            sem_oxy_done.acquire();   
            sem_h.release();        
        }

        void oxygen(std::function<void()> releaseOxygen) {

            sem_o.acquire(); 
            
            sem_hydro_done.acquire();
            sem_hydro_done.acquire();
            
            releaseOxygen();
            
            sem_oxy_done.release(2); 
            sem_o.release(); 
        }
};
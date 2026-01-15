#include <functional>
#include <mutex>
#include <condition_variable>

class DiningPhilosophers {

public:

    DiningPhilosophers() {}

    void wantsToEat(int philosopher,
                    std::function<void()> pickLeftFork,
                    std::function<void()> pickRightFork,
                    std::function<void()> eat,
                    std::function<void()> putLeftFork,
                    std::function<void()> putRightFork ) {

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait( lock, [this, &philosopher] { return id == philosopher ; } );

        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();

        id = (id + 1) % 5;
        cv.notify_all();
    }

private:

    std::mutex mtx;
    std::condition_variable cv;
    int id = 0; 

};
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
                    std::function<void()> putRightFork) {

        int id = philosopher;                            

        std::scoped_lock lock(mtx[id % 5], mtx[ (id + 1) % 5]);

        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();

    }

private:

    std::mutex mtx[5];

};
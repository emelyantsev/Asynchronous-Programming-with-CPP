// // atomic_type_not_lock-free.

#include <atomic>
#include <iostream>

struct no_lock_free {

    int a[128];

    no_lock_free() {
    
        for (int i = 0; i < 128; ++i) {
            a[i] = i;
        }
    }
};

int main() {

    std::atomic<no_lock_free> s;

    std::cout << "Size of no_lock_free: " << sizeof(no_lock_free) << " bytes\n";
    std::cout << "Size of std::atomic<no_lock_free>: " << sizeof(s) << " bytes\n";

    std::cout << "Is std::atomic<no_lock_free> always lock-free: " << std::boolalpha
              << std::atomic<no_lock_free>::is_always_lock_free << std::endl;

    std::cout << "Is std::atomic<no_lock_free> lock-free: " << std::boolalpha << s.is_lock_free() << std::endl;

    no_lock_free s1;
    s1.a[3] = 0;
    s1.a[117] = -1;

    s.store(s1);

    return 0;
}


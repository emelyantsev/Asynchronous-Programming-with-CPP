#pragma once

#include <concepts>


template <typename T>
concept Releasable = requires(T t) {
    { t.acquire() } -> std::same_as<void>;
    { t.release() } -> std::same_as<void>;
};

template <Releasable T>
class MovableSemaphoreGuard {
    
    public:

        explicit MovableSemaphoreGuard(T& sem) : sem_(&sem) {
            sem_->acquire(); 
        }

        MovableSemaphoreGuard(MovableSemaphoreGuard&& other) noexcept 
            : sem_(std::exchange(other.sem_, nullptr)) {}

        MovableSemaphoreGuard(const MovableSemaphoreGuard&) = delete;

        MovableSemaphoreGuard& operator=(const MovableSemaphoreGuard&) = delete;

        ~MovableSemaphoreGuard() {
        
            if (sem_) {
                sem_->release();
            }
        }

        void reset() {

            if (sem_) {
                sem_->release();
                sem_ = nullptr;
            }
        }

    private:

        T* sem_ = nullptr;
};
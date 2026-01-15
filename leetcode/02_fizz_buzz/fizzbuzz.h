// https://leetcode.com/problems/fizz-buzz-multithreaded/description/

#include <functional>
#include <barrier>

class FizzBuzz {


    public:

        FizzBuzz(int n) : n(n), bar(4) {}

        void fizz(std::function<void()> printFizz) {
            
            for (int i = 1; i <= n ; ++i) {

                if (i % 3 == 0 && i % 5 != 0) {
                    printFizz();
                }

                bar.arrive_and_wait();
            }

        }


        void buzz(std::function<void()> printBuzz) {

            for (int i = 1; i <= n ; ++i) {

                if (i % 3 != 0 && i % 5 == 0) {
                    printBuzz();
                }

                bar.arrive_and_wait();
            }
            
        }

        void fizzbuzz(std::function<void()> printFizzBuzz) {
            
            for (int i = 1; i <= n ; ++i) {

                if (i % 3 == 0 && i % 5 == 0) {
                    printFizzBuzz();
                }

                bar.arrive_and_wait();
            }
        }

        void number(std::function<void(int)> printNumber) {

            for (int i = 1; i <= n ; ++i) {

                if (i % 3 != 0 && i % 5 != 0) {
                    printNumber(i);
                }

                bar.arrive_and_wait();
            }
        }

    private:
        
        int n;
        std::barrier<> bar;
};
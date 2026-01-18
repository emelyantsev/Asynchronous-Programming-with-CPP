#include <algorithm>
#include <forward_list>
#include <future>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

#include "common.h"

template <typename C>
void print_address_of( const C& container) {

    sync_cout << "\"" << typeid( container ).name() << "\"" << " " << std::addressof(container) << std::endl;
}


template <typename C>
bool search(const C& container, const std::string& target) {

    print_address_of(container);
    return std::find( container.begin(), container.end(), target ) != container.end();
}



int main() {

    std::vector<std::string> africanAnimals = {"elephant", "giraffe", "lion", "zebra"};
    std::list<std::string> americanAnimals = {"alligator", "bear", "eagle", "puma"};
    std::forward_list<std::string> asianAnimals = {"orangutan", "panda", "tapir", "tiger"};
    std::set<std::string> europeanAnimals = {"deer", "hedgehog", "linx", "wolf"};

    std::string target = "elephant";

    // Launch asynchronous searches on different containers
    auto fut1 = std::async( std::launch::async, search<std::vector<std::string>>, std::ref( africanAnimals ), target);
    auto fut2 = std::async( std::launch::async, search<std::list<std::string>>, std::ref(americanAnimals), target);
    auto fut3 = std::async( std::launch::async, search<std::forward_list<std::string>>, std::ref(asianAnimals), target);
    auto fut4 = std::async( std::launch::async, search<std::set<std::string>>, std::ref(europeanAnimals), target);

    print_address_of(africanAnimals);
    print_address_of(americanAnimals);
    print_address_of(asianAnimals);
    print_address_of(europeanAnimals);


    // Collect results
    bool found = fut1.get() || fut2.get() || fut3.get() || fut4.get();

    if (found) {
        sync_cout << target << " found in one of the containers.\n";
    } else {
        sync_cout << target << " not found in any of the containers.\n";
    }

    return 0;
}
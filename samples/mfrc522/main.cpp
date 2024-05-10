//
// Created by chiheb on 06/05/24.
//
#include <iostream>
#include "pitools.hpp"
using namespace std::chrono_literals;

int main() {
    gpioInitialise();
    pitools::sensors::HC04 p{23,24}; // Trig->GPIO23, Echo->GPIO24
    while (1) {
        auto temp = p.getData();
        std::cout << "Distance: "<<temp<< std::endl;
        std::this_thread::sleep_for(1s);
    }
    return 0;
}
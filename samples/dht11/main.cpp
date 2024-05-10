//
// Created by chiheb on 06/05/24.
//
#include <iostream>
#include "pitools.hpp"
using namespace std::chrono_literals;

int main() {
    gpioInitialise();
     pitools::sensors::DHT11 p {16}; // DHT11 data bin assumed to be connected GPIO16
     while (1) {
         auto temp = p.getData();
         std::cout <<"Temperature: "<<temp.temperature << std::endl;
         std::cout << "Humidity: "<<temp.humidity << std::endl;
         std::this_thread::sleep_for(1s);
     }
    return 0;
}
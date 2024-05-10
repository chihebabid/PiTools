//
// Created by chiheb on 06/05/24.
//
#include <iostream>
#include "pitools.hpp"
using namespace std::chrono_literals;

int main() {
    gpioInitialise();
    pitools::actuators::Servo s{13}; // Signal -> GPIO13

    while (1) {
        //  Rotate from 0 degrees to 180 degrees
        for (auto i{0};i<180;++i) {
            s.setAngle(i);
            std::this_thread::sleep_for(10ms);
        }
        std::this_thread::sleep_for(500ms);

        // Reverse the direction to make the servo rotate from 180 degrees to 0 degrees
        for (auto i{180};i>0;--i) {
            s.setAngle(i);
            std::this_thread::sleep_for(10ms);
        }
        std::this_thread::sleep_for(500ms);
    }

    return 0;
}
//
// Created by chiheb on 06/05/24.
//
#include <iostream>
#include "pitools.hpp"
using namespace std::chrono_literals;

int main() {
    gpioInitialise();
    pitools::actuators::StepperMotor28BYJ48 motor {{18,23,24,25}}; // A->GPIO18, B->GPIO23, C->GPIO24, D->GPIO25
    while (1) {
        motor.rotateClockAngle(360).waitFor(1s).rotateAntiClockAngle(360).waitFor(1s);
    }
    return 0;
}
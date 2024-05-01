//
// Created by chiheb on 20/04/24.
//
#include <iostream>
#include <pigpio.h>
#include "servo.hpp"
#include "defs.hpp"
namespace pitools {
    namespace actuators {
        Servo::Servo(const uint8_t pin, servo_param_t param): mPin(pin), mParam(param) {
            pitools::use_pins({pin});
            init();
        }

        void Servo::init() {
            gpioSetMode(mPin,PI_OUTPUT);
        }

        uint8_t Servo::getAngle() const {
            return mAngle;
        }

        void Servo::setAngle(const uint8_t angle) {
            mAngle=angle;
            auto pulse_width {pitools::map(mAngle,mParam.mAngleLow,mParam.mAngleHigh,mParam.mServoLow ,mParam.mServoMax)};
            gpioServo(mPin,pulse_width);
        }
    }
}
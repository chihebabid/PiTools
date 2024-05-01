//
// Created by chiheb on 20/04/24.
//

#ifndef SMARTHOME_SERVO_HPP
#define SMARTHOME_SERVO_HPP
#include "actuator.hpp"
#include "defs.hpp"
#include <cstdint>
namespace pitools {
    namespace actuators {
        struct servo_param_t {
            long mAngleLow {0L},mAngleHigh {180L},mServoLow {500L},mServoMax {2500L};
        };
        class Servo : public Actuator {
            uint8_t mPin;
            uint8_t mAngle;
            servo_param_t mParam;
            void init() override;
        public:
            Servo(const uint8_t pin, servo_param_t t=servo_param_t{});
            uint8_t getAngle() const;
            void setAngle(const uint8_t angle);
        };
    }
}

#endif //SMARTHOME_SERVO_HPP

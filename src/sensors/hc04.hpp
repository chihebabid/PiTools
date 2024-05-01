//
// Created by chiheb on 19/04/24.
//

#ifndef SMARTHOME_HC04_HPP
#define SMARTHOME_HC04_HPP
#include "pigpio.h"
#include "pitools.hpp"
#include "sensor.hpp"

namespace pitools {
    namespace sensors {
        class HC04 : public Sensor<float> {
            pitools::gpio::GPIOOutput mTrig;
            pitools::gpio::GPIOInput mEcho;

            void init() override;

            uint32_t pulseIn();

        public:
            HC04(const uint8_t trig, const uint8_t echo);
            float getData() override;
        };
    }
}
#endif //SMARTHOME_HC04_HPP

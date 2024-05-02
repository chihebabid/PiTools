//
// Created by chiheb on 25/04/24.
//

#ifndef SMARTHOME_GPIOOUTPUT_HPP
#define SMARTHOME_GPIOOUTPUT_HPP

#include "GPIODevice.hpp"
#include <chrono>
#include <thread>
namespace pitools {
    namespace gpio {
        class GPIOOutput :public GPIODevice {
        public:
            GPIOOutput(const uint8_t &p);

        };


    }
}

#endif //SMARTHOME_GPIOOUTPUT_HPP

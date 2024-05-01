//
// Created by chiheb on 25/04/24.
//

#include "GPIOOutput.hpp"

namespace pitools {
    namespace gpio {
        GPIOOutput::GPIOOutput(const uint8_t &p) : GPIODevice(p) {
            mode(GPIOMODE::OUTPUT);
        }
    }
}
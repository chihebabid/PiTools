//
// Created by chiheb on 25/04/24.
//
#include "GPIODevice.hpp"
#include "defs.hpp"
#include <pigpio.h>
namespace pitools {
    namespace gpio {
        GPIODevice::GPIODevice(const uint8_t &p):mPin(p)  {

        }
        GPIODevice& GPIODevice::mode(const pitools::gpio::GPIOMODE &m) {
            gpioSetMode(mPin,static_cast<int>(m));
            return *this;
        }

        GPIOSTATE GPIODevice::state() const {
            return static_cast<GPIOSTATE>(gpioRead(mPin));
        }

        GPIODevice&  GPIODevice::set(const bool &b) {
            gpioWrite(mPin,b);
            return *this;
        }
        GPIODevice& GPIODevice::clear() {
            gpioWrite(mPin,0);
            return *this;
        }

        GPIODevice& GPIODevice::use() {
            pitools::use_pins({mPin});
            return *this;
        }
        GPIODevice& GPIODevice::unuse() {
            return *this;
        }

        uint8_t GPIODevice::getPin() const {
            return mPin;
        }



    }
}
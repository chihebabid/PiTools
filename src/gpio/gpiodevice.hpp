//
// Created by chiheb on 25/04/24.
//

#ifndef SMARTHOME_GPIODEVICE_HPP
#define SMARTHOME_GPIODEVICE_HPP
#include <cstdint>
#include <chrono>
#include <thread>
#include "timers/WaitDuration.hpp"

namespace pitools {
    namespace gpio {
       enum class GPIOMODE {
           INPUT,
           OUTPUT,
           ALT0,
           ALT1,
           ALT2,
           ALT3,
           ALT4,
           ALT5
       };
        enum class GPIOSTATE {
            LOW,
            HIGH
        };

        enum class GPIOPUDUP {
            PUD_OFF,
            PUD_DOWN,
            PUD_UP
        };

    class GPIODevice : public pitools::timers::WaitDuration<GPIODevice> {
           GPIODevice(const GPIODevice&);
       protected:
           uint8_t mPin;
       public:
           GPIODevice(const uint8_t &) ;
           uint8_t getPin() const;
           GPIODevice &mode(const GPIOMODE &m);
           GPIOSTATE state() const;
           GPIODevice&  set(const bool &b=1);
           GPIODevice& clear();
           GPIODevice& use();
           GPIODevice& unuse();
       };
    }
}
#endif //SMARTHOME_GPIODEVICE_HPP

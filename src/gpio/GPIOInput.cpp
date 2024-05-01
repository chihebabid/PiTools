//
// Created by chiheb on 25/04/24.
//
#include <stdexcept>
#include "GPIOInput.hpp"
#include <pigpio.h>
namespace pitools {
    namespace gpio {
        GPIOInput::GPIOInput(const uint8_t &p) : GPIODevice(p) {
            mode(GPIOMODE::INPUT);
        }
        std::shared_ptr<GPIOInput> GPIOInput::create(const uint8_t &p) {
            return std::make_shared<GPIOInput>(p);
        }

        GPIOInput& GPIOInput::pullup() {
            gpioSetPullUpDown(mPin,PI_PUD_UP);
            return *this;
        }

        GPIOInput& GPIOInput::pulldown() {
            gpioSetPullUpDown(mPin,PI_PUD_DOWN);
            return *this;
        }

        GPIOInput& GPIOInput::pulloff() {
            gpioSetPullUpDown(mPin,PI_PUD_OFF);
            return *this;
        }


        GPIOInput& GPIOInput::waitUntil(const GPIOSTATE& state, uint32_t& duration,uint32_t timeout) {
            auto start{gpioTick()};
            if (timeout!=0) {
                while (gpioRead(mPin)!=static_cast<int>(state)) {
                    if (gpioTick()-start>=timeout) throw std::runtime_error("timeout");;
                    std::this_thread::yield();
                }
            }
            else {
                while (gpioRead(mPin)!=static_cast<int>(state)) {
                    std::this_thread::yield();
                }
            }
            duration=gpioTick()-start;
            return *this;
        }
    }
}
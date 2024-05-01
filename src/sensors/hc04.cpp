//
// Created by chiheb on 19/04/24.
//
#include <chrono>
#include <thread>
#include "hc04.hpp"
#include "defs.hpp"
using namespace std::chrono_literals;


namespace pitools {
    namespace sensors {
        constexpr int MAX_DISTANCE=220;
        constexpr int timeOut=MAX_DISTANCE*60;
        HC04::HC04(const uint8_t trig, const uint8_t echo) : mTrig(trig), mEcho(echo) {
            pitools::use_pins({trig,echo});
            init();
        }

        void HC04::init() {
            mTrig.clear();
        }


        uint32_t HC04::pulseIn() {
            uint32_t duration;
            try {
                mEcho.waitUntil(pitools::gpio::GPIOSTATE::HIGH,duration,timeOut);
                mEcho.waitUntil(pitools::gpio::GPIOSTATE::LOW,duration,timeOut);
            }
            catch(...) {
                return timeOut;
            }
            return duration;
        }

        float HC04::getData() {
            float distance;
            mTrig.set().waitFor(10us).clear();
            auto pingTime = pulseIn();
            distance = (float) pingTime * (340.0 * 100.0) / 2.0 / 1000000.0; // the sound speed is 340m/s
            return distance;
        }
    } // end namespace sensors
} // end namespace pitools

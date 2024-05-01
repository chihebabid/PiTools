/*
 * C74595Exp.h
 *
 *  Created on: 16 avr. 2021
 *      Author: chiheb
 */

#ifndef CHC595EXP_H_
#define CHC595EXP_H_
#include <cstdint>
#include <bit>
#include "pitools.hpp"

namespace pitools {
    namespace actuators {
        class HC595 : public Actuator {
        private:
            pitools::gpio::GPIOOutput _sclk, _latch, _sda;
            uint8_t _byte;



            /*
             * Latch data to output
             */
            void update();

            /*
             * shift bits
             */
            void write8Bits();
            void init() override;
        public:
            HC595(uint8_t sclk, uint8_t latch, uint8_t sda);

            /*
             * Write a byte to parallel output
             */
            HC595& writeByte(uint8_t b);

            HC595 &operator<<(uint8_t b);

            HC595 &operator<<(const char *b);
            HC595& writeBit(const bool &b);
            //HC595 &shl(const bool &b);
        };
    }
}
#endif /* CHC595EXP_H_ */

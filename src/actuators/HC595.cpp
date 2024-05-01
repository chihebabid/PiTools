/*
 * CHc595Exp.cpp
 *
 *  Created on: 16 avr. 2021
 *      Author: chiheb
 */

#include "HC595.h"
#include "pigpio.h"
#include <cstdint>
#include <chrono>
#include <thread>
#include <ostream>
#include <cstring>
#include "defs.hpp"
using namespace std::chrono_literals;
namespace pitools {
    namespace actuators {
        HC595::HC595(uint8_t sclk, uint8_t latch, uint8_t sda) : _sclk(sclk), _latch(latch), _sda(sda), _byte(0) {
            pitools::use_pins({sclk,latch,sda});
        }

        void HC595::init() {
            _sclk.clear();
            _latch.clear().waitFor(1us);
        }

        void HC595::update() {
            _latch.set().waitFor(1ms);
        }

        void HC595::write8Bits() {
            uint8_t bit;
            uint8_t new_byte {_byte};
            for (int i{0}; i < 8; ++i) {
                bit = !!(new_byte & 0x80);
                new_byte = new_byte << 1;
                _sda.set(bit).waitFor(1us);
                _sclk.set().waitFor(1us).clear().waitFor(1us);
            }
        }

        HC595& HC595::writeByte(uint8_t b) {
            init();
            _byte = b;
            write8Bits();
            update();
            return *this;
        }

        HC595 &HC595::operator<<(uint8_t b) {
            writeByte(b);
            return *this;
        }

        HC595 &HC595::operator<<(const char *b) {
            init();
            for (size_t i = 0; i < strlen(b); i++) {
                _byte = b[i];
                write8Bits();
            }
            update();
            return *this;
        }




        HC595& HC595::writeBit(const bool &b) {
            init();
            _byte = _byte << 1;
            _byte|=b;
            _sda.set(b).waitFor(1us);
            _sclk.set().waitFor(1us).clear().waitFor(1us);
            update();
            return *this;
        }

    }
}
//
// Created by chiheb on 23/04/24.
//
#include <stdexcept>
#include <array>
#include "defs.hpp"
namespace pitools{
    long map(long value,long fromLow,long fromHigh,long toLow,long toHigh) {
        return (toHigh-toLow)*(value-fromLow) / (fromHigh-fromLow) + toLow;
    }

    void use_pins(std::initializer_list<uint8_t> l,const GPIOUSE&type) {
        static std::array<GPIOUSE, 53> lpins {};
        for (const auto& pin : l) {
            if (lpins[pin]==GPIOUSE::NOTYET &&(lpins[pin]!=GPIOUSE::IO && lpins[pin]==type)) lpins[pin] = type;
            else throw std::runtime_error("Pin can't used with different configurations...");

        }
    }
}

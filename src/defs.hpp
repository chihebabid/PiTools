//
// Created by chiheb on 23/04/24.
//

#ifndef SMARTHOME_DEFS_HPP
#define SMARTHOME_DEFS_HPP
#include <initializer_list>
#include <cstdint>
namespace pitools{
    enum class GPIOUSE {
        NOTYET,
        IO,
        I2C,
        SPI
    };

    long map(long value,long fromLow,long fromHigh,long toLow,long toHigh);
    void use_pins(std::initializer_list<uint8_t> l,const GPIOUSE&type=GPIOUSE::IO);
}
#endif //SMARTHOME_DEFS_HPP

//
// Created by chiheb on 23/04/24.
//

#ifndef SMARTHOME_DEFS_HPP
#define SMARTHOME_DEFS_HPP
#include <initializer_list>
#include <cstdint>
namespace pitools{
    long map(long value,long fromLow,long fromHigh,long toLow,long toHigh);
    void use_pins(std::initializer_list<uint8_t> l);
}
#endif //SMARTHOME_DEFS_HPP

//
// Created by chiheb on 24/04/24.
//

#ifndef SMARTHOME_I2CDEVICE_HPP
#define SMARTHOME_I2CDEVICE_HPP
#include <cstdint>
#include "defs.hpp"
namespace pitools {
    namespace i2cdevices {
        class I2CDevice {
            virtual void init() = 0;
        protected:
            int16_t mI2cHandle;
        public:
            I2CDevice(const uint8_t addr, const uint8_t bus = 0);

            virtual ~I2CDevice();
        };
    }
}

#endif //SMARTHOME_I2CDEVICE_HPP

//
// Created by chiheb on 24/04/24.
//
#include <pigpio.h>
#include "I2CDevice.hpp"
namespace pitools {
    namespace i2cdevices {

        I2CDevice::I2CDevice(const uint8_t addr, const uint8_t bus ) {
            /*if (bus==0) {
                pitools::use_pins({0, 1});
            }
            else {
                pitools::use_pins({2, 3});
            }*/
            mI2cHandle = i2cOpen(addr, bus, 0);

        }
         I2CDevice::~I2CDevice() {
            if (mI2cHandle>=0) {
                i2cClose(mI2cHandle);
            }
        }

    }
}
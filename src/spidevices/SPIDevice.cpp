//
// Created by chiheb on 24/04/24.
//
#include <pigpio.h>
#include "SPIDevice.hpp"

namespace pitools {
    namespace spidevices {
        SPIDevice::SPIDevice(uint8_t channel,uint32_t speed,SPI_MODE mode,bool active_low) {
            uint16_t flags {0b0000000000000000};
            flags|=static_cast<uint8_t>(mode);
            if (!active_low) flags|=0b11100;
            mHSpi= spiOpen(channel,speed,flags);
        }

        SPIDevice::~SPIDevice() {
            if (mHSpi>=0) {
                spiClose(mHSpi);
            }
        }
    }
}
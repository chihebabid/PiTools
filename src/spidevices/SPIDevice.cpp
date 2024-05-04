//
// Created by chiheb on 24/04/24.
//
#include <pigpio.h>
#include "SPIDevice.hpp"

namespace pitools {
    namespace spidevices {
        SPIDevice::SPIDevice(const spi_options_t &options) {
            uint16_t flags {0b0000000000000000};
            flags|=static_cast<uint8_t>(options.mode);
            if (!options.active_low) flags|=0b11100;
            mHSpi= spiOpen(options.channel,options.speed,flags);
        }

        SPIDevice::~SPIDevice() {
            if (mHSpi>=0) {
                spiClose(mHSpi);
            }
        }
    }
}
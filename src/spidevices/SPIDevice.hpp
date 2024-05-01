//
// Created by chiheb on 24/04/24.
//

#ifndef SMARTHOME_SPIDEVICE_HPP
#define SMARTHOME_SPIDEVICE_HPP
#include <cstdint>
namespace pitools {
    namespace spidevices {
        enum class SPI_MODE {
            MODE0,
            MODE1,
            MODE2,
            MODE3
        };
        class SPIDevice {
            virtual void init()=0;
        protected:
            int32_t mHSpi; // Spi handle
        public:
            SPIDevice(uint8_t channel,uint32_t speed,SPI_MODE mode=SPI_MODE::MODE0,bool active_low=true);
            ~SPIDevice();
        };
    }
}

#endif //SMARTHOME_SPIDEVICE_HPP

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
        struct spi_options_t {
            uint8_t channel {0};
            uint32_t speed;
            SPI_MODE mode {SPI_MODE::MODE0};
            bool active_low {true};
        };
        class SPIDevice {
            virtual void init()=0;
        protected:
            int32_t mHSpi; // Spi handle
        public:
            SPIDevice(const spi_options_t &options=spi_options_t{});
            ~SPIDevice();
        };
    }
}

#endif //SMARTHOME_SPIDEVICE_HPP

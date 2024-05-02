//
// Created by chiheb on 25/04/24.
//

#ifndef SMARTHOME_GPIOINPUT_HPP
#define SMARTHOME_GPIOINPUT_HPP
#include <memory>
#include "GPIODevice.hpp"

namespace pitools {
    namespace gpio {
    class GPIOInput : public GPIODevice {
        public:
            GPIOInput(const uint8_t &p);
            static std::shared_ptr<GPIOInput> create(const uint8_t &p);
            GPIOInput& pullup();
            GPIOInput& pulldown();
            GPIOInput& pulloff();
            GPIOInput& waitUntil(const GPIOSTATE&, uint32_t& duration,uint32_t timeout = 0);

        };
    }
}

#endif //SMARTHOME_GPIOINPUT_HPP

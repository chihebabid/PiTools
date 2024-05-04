//
// Created by chiheb on 24/04/24.
//

#ifndef SMARTHOME_MCP9808_HPP
#define SMARTHOME_MCP9808_HPP
#include "I2CDevice.hpp"
namespace pitools {
    namespace i2cdevices {
        enum class MCP9808_ADRESS {
            MCP9808_ADRESS_0 = 0x18,
            MCP9808_ADRESS_1 = 0x19
        };
        class MCP9808 : public I2CDevice {
            void init() override;
        public:
            MCP9808(const MCP9808_ADRESS address=MCP9808_ADRESS::MCP9808_ADRESS_0, const uint8_t bus=0);
            float getData();
        };
    }
}

#endif //SMARTHOME_MCP9808_HPP

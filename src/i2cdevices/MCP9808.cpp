//
// Created by chiheb on 24/04/24.
//
#include <thread>
#include <pigpio.h>
#include "MCP9808.hpp"

namespace pitools {
    namespace i2cdevices {
        MCP9808::MCP9808(const MCP9808_ADRESS address, const uint8_t bus): I2CDevice(static_cast<const uint8_t >(address),bus) {
            init();
        }

        void MCP9808::init() {
            i2cWriteWordData(mI2cHandle,0x01,0);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        float MCP9808::getData() {
            int data=i2cReadWordData(mI2cHandle,0x05);
            int val{((data << 8) | (data >> 8)) & 0x1FFF};
            if (val>4095) val-=8192;
            float cTemp {val * 0.0625F};
            return cTemp;
        }
    }
}
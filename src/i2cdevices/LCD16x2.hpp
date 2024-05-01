//
// Created by chiheb on 23/04/24.
//

#ifndef SMARTHOME_LCD16X2_HPP
#define SMARTHOME_LCD16X2_HPP
#include "I2CDevice.hpp"
#include <cstdint>
#include <cstdarg>
#include <cstdio>

namespace pitools {
    namespace i2cdevices {
        class LCD16x2 : I2CDevice {
        private:
            const uint8_t m_lcdRowOffset[4] = {0x80, 0xC0, 0x14, 0x54};
            int16_t m_i2cHandle;
            bool m_backlight_on;
            uint8_t m_BL;
            uint8_t m_displayFunction; // Mode 4/8bits ; #lines ; font
            uint8_t m_displayControl;
            uint8_t m_displayMode;

            void init() override;

            void write4bits(uint8_t value) const;

            void sendCommand(const uint8_t cmd) const;

            void sendByte(uint8_t lsb, uint8_t msb) const;
        public:
            LCD16x2(const uint8_t bus, uint8_t addr, uint8_t width = 16, bool backlight_on = true);
            ~LCD16x2();
            void setPosition(const uint8_t x, const uint8_t y) const;

            void putChar(const uint8_t bits) const;

            void puts(const char *str);

            void clear() const;

            void goHome() const;

            void enableBacklight(bool backlight_on = true);

            bool getBacklight() const;

            void enableCursor(bool enable = true);

            void enableBlinking(bool enable = true);

            LCD16x2 &operator<<(const char *chaine);

            void scrollDisplayRight(bool right = true);

            void autoScroll(bool enable = true);

            void printf(const char *, ...);
        };
    }
}



#endif //SMARTHOME_LCD16X2_HPP

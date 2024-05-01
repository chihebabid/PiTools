//
// Created by chiheb on 23/04/24.
//
#include <thread>
#include <pigpio.h>

#include "LCD16x2.hpp"

using namespace std::chrono_literals;
namespace pitools {
    namespace i2cdevices {
        enum COMMANDS {
            LCD_CLEAR = 0x01,
            LCD_RETURNHOME = 0x02,
            LCD_ENTRYMODESET = 0x04,
            LCD_DISPLAYCONTROL = 0x08,
            LCD_CURSORSHIFT = 0x10,
            LCD_FUNCTIONSET = 0x20
        };


        enum FUNC_CTRL {
            LCD_8BITMODE = 0x10,
            LCD_2LINE = 0x08,
            LCD_5x10DOTS = 0x04
        };


// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_CURSORON 0x02
#define LCD_BLINKON 0x01


// flags for display entry mode
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_MOVERIGHT 0x04


// flags for backlight control
#define LCD_BACKLIGHT 0x08

        enum I2C_EXPANDER {
            _RS = 0x1,
            _EN = 0x4,
            _B4 = 4
        };


        LCD16x2::LCD16x2(const uint8_t bus, uint8_t addr, uint8_t width, bool backlight_on): I2CDevice(addr,bus) {
            m_backlight_on = backlight_on;
            m_displayFunction = LCD_FUNCTIONSET | LCD_2LINE; // | LCD_4BITMODE;// | LCD_5x8DOTS;
            m_displayControl = LCD_DISPLAYCONTROL | LCD_DISPLAYON;
            m_displayMode = LCD_ENTRYMODESET | LCD_ENTRYLEFT;
            init();
            clear();
        }

        LCD16x2::~LCD16x2() {

        }

/*
 * Initialise the LCD screen
 */
        void LCD16x2::init() {
            write4bits(0x3 << 4);
            std::this_thread::sleep_for(std::chrono::microseconds(4500));
            write4bits(0x3 << 4);
            std::this_thread::sleep_for(std::chrono::microseconds(4500));
            write4bits(0x3 << 4);
            std::this_thread::sleep_for(std::chrono::microseconds(150));
            write4bits(0x2 << 4);
            sendCommand(m_displayFunction);
            sendCommand(m_displayControl);
            sendCommand(m_displayMode);
            goHome();
        }

        void LCD16x2::sendCommand(const uint8_t cmd) const {
            uint8_t MSN = (cmd >> 4) & 0x0F;
            uint8_t LSN = cmd & 0x0F;
            uint8_t MSb = MSN << _B4;
            uint8_t LSb = LSN << _B4;
            sendByte(MSb, LSb);
        }

/*
 * Display a char
 */
        void LCD16x2::putChar(const uint8_t bits) const {
            uint8_t MSN = (bits >> 4) & 0x0F;
            uint8_t LSN = bits & 0x0F;
            uint8_t MSb = (MSN << _B4) | _RS;
            uint8_t LSb = (LSN << _B4) | _RS;
            sendByte(MSb, LSb);
        }

        void LCD16x2::sendByte(uint8_t msb, uint8_t lsb) const {
            if (m_backlight_on) {
                lsb = lsb | LCD_BACKLIGHT;
                msb = msb | LCD_BACKLIGHT;
            }
            write4bits(msb);
            write4bits(lsb);
        }

        void LCD16x2::write4bits(uint8_t value) const {
            i2cWriteByte(m_i2cHandle, value | _EN);
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            i2cWriteByte(m_i2cHandle, value & ~_EN);
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }

/*
 * Move the cursor to a position
 */
        void LCD16x2::setPosition(const uint8_t x, const uint8_t y) const {
            sendCommand(m_lcdRowOffset[y] + x);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

/*
 * Clear the LCD screen
 */
        void LCD16x2::clear() const {
            sendCommand(LCD_CLEAR);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));

        }

/*
 * Write a string
 */
        void LCD16x2::puts(const char *str) {
            while (*str)
                putChar((uint8_t) *(str++));
        }

/*
 * Set the cursor to position 0,0
 */
        void LCD16x2::goHome() const {
            sendCommand(LCD_RETURNHOME);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

/*
 * Enable/Disable backlight
 */
        void LCD16x2::enableBacklight(bool backlight_on) {
            m_backlight_on = backlight_on;
            i2cWriteByte(m_i2cHandle, m_backlight_on ? LCD_BACKLIGHT : 0);
        }

/*
 * Return the status of backlight
 */
        bool LCD16x2::getBacklight() const {
            return m_backlight_on;
        }

/*
 * Enable/Disable cursor
 */
        void LCD16x2::enableCursor(bool enable) {
            m_displayControl = enable ? (m_displayControl | LCD_CURSORON) : (m_displayControl & ~LCD_CURSORON);
            sendCommand(m_displayControl);
        }

/*
 * Enable/Disable cursor blinking
 */
        void LCD16x2::enableBlinking(bool enable) {
            m_displayControl = enable ? (m_displayControl | LCD_BLINKON) : (m_displayControl & ~LCD_BLINKON);
            sendCommand(m_displayControl);
        }

/*
 * Display a C string using operator <<
 * TODO : accept formatting operations like cout<<
 */
        LCD16x2 &LCD16x2::operator<<(const char *chaine) {
            puts(chaine);
            return *this;
        }

/*
 * Scroll display to the left or to the right
 */
        void LCD16x2::scrollDisplayRight(bool right) {
            sendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | (right ? LCD_MOVERIGHT : 0));
        }

/*
 * Enable/Disable autoscroll
 */
        void LCD16x2::autoScroll(bool enable) {
            m_displayMode = enable ? (m_displayMode | LCD_ENTRYSHIFTINCREMENT) : (m_displayMode & ~LCD_ENTRYSHIFTINCREMENT);
            sendCommand(m_displayMode);
        }

/*
 * A C-printf allowing to print a formatted string
 */
        void LCD16x2::printf(const char *fmt, ...) {
            char buffer[256];
            va_list args;
            va_start (args, fmt);
            vsprintf(buffer, fmt, args);
            puts(buffer);
            va_end (args);
        }

    }
}
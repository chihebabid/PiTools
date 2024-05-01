/*
 * DHT11.cpp
 *
 *  Created on: 13 avr. 2021
 *      Author: chiheb
 */

#include "dht11.h"
#include <pigpio.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include <stdexcept>
#include "defs.hpp"



namespace pitools {
    namespace sensors {
        constexpr auto FREQUENCY=10000000;
        DHT11::DHT11(const uint8_t gpioPin) :
                mDataPin(gpioPin) {
            pitools::use_pins({mDataPin});
            init();
        }

        void DHT11::init() {
            gpioSetMode(mDataPin, PI_OUTPUT);
            gpioWrite(mDataPin, 1);
        }

        dht11_data_t DHT11::ProcessData(uint64_t Data) {
            uint8_t HumidityHigh = (Data >> 32) & 0xFF;
            uint8_t HumidityLow = (Data >> 24) & 0xFF;
            uint8_t TemperatureHigh = (Data >> 16) & 0xFF;
            uint8_t TemperatureLow = (Data >> 8) & 0xFF;
            uint8_t Parity = Data & 0xFF;

            if (Parity
                != CalculateParity(HumidityHigh, HumidityLow, TemperatureHigh,
                                   TemperatureLow)) {
                mError=DHT11_ERRORS::CHECKSUM_ERROR;
                return {0,0};
            }


            return {TemperatureHigh, HumidityHigh};
        }

        dht11_data_t DHT11::getData() {
            uint64_t data{0};
            mError=DHT11_ERRORS::NO_ERROR;
            SendStartSignal();
            gpioSetMode(mDataPin, PI_INPUT);
            try {
                WaitForLow();
                WaitForHigh();
                WaitForLow();
                for (auto i{0}; i < 40; ++i) {
                    data <<= 1;
                    int LowTime = WaitForHigh();
                    int HighTime = WaitForLow();
                    if (LowTime < HighTime) {
                        data |= 0x1;
                    }
                }
                WaitForHigh();
            } catch (...) {
                gpioSetMode(mDataPin, PI_OUTPUT);
                gpioWrite(mDataPin, 1);
                mError=DHT11_ERRORS::TIMEOUT;
                return {0,0};
            }
            gpioSetMode(mDataPin, PI_OUTPUT);
            gpioWrite(mDataPin, 1);
            return ProcessData(data);
        }

        int DHT11::WaitForLow() {
            auto StartTime = gpioTick();
            while (gpioRead(mDataPin)) {
                if (FREQUENCY < gpioTick() - StartTime) {
                    throw std::runtime_error(
                            "Timeout while waiting for pin to get low.");
                }
            }
            return gpioTick() - StartTime;
        }

        int DHT11::WaitForHigh() {
            auto StartTime = gpioTick();
            while (!gpioRead(mDataPin)) {
                if (FREQUENCY < gpioTick() - StartTime) {
                    throw std::runtime_error(
                            "Timeout while waiting for pin to get high.");
                }
            }
            return gpioTick() - StartTime;
        }

        void DHT11::SendStartSignal() {
            gpioWrite(mDataPin, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            gpioWrite(mDataPin, 1);
        }


        uint8_t DHT11::CalculateParity(uint8_t HumidityHigh, uint8_t HumidityLow,
                                       uint8_t TemperatureHigh, uint8_t TemperatureLow) {
            return static_cast<uint8_t>(HumidityHigh + HumidityLow + TemperatureHigh
                                        + TemperatureLow);
        }

        DHT11_ERRORS DHT11::getError() {
            return mError;
        }
    } // end namespace sensors
} // end namespace pitools
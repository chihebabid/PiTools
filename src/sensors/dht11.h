/*
 * DHT11.h
 *
 *  Created on: 13 avr. 2021
 *      Author: chiheb
 */

#ifndef CDHT11_H_
#define CDHT11_H_

#include <cstdint>
#include <tuple>
#include "sensor.hpp"

namespace pitools {
    namespace sensors {
        enum class DHT11_ERRORS {
            NO_ERROR = 0,
            TIMEOUT = 1,
            CHECKSUM_ERROR = 2,
            UNKNOWN_ERROR = 3
        };
        struct dht11_data_t {
            int temperature;
            int humidity;
        };
        class DHT11 : public Sensor<dht11_data_t> {
        public:
            /*
             * Constructor
             * Provide an instance to a raspberry pi object and the GPIO pin for data transmission.
             */
            explicit DHT11(const uint8_t gpioPin);

            void init() override;

            /*
             * This method starts a measurement and returns the data from the last measurement.
             * This behaviour is due to how the DHT11 sensor works. Make two consecutive readings
             * to get the most current data. But wait at least 5 seconds between measurements,
             * otherwise the data will be wrong.
             */
            dht11_data_t getData() override;

        private:
            const uint8_t mDataPin;
            DHT11_ERRORS mError {DHT11_ERRORS::NO_ERROR};

            /*
             * Signal the sensor that we want to read the data.
             */
            void SendStartSignal();

            /*
             * Wait for the GPIO pin to get low. Returns the microseconds waited until the pin got low.
             */
            int WaitForLow();

            /*
             * Wait for the GPIO pin to get high. Returns the microseconds waited until the pin git high.
             */
            int WaitForHigh();

            /*
             * Extracts the relevant data from the binary raw data.
             */
            dht11_data_t ProcessData(uint64_t Data);

            /*
             * Calculates the checksum for the data to make sure there was no error during data transmission.
             */
            uint8_t CalculateParity(uint8_t HumidityHigh, uint8_t HumidityLow,
                                    uint8_t TemperatureHigh, uint8_t TemperatureLow);
            /*
             * @brief Return the error code
             */
            DHT11_ERRORS getError();

        };
    }
}
#endif /* CDHT11_H_ */

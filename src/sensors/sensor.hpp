//
// Created by chiheb on 19/04/24.
//

#ifndef SMARTHOME_SENSOR_HPP
#define SMARTHOME_SENSOR_HPP
namespace pitools {
    namespace sensors {
        template<typename T>
        class Sensor {
            virtual void init() = 0;

        protected:
            Sensor() = default;

        public:
            /*
             * No need for copying objects of this class
             */
            Sensor(const Sensor &) = delete;
            Sensor &operator=(const Sensor &) = delete;
            virtual T getData() = 0;
        };
    } // end namespace sensors
} // end namespace pitools
#endif //SMARTHOME_SENSOR_HPP

//
// Created by chiheb on 20/04/24.
//

#ifndef SMARTHOME_ACTUATOR_HPP
#define SMARTHOME_ACTUATOR_HPP
namespace pitools {
    namespace actuators {
        //template<typename T>
        class Actuator {
            virtual void init() = 0;
        protected:
            Actuator() = default;

        public:
            /*
             * No need for copying objects of this class
             */
            Actuator(const Actuator &) = delete;

            Actuator &operator=(const Actuator &) = delete;


        };
    } // end namespace sensors
} // end namespace pitools
#endif //SMARTHOME_ACTUATOR_HPP

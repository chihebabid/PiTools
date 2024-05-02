//
// Created by chiheb on 29/04/24.
//

#ifndef SMARTHOME_STEPPERMOTOR28BYJ48_HPP
#define SMARTHOME_STEPPERMOTOR28BYJ48_HPP
#include <chrono>
#include <array>
#include "pitools.hpp"

using namespace std::chrono_literals;
namespace pitools {
    namespace actuators {
        struct stepper_param_t {
            const uint32_t maxSteps {2048};
            const std::chrono::duration<double>  stepDelay {3ms};
            const uint8_t clock_dir[4] {0, 1, 2, 3};
            const uint8_t anti_clock_dir[4] {3, 2, 1, 0};
        };
        class StepperMotor28BYJ48 : public Actuator, public pitools::timers::WaitDuration<StepperMotor28BYJ48> {
            std::array<pitools::gpio::GPIOOutput,4> mlPins;
            stepper_param_t mParams;
            void init() override;


            void makeStep();
            void makeHalfStep();

            int mCurrentStep = 0;
            int mCurrentDir = 1;
            int mSteps = 0;
            uint8_t mCurrentCoilIndex {0};


        public:
            StepperMotor28BYJ48(const std::array<uint8_t,4>& lpins, const stepper_param_t & d=stepper_param_t{});
            StepperMotor28BYJ48& rotate(const bool &dir, const uint32_t & steps);
            StepperMotor28BYJ48& rotateClock(const uint32_t & steps);
            StepperMotor28BYJ48& rotateAntiClock(const uint32_t & steps);
            StepperMotor28BYJ48& rotateAngle(const bool &dir, const uint32_t & steps);
            StepperMotor28BYJ48& rotateClockAngle(const uint32_t & steps);
            StepperMotor28BYJ48& rotateAntiClockAngle(const uint32_t & steps);
        };
    }
}


#endif //SMARTHOME_STEPPERMOTOR28BYJ48_HPP

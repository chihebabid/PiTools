//
// Created by chiheb on 29/04/24.
//

#include "StepperMotor28BYJ48.hpp"


namespace pitools {
    namespace actuators {
        StepperMotor28BYJ48::StepperMotor28BYJ48(const std::array<uint8_t,4>& lpins, const stepper_param_t &d): mlPins{lpins[0], lpins[1], lpins[2], lpins[3]}, mParams{d} {
            init();
        }
        /*
         * @brief Initialize the stepper motor
         */
        void StepperMotor28BYJ48::init() {
            for (auto &pin: mlPins) {
                pin.clear();
            }
        }

        /*
         * @brief Perform one step
         */
        void StepperMotor28BYJ48::makeStep() {
            mlPins[mCurrentCoilIndex].set();
            if (mCurrentDir) {
                mlPins[mCurrentCoilIndex == 0 ? 3 : mCurrentCoilIndex - 1].clear();
                mCurrentCoilIndex=(mCurrentCoilIndex+1)%4;
            }
            else {
                mlPins[(mCurrentCoilIndex + 1) % 4].clear();
                mCurrentCoilIndex=mCurrentCoilIndex==0 ? 3 : mCurrentCoilIndex-1;
            }
            std::this_thread::sleep_for(mParams.stepDelay);
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotate(const bool &dir, const uint32_t & steps) {
            mCurrentDir=dir;
            for (uint32_t i {0};i<steps;++i) {
                makeStep();
            }
            return *this;
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotateClock(const uint32_t & steps) {
            return rotate(1,steps);
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotateAntiClock(const uint32_t & steps) {
            return rotate(0,steps);
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotateAngle(const bool &dir, const uint32_t & angle) {
            mCurrentDir=dir;
            uint32_t steps {static_cast<uint32_t >(angle*(static_cast<float>(mParams.maxSteps)/360.0f))};
            for (uint32_t i {0};i<steps;++i) {
                makeStep();
            }
            return *this;
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotateClockAngle(const uint32_t & angle) {
            return rotateAngle(1,angle);
        }

        StepperMotor28BYJ48& StepperMotor28BYJ48::rotateAntiClockAngle(const uint32_t & angle) {
            return rotateAngle(0,angle);
        }
    }
}

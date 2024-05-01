//
// Created by chiheb on 29/04/24.
//

#include "StepperMotor.hpp"


namespace pitools {
    namespace actuators {
        StepperMotor::StepperMotor(const uint8_t lpins[],const stepper_param_t &d):lPins{lpins[0],lpins[1],lpins[2],lpins[3]},mParams{d} {
            init();
        }
        /*
         * @brief Initialize the stepper motor
         */
        void StepperMotor::init() {
            for (auto &pin: lPins) {
                pin.clear();
            }
        }

        /*
         * @brief Perform one step
         */
        void StepperMotor::makeStep() {
            lPins[mCurrentCoilIndex].set();
            if (mCurrentDir) {
                lPins[mCurrentCoilIndex==0 ? 3 : mCurrentCoilIndex-1].clear();
                mCurrentCoilIndex=(mCurrentCoilIndex+1)%4;
            }
            else {
                lPins[(mCurrentCoilIndex + 1)%4].clear();
                mCurrentCoilIndex=mCurrentCoilIndex==0 ? 3 : mCurrentCoilIndex-1;
            }
            std::this_thread::sleep_for(mParams.stepDelay);
        }

        StepperMotor& StepperMotor::rotate(const bool &dir,const uint32_t & steps) {
            mCurrentDir=dir;
            for (uint32_t i {0};i<steps;++i) {
                makeStep();
            }
            return *this;
        }

        StepperMotor& StepperMotor::rotateClock(const uint32_t & steps) {
            return rotate(1,steps);
        }

        StepperMotor& StepperMotor::rotateAntiClock(const uint32_t & steps) {
            return rotate(0,steps);
        }

        StepperMotor& StepperMotor::rotateAngle(const bool &dir,const uint32_t & angle) {
            mCurrentDir=dir;
            uint32_t steps {static_cast<uint32_t >(angle*(static_cast<float>(mParams.maxSteps)/360.0f))};
            for (uint32_t i {0};i<steps;++i) {
                makeStep();
            }
            return *this;
        }

        StepperMotor& StepperMotor::rotateClockAngle(const uint32_t & angle) {
            return rotateAngle(1,angle);
        }

        StepperMotor& StepperMotor::rotateAntiClockAngle(const uint32_t & angle) {
            return rotateAngle(0,angle);
        }
    }
}

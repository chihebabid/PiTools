//
// Created by chiheb on 04/05/24.
//

#include "MFRC522.hpp"
#include <pitools.hpp>
namespace pitools {
    namespace spidevices {
        MFRC522::MFRC522(const mfrc522_options_t &options): SPIDevice(options),mType(options.type) {
            if (options.hard_reset>0) {
                mHardResetPin = std::make_unique<pitools::gpio::GPIODevice>(options.hard_reset);
                mHardResetPin->set();
            }
            init();
        }

        void MFRC522::writeRegister(uint8_t reg, uint8_t value) {
            char buf[2];
            buf[0]= reg;
            buf[1]= value;
            spiWrite(mHSpi, buf, 2);
        }

        void MFRC522::reset() {
            bool hardware_reset {false};
            if (mHardResetPin) {
                hardware_reset=resetHardware();
            }
            if (!hardware_reset) {
                // Soft reset
                writeRegister(PCD_Register::CommandReg,PCD_Command::PCD_SoftReset);
            }
        }

        void MFRC522::init() {
            reset();
            std::this_thread::sleep_for(200ms);
            writeRegister(PCD_Register::TModeReg,0x80);
            writeRegister(PCD_Register::TPrescalerReg,0x3E);
            writeRegister(PCD_Register::TReloadRegL,30);
            writeRegister(PCD_Register::TReloadRegH,0);
            writeRegister(PCD_Register::TxASKReg,0x40);
            writeRegister(PCD_Register::ModeReg,0x3D);
            if (mType=='A') {
                writeRegister(PCD_Register::Status2Reg,0x7F);
                writeRegister(PCD_Register::ModeReg,0x3D);
                writeRegister(PCD_Register::RxSelReg,0x86);
                writeRegister(PCD_Register::RFCfgReg,0x7F);
                writeRegister(PCD_Register::TReloadRegL,30);
                writeRegister(PCD_Register::TReloadRegH,0);
                writeRegister(PCD_Register::TModeReg,0x8D);
            }




        }

        bool MFRC522::resetHardware() {
            // First set the resetPowerDownPin as digital input, to check the CMFRC522 power down mode.
            mHardResetPin->mode(pitools::gpio::GPIOMODE::INPUT);

            if (mHardResetPin->state()==pitools::gpio::GPIOSTATE::LOW) {    // The CMFRC522 chip is in power down mode.
                mHardResetPin->mode(pitools::gpio::GPIOMODE::OUTPUT).clear().waitFor(2ms).set().waitFor(50ms);
                return true;
            }
            return false;
        }
    }
}
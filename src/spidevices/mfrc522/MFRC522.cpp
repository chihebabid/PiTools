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

        void MFRC522::clearRegisterBitMask(const uint8_t& reg,const uint8_t& mask) {
            uint8_t tmp;
            tmp = readRegister(reg);
            writeRegister(reg, tmp & (~mask));
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
                writeRegister(PCD_Register::TPrescalerReg,0x3E);
            }
            antennaOn();
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
        void MFRC522::antennaOn() {
            uint8_t value = readRegister(TxControlReg);
            if ((value & 0x03) != 0x03) {
                writeRegister(TxControlReg, value | 0x03);
            }
        }

        void MFRC522::antennaOff() {
            clearRegisterBitMask(TxControlReg, 0x03);
        }

        uint8_t MFRC522::readRegister(uint8_t reg) {
            char txBuf[2];
            txBuf[0]=(reg & 0x7E) | 0x80;
            txBuf[1]= 0;
            char rxBuf[2];
            spiXfer(mHSpi, txBuf, rxBuf, 2);
            return rxBuf[1];
        }

        MFRC522_Status_t MFRC522::request(uint8_t reqMode, uint8_t* TagType) {
            MFRC522_Status_t status;
            uint16_t backBits;			//The received data bits

            writeRegister(BitFramingReg, 0x07);//TxLastBists = BitFramingReg[2..0]	???

            TagType[0] = reqMode;

            status = toCard(PCD_Transceive, TagType, 1, TagType, &backBits);

            if ((status != MI_OK)) {
                return status;
            }
            if (backBits != 0x10) {
                return MI_ERR;
            }
            return MI_OK;
        }

        void MFRC522::setRegisterBitMask(const uint8_t &reg,const uint8_t &mask) {
            uint8_t tmp;
            tmp = readRegister(reg);
            writeRegister(reg, tmp | mask);            // set bit mask
        }

            MFRC522_Status_t MFRC522::toCard(uint8_t command, uint8_t* sendData,uint8_t sendLen, uint8_t* backData, uint16_t* backLen) {
                MFRC522_Status_t status = MI_ERR;
                uint8_t irqEn = 0x00;
                uint8_t waitIRq = 0x00;
                uint8_t lastBits;
                uint8_t n;
                uint16_t i;

                switch (command) {
                    case PCD_MFAuthent: {
                        irqEn = 0x12;
                        waitIRq = 0x10;
                        break;
                    }
                    case PCD_Transceive: {
                        irqEn = 0x77;
                        waitIRq = 0x30;
                        break;
                    }
                    default:
                        break;
                }

                writeRegister(ComIEnReg, irqEn | 0x80);
                clearRegisterBitMask(ComIrqReg, 0x80);
                setRegisterBitMask(FIFOLevelReg, 0x80);

            writeRegister(CommandReg, PCD_Idle);

                //Writing data to the FIFO
                for (i = 0; i < sendLen; i++) {
                    writeRegister(FIFODataReg, sendData[i]);
                }

                //Execute the command
            writeRegister(CommandReg, command);
                if (command == PCD_Transceive) {
                    setRegisterBitMask(BitFramingReg, 0x80);//StartSend=1,transmission of data starts
                }

                //Waiting to receive data to complete
                i = 2000;//i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
                do {
                    //CommIrqReg[7..0]
                    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
                    if (mCheckingCard) {
                        std::this_thread::sleep_for(16ms);
                    } else {
                        std::this_thread::sleep_for(20ms);
                    }
                    n = readRegister(ComIrqReg);
                    i--;
                } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

                clearRegisterBitMask(BitFramingReg, 0x80);		//StartSend=0

                if (i != 0) {
                    if (!(readRegister(ErrorReg) & 0x1B)) {

                        if (n & irqEn & 0x01) {
                            status = MI_NOTAGERR;
                        } else {
                            status = MI_OK;
                        }

                        if (command == PCD_Transceive) {
                            n = readRegister(FIFOLevelReg);
                            lastBits = readRegister(ControlReg) & 0x07;
                            if (lastBits) {
                                *backLen = (n - 1) * 8 + lastBits;
                            } else {
                                *backLen = n * 8;
                            }

                            if (n == 0) {
                                n = 1;
                            }
                            if (n > MFRC522_MAX_LEN) {
                                n = MFRC522_MAX_LEN;
                            }

                            //Reading the received data in FIFO
                            for (i = 0; i < n; i++) {
                                backData[i] = readRegister(FIFODataReg);
                            }
                        }
                    } else {
                        status = MI_ERR;
                    }
                }

                return status;
        }

        MFRC522_Status_t MFRC522::anticoll(uint8_t* serNum) {
            MFRC522_Status_t status;
            uint8_t i;
            uint8_t serNumCheck = 0;
            uint16_t unLen;

            writeRegister(BitFramingReg, 0x00);//TxLastBists = BitFramingReg[2..0]

            serNum[0] = PICC_ANTICOLL;
            serNum[1] = 0x20;
            status = toCard(PCD_Transceive, serNum, 2, serNum, &unLen);

            if (status == MI_OK) {
                //Check card serial number
                for (i = 0; i < 4; i++) {
                    serNumCheck ^= serNum[i];
                }
                /* check sum with last byte*/
                if (serNumCheck != serNum[i]) {
                    status = MI_ERR;
                }
            }
            return status;
        }
    }
}
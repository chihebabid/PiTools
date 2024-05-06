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

        uint8_t MFRC522::selectTag(uint8_t* serNum) {
            uint8_t i;
            MFRC522_Status_t status;
            uint8_t size;
            uint16_t recvBits;
            uint8_t buffer[32] = "";

            buffer[0] = PICC_SElECTTAG;
            buffer[1] = 0x70;
            for (i = 0; i < 5; i++) {
                buffer[i + 2] = *(serNum + i);
            }
            calculateCRC(buffer, 7, &buffer[7]);	//Fill [7:8] with 2byte CRC
            status = toCard(PCD_Transceive, buffer, 9, buffer, &recvBits);

            if ((status == MI_OK) && (recvBits == 0x18)) {
                size = buffer[0];
            } else {
                size = 0;
            }
            return size;
        }

        void MFRC522::calculateCRC(uint8_t* pIndata, uint8_t len, uint8_t* pOutData) {
            uint8_t i, n;

            clearRegisterBitMask(DivIrqReg, 0x04);			//CRCIrq = 0
            setRegisterBitMask(FIFOLevelReg, 0x80);	//Clear the FIFO pointer
            //Write_MFRC522(CommandReg, PCD_IDLE);

            //Writing data to the FIFO
            for (i = 0; i < len; i++) {
                writeRegister(FIFODataReg, *(pIndata + i));
            }
            writeRegister(CommandReg, PCD_CalcCRC);

            //Wait CRC calculation is complete
            i = 0xFF;
            do {
                n = readRegister(DivIrqReg);
                i--;
            } while ((i != 0) && !(n & 0x04));			//CRCIrq = 1

            //Read CRC calculation result
            pOutData[0] = readRegister(CRCResultRegL);
            pOutData[1] = readRegister(CRCResultRegH);
        }

        const char *MFRC522::typeToString(PICC_TYPE_t type) {
            return PICC_TYPE_STRING[type];
        }

        int MFRC522::parseType(uint8_t TagSelectRet) {
            if (TagSelectRet & 0x04) { // UID not complete
                return PICC_TYPE_NOT_COMPLETE;
            }

            switch (TagSelectRet) {
                case 0x09:
                    return PICC_TYPE_MIFARE_MINI;
                    break;
                case 0x08:
                    return PICC_TYPE_MIFARE_1K;
                    break;
                case 0x18:
                    return PICC_TYPE_MIFARE_4K;
                    break;
                case 0x00:
                    return PICC_TYPE_MIFARE_UL;
                    break;
                case 0x10:
                case 0x11:
                    return PICC_TYPE_MIFARE_PLUS;
                    break;
                case 0x01:
                    return PICC_TYPE_TNP3XXX;
                    break;
                default:
                    break;
            }

            if (TagSelectRet & 0x20) {
                return PICC_TYPE_ISO_14443_4;
            }

            if (TagSelectRet & 0x40) {
                return PICC_TYPE_ISO_18092;
            }

            return PICC_TYPE_UNKNOWN;
        }

        void MFRC522::halt() {
            uint16_t unLen;
            uint8_t buff[4];

            buff[0] = PICC_HALT;
            buff[1] = 0;
            calculateCRC(buff, 2, &buff[2]);

            toCard(PCD_Transceive, buff, 4, buff, &unLen);
        }

        MFRC522_Status_t MFRC522::read(uint8_t blockAddr, uint8_t* recvData) {
            MFRC522_Status_t status;
            uint16_t unLen;

            recvData[0] = PICC_READ;
            recvData[1] = blockAddr;
            calculateCRC(recvData, 2, &recvData[2]);
            status = toCard(PCD_Transceive, recvData, 4, recvData, &unLen);

            if ((status != MI_OK) || (unLen != 0x90)) {
                return MI_ERR;
            }

            return (MFRC522_Status_t)unLen;
        }

        MFRC522_Status_t MFRC522::auth(uint8_t authMode, uint8_t BlockAddr,uint8_t* Sectorkey, uint8_t* serNum) {
            MFRC522_Status_t status;
            uint16_t recvBits;
            uint8_t i;
            uint8_t buff[12];

            //Verify the command block address + sector + password + card serial number
            buff[0] = authMode;
            buff[1] = BlockAddr;
            for (i = 0; i < 6; i++) {
                buff[i + 2] = *(Sectorkey + i);
            }
            for (i = 0; i < 4; i++) {
                buff[i + 8] = *(serNum + i);
            }
            status = toCard(PCD_Transceive, buff, 12, buff, &recvBits);

            if ((status != MI_OK)
                || (!(readRegister(Status2Reg) & 0x08))) {
                status = MI_ERR;
            }

            return status;
        }

        MFRC522_Status_t MFRC522::write(uint8_t blockAddr, uint8_t* writeData) {
            MFRC522_Status_t status;
            uint16_t recvBits;
            uint8_t i;
            uint8_t buff[18];

            buff[0] = PICC_WRITE;
            buff[1] = blockAddr;
            calculateCRC(buff, 2, &buff[2]);
            status = toCard(PCD_Transceive, buff, 4, buff, &recvBits);

            if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
                goto ERROR;
            }

            if (status == MI_OK) {
                //Data to the FIFO write 16Byte
                for (i = 0; i < 16; i++) {
                    buff[i] = *(writeData + i);
                }
                calculateCRC(buff, 16, &buff[16]);
                status = toCard(PCD_Transceive, buff, 18, buff, &recvBits);

                if ((status != MI_OK) || (recvBits != 4)
                    || ((buff[0] & 0x0F) != 0x0A)) {
                    goto ERROR;

                }
            }
            return MI_OK;

            ERROR:
            if (recvBits == 4) {
                status = static_cast<MFRC522_Status_t>(buff[0] & 0x0F);
            } else {
                status = MI_ERR;
            }
            return status;
        }
    }
}
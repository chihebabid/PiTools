add_library(pitools pitools.hpp
        gpio/GPIODevice.hpp
        gpio/GPIOInput.cpp
        gpio/GPIOInput.hpp
        gpio/GPIODevice.cpp
        sensors/hc04.cpp sensors/dht11.cpp
        sensors/hc04.hpp sensors/dht11.h
        sensors/sensor.hpp
        sensors/dht11.h
        actuators/HC595.h
        actuators/HC595.cpp
        actuators/actuator.hpp
        actuators/Servo.cpp
        actuators/Servo.hpp
        i2cdevices/LCD16x2.cpp
        i2cdevices/LCD16x2.hpp
        defs.hpp
        defs.cpp
        i2cdevices/I2CDevice.cpp
        i2cdevices/I2CDevice.hpp
        i2cdevices/MCP9808.cpp
        i2cdevices/MCP9808.hpp
        spidevices/SPIDevice.cpp
        spidevices/SPIDevice.hpp
        gpio/GPIOOutput.cpp
        gpio/GPIOOutput.hpp
        actuators/StepperMotor28BYJ48.cpp
        actuators/StepperMotor28BYJ48.hpp
        timers/WaitDuration.hpp

        spidevices/mfrc522/MFRC522.cpp
        spidevices/mfrc522/MFRC522.hpp
        ../samples/dht11/main.cpp
)

target_link_libraries(pitools pigpio)
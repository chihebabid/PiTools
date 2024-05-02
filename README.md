A C++ library based on pigpio that provides several classes to manage actuators and sensors.

## Description
PiTools offers a suite of classes that abstract the pigpio C library, enabling access to the full I/O capabilities of the Raspberry Pi platform. This allows developers to concentrate on the application's business logic. The library includes classes for controlling a variety of sensors and actuators, most of which offer a fluent interface and are implemented using the C++20 standard.

## Currently Implemented Classes
### Sensors
•  [**HC-SR04**](https://www.bing.com/search?form=SKPBOT&q=HC-SR04): Ultrasonic distance sensor.

•  [**DHT11**](https://www.bing.com/search?form=SKPBOT&q=DHT11): Temperature and humidity sensor.


### Actuators
•  [**74HC595**](https://www.bing.com/search?form=SKPBOT&q=74HC595): 8-bit serial-in, serial or parallel-out shift register.

•  [**28BYJ-48**](https://www.bing.com/search?form=SKPBOT&q=28BYJ-48): Stepper motor.

•  [**Servo Motors**](https://www.bing.com/search?form=SKPBOT&q=Servo%20Motors): Controlled by PWM signal.


### I2C Components
•  [**MCP9808**](https://www.bing.com/search?form=SKPBOT&q=MCP9808): Temperature sensor.

•  [**LCD 16x2**](https://www.bing.com/search?form=SKPBOT&q=LCD%2016x2): A 16x2 character LCD display.


## Roadmap
### Upcoming Classes
•  [**PCF8574**]:  8-bit I/O expander for the I2C bus.

•  [**RC522**](https://www.bing.com/search?form=SKPBOT&q=RC522): RFID reader/writer module.


## Contribution
Contributions are welcome! If you'd like to add a new class or improve existing ones, please submit a pull request.

# Why another Wiegand library? 



# Multi Reader Wiegand 4 bit, 8 bit, 24 bit, 26 bit, 32 bit and 34 bit library for Arduino

The Wiegand interface is a de facto standard commonly used to connect a card reader or keypad to an electronic entry system. Wiegand interface has the ability to transmit signal over long distance with a simple 3 wires connection. This library uses interrupt pins from Arduino to read the pulses from Wiegand interface and return the code and type of the Wiegand.

## NON Standard Bit Length

This library was designed to decode standard Wiegand protocol with 1 START bit, 1 STOP bit and bit length stated in the title. For multi-bit length protocol, please try [Wiegand NG Multi Bit Wiegand Library for Arduino](https://github.com/jpliew/Wiegand-NG-Multi-Bit-Wiegand-Library-for-Arduino).

## Requirements

The following are needed 

* [Arduino](http://www.arduino.cc) - Any ATMEGA328 compatible board should work.
* [Wiegand RFID Reader](http://www.monkeyboard.org/products/85-developmentboard/84-rfid-wiegand-protocol-development-kit) - The code was written for this reader however customers reported working with [HID](http://www.hidglobal.com/products/cards-and-credentials) compatible readers.
* DATA0 of the first Wiegand reader connects to Arduino PIN 2 and DATA1 of the first Wiegand reader connects to Arduino PIN 3
* DATA0 of the second Wiegand reader connects to Arduino PIN 4 and DATA1 of the second Wiegand reader connects to Arduino PIN 5


## Installation 

Create a folder named WiegandMulti in Arduino's libraries folder.  You will have the following folder structure:

	cd arduino/libraries
	git clone https://github.com/jpliew/Multi-Reader-Wiegand-Protocol-Library-for-Arduino.git WiegandMulti

## Arduino Sketch

![alt text](http://www.monkeyboard.org/images/tutorials/wiegand/wiegand_arduino.png "RFID Reader to Arduino connection diagram")


Execute Arduino IDE, select Example-->WiegandMulti-->WiegandTest

### Example

Please see example in the example folder.

## Contributors

[softwarefoundry](https://github.com/softwarefoundry) added library.properties

[Francesco Uggetti (ugge75)](https://github.com/ugge75) improved this version of library to support multiple readers for ATMEGA2560.  Please check out [his version of multiple wiegand readers library here](https://github.com/ugge75/Wiegand-Protocol-Library-for-Arduino-MEGA-2560)

[Apollon77](https://github.com/Apollon77) improved interrupt safety and removed sysTick from global

[paulfurley](https://github.com/paulfurley) added 4 bit code

[PaulStoffregen](https://github.com/PaulStoffregen) added Use digitalPinToInterrupt on newer Arduino software, if present

[tholum](https://github.com/tholum)  Simpler Instructions

[zanhecht](https://github.com/zanhecht) Recognize 24- and 32-bit

[luckymallari](https://github.com/luckymallari) Simplified begin(D0, D1) for ESP(8266/32) devices.

Written by [JP Liew](http://jpliew.com)

Project home: http://www.monkeyboard.org/tutorials/82-protocol/24-wiegand-converter

*This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.*

*This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.*

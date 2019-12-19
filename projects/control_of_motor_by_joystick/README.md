
# CMPT 2 Final project

#### Table of Contents

1. [Introduction](#introduction)
2. [Hardware](#hardware)
3. [Software](#software)
4. [References](#references)


## Introduction

This project takes place under CMPT 2 (Digital Elctronics) lessons, held at Brno Univ. of Technology during the winter semester of 2019. The chosen task was to control several servomotors with joysticks. A video demonstration can be found [here](https://photos.google.com/share/AF1QipMdpGbvYN99597vpX2I2iQgWYrYtQ_Slmrb9rP06C0bkLe5C2c2SOC2XVQtNyGAcw?key=dDFmclJlN3dPSGxSQUx2NzhiX2pqblhRV3hVTk5B). The PowerPoint presentation can be found [here](https://github.com/Nyrdann/Digital-electronics-2/blob/master/docs/Digital%20electronics%202%20project.pptx).

## Hardware

This project is meant for the microcontroller [ATmega328P](https://www.microchip.com/wwwproducts/en/ATmega328p).
    The Arduino Uno Rev3 was used, with 2 servomotors [SG90](https://arduino-shop.cz/arduino/5461-servo-motor-9g-sg90-4.2v-6v.html) and a [HW-504 analog joystick](https://arduino-shop.cz/arduino/884-arduino-joystick-ps2.html). 
A [wiring diagram](https://github.com/Nyrdann/Digital-electronics-2/blob/master/docs/sketch/connection%20sketch.png) can be found in the directory *docs/sketch* of this repository. 


## Software
The code was compiled using AVR 8-bit Toolchain 3.6.2
A [flowchart](https://github.com/Nyrdann/Digital-electronics-2/blob/master/docs/flowchart/CMPT_project.png) description is avalailable under the *docs/flowchart* directory. 
There's only one file, *main.c*. It requires the provided library **gpio** and **timer**, and the **uart** library for a debugging aspect (libraries are included, in the *projects/library* directory).
Explanations about how installing the toolchain, compiling and flashing can be found under */docs/HOWTO_XXX.md*.

## References

All the datasheet can be found under */docs*.
1. Microchip, [AVR 8-bit Toolchain for Windows, Linux, Mac OS X](https://www.microchip.com/mplab/avr-support/avr-and-arm-toolchains-c-compilers)
2. [ATmega328P datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf)
3. [SG90 datasheet](http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf)
4. [HW-504 analaog joystick](http://www.energiazero.org/arduino_sensori/joystick_module.pdf) datasheet and tips
5. [Useful explanations](https://docs.isy.liu.se/pub/VanHeden/DataSheets/Analog_Joystick.pdf) for HW-504 analog joystick

# ESP_HomieRollerShutter [![Build Status](https://travis-ci.org/fablabnbg/ESP_HomieRollerShutter.svg?branch=master)](https://travis-ci.org/fablabnbg/ESP_HomieRollerShutter)

A "Homie Node" for homie-esp8266 implentation to control a rollershutter.

## Features

* Compatible to Homie-Convention 3.0.1
* Variable output pin configuration
* Reacts to following MQTT commands (compatible to OpenHAB Rollershutter item/type:
  * <number_value>   - moves rollershutter to this position (0-100% with 0% fully open (UP))
  * UP - go up
  * DOWN - go down
  * STOP - halt
* Adjustable times for moving up and for moving down
* Adjustable extra time tp move after reaching end position


## Build instructions

* to be built with platformio:

`platformio run`


If you don't have platformio installed, see https://platformio.org/platformio-ide for installation instructions.

## Dependencies

**All dependencies are installed automatically by platformio**

* Automaton 
:reactive state-machine
* homie-ESP8266 
:ESP8266 implementation of Homie convention
* HomieLoggerNode  
:Logging mechanism for homie-esp8266

Currently these dependencies are pulled from branches, because Homie V3 support is not released yet or in case of automaton, additional features have been implemnted to the library.


## Programm Logic

* The


## Outlook

* Make this project a real library (main.cpp will be moved to examples/ and renamed)
* Support loading PINs and timings from JSON configuration file
*  

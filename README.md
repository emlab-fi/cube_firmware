# Cube Firmware (and emulator)

## Content
This repo will contain the following parts of Cube ecosystem:
- `include/` contains the common parts of the firmware - motion planner, reactor, common data structures etc.
- `firmware/` has the toolchain and CMake project needed to build the firmware for STM32G4 target
- `tests/` contains unit testing for the common parts
- `emulator/` contains the desktop emulator of Cube

## Firmware features
- support for both cartesian and polar coordinates
- ability to work with relative coordinates
- decently abstracted hardware so that porting is easy
- I2C/SPI passthrough to the connected sensor

## Emulator features
- fully emulates the cube hardware connected via a serial port
- might include 2D/3D simulator later on

## License

TBD
# Cube Firmware

## Content
This repo will contain the following parts of Cube ecosystem:
- `cube_lib/` contains the common parts of the firmware - motion planner, controller, common data structures etc.
- `firmware/` has the toolchain and CMake project needed to build the firmware for STM32G4 target
- `firmware_backport/` contains the backport for the old hardware running on NXP K64F devkit
- `tests/` contains unit testing for the common parts

## Firmware features
- support for both cartesian and polar coordinates
- ability to work with relative coordinates
- decently abstracted hardware so that porting is easy
- I2C/SPI passthrough to the connected sensor

## Documentation and usage
Detailed documentation and usage can be found [here](https://xpecak.pages.fi.muni.cz/cube/docs)

To help with development setup, we provide VSCode workspace config.
To aid debugging on hardware, we have a configuration for the `cortex-debug` plugin for VSCode in each firmware directory.

## License
Mozilla Public License Version 2.0

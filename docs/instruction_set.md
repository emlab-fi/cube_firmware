# Cube instruction set

## nop
__parameters:__  -\
__description:__ Does nothing (might send back some default "hearbeat" data.

## status
__parameters:__ -\
__description:__ Sends back hardware status.

## move_to
__parameters:__  `target_pos`\
__description:__ Moves the sensor head to `target pos`, expects the data to be in suitable format depending on mode (cartesian, polar, relative etc.).

## get_abs_pos
__parameters:__  -\
__description:__ Sends back current absolute position in current coordinate format (polar or cartesian).

## get_rel_pos
__parameters:__  -\
__description:__ Sends back current relative position in current coordinate format (polar or cartesian).

## set_zero_pos
__parameters:__  -\
__description:__ Sets current position as the relative zero of the coordinate system.

## set_coordinate_mode
__parameters:__  `{cartesian, polar2d, polar3d}`\
__description:__ Sets the coordinate mode to specified and resets the relative position to absolute zero.

## home
__parameters:__  -\
__description:__ Launches the homing procedure and resets the whole machine to absolute zero, including relative zero.

## spi_transfer
__parameters:__ `chip_select{0 - 3}`, `length{0-64}`, `data`\
__description:__ Executes SPI transfer with the specified chip select. The transfer is symmetrical - `bytes in == bytes out`. The max length of the transfer is 64 bytes. Returns the transfer status and received bytes.

## i2c_transfer
__parameters:__  `rx_length{0-64}`, `tx_length{0-64}`, `address`, `data`\
__description:__ Executes I2C transfer with the specified rx and tx length, address and sends the specified data. Returns the status of the transfer and received bytes.

## set_gpio_mode
__parameters:__  `gpio_index{0-15}`, `{input_output}`\
__description:__ Sets the GPIO pin to input or output.

## set_gpio
__parameters:__  `gpio_index{0-15}`, `bool value`\
__description:__ Sets the pin output to specified value. Returns error if pin is configured as input.

## get_gpio
__parameters:__  `gpio_index{0-15}`\
__description:__ Reads the pin value and sends it back. If the pin is set as a output, it returns the current set value.

## set_parameter
__parameters:__  `parameter_id`, `parameter_value`\
__description:__ Sets the specified parameter. `parameter_id` is 16bit integer maybe? `parameter_value` is raw 32bit binary form of the value to be set?

## get_parameter
__parameters:__  `parameter_id`\
__description:__ Sets the specified parameter. `parameter_id` is 16bit integer maybe? Returns the raw binary form of the value of the parameter.
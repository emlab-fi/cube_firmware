#include "hardware.hpp"

namespace cube_hw {

std::pair<status, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload*)
{
    return {status::error, {} };
}

std::pair<status, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload*)
{
    return {status::error, {} };
}

status set_gpio_mode(cube::gpio_config_payload*) { return status::error; }

status set_gpio(cube::gpio_config_payload*) { return status::error; }

std::pair<status, bool> read_gpio(cube::gpio_config_payload*) { return {status::error, false}; }

} //namespace cube_hw


#include "hardware.hpp"

namespace cube_hw {

std::optional<cube::encoded_message> get_message() { return {}; }

status send_message(const cube::encoded_message& msg) { return status::error; }

status set_motor_power(bool enabled) { return status::error; }

status do_steps(int32_t a, int32_t b, int32_t c) { return status::error; }

status do_velocity(int32_t a, int32_t b, int32_t c) { return status::error; }

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

uint8_t limits_status() { return 0U; }

} //namespace cube_hw


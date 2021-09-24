#include "hardware.hpp"
#include "peripherals.hpp"

namespace cube_hw {

std::pair<status, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload* data) {

    log_info("cube_hw: addr: %x tx_len: %u rx_len: %u\n", data->address, data->tx_length, data->rx_length);
    auto ret = HAL_I2C_Master_Transmit(&hi2c3, data->address, data->data.data(), data->tx_length, 5);

    if (ret != HAL_OK) {
        return {status::i2c_transfer_error, {} };
    }

    cube::data_reply_payload reply{};
    reply.length = data->rx_length;

    ret = HAL_I2C_Master_Receive(&hi2c3, data->address, reply.data.data(), data->rx_length, 5);

    if (ret != HAL_OK) {
        return {status::i2c_transfer_error, {} };
    }

    return {status::no_error, reply };
}

std::pair<status, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload*)
{
    return {status::error, {} };
}

status set_gpio_mode(cube::gpio_config_payload*) { return status::error; }

status set_gpio(cube::gpio_config_payload*) { return status::error; }

std::pair<status, bool> read_gpio(cube::gpio_config_payload*) { return {status::error, false}; }

} //namespace cube_hw


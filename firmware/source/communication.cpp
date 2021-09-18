#include "hardware.hpp"
#include "peripherals.hpp"

namespace cube_hw {

std::optional<cube::encoded_message> get_message() { return {}; }

status send_message(const cube::encoded_message& msg) {

    uint8_t head[5] = {0x55, 0x55, 0x55, 0, 0};

    switch(msg.type) {
    case cube::message_type::command:
        head[3] = 0x01;
        break;

    case cube::message_type::reply:
        head[3] = 0x02;
        break;

    case cube::message_type::status:
        head[3] = 0x03;
        break;

    default:
        return status::msg_send_error;
    }

    head[4] = static_cast<uint8_t>(msg.length);

    auto ret = HAL_UART_Transmit(&data_uart, head, 5, 1);
    if (ret != HAL_OK) {
        return status::msg_send_error;
    }

    ret = HAL_UART_Transmit(&data_uart, const_cast<uint8_t*>(msg.data.data()), msg.length, 20);
    if (ret != HAL_OK) {
        return status::msg_send_error;
    }

    return status::no_error;
}



} //namespace cube_hw
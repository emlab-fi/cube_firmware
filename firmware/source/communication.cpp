#include "hardware.hpp"
#include "peripherals.hpp"
#include "serial_buffer.hpp"

static serial_buffer receive_buffer{};

extern "C" {
/*
custom UART handler for reading bytes
we basically just shim the regular interrupt
*/
void USART1_IRQHandler(void) {

  //if the interrupt is RXNE(Receive Data Register Not Empty), call our handler on that
  if (__HAL_UART_GET_IT(&data_uart, UART_IT_RXNE)) {
      uint8_t data = data_uart.Instance->RDR;
      receive_buffer.new_byte(data);
  }
  //call the HAL handler to take care of errors and such
  HAL_UART_IRQHandler(&data_uart);
}

} //extern C

namespace cube_hw {

std::optional<cube::encoded_message> get_message() {
    if (!receive_buffer.empty()) {
        return receive_buffer.read();
    }
    return {};
}

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

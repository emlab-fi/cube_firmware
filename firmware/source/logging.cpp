#include <cstdarg>
#include "hardware.hpp"
#include "peripherals.hpp"
#include "printf.h"

extern "C" {
// do nothing implementation
void _putchar(char character)
{
  return;
}

} //extern "C"

namespace {

void write_message(const char symbol, const char * fmt, va_list args) {

    uint32_t tick = HAL_GetTick();

    uint8_t buffer[256] = {0};

    int written = snprintf((char*)buffer, 256, "%10u [%c] ", tick, symbol);

    if (written == -1) {
        return;
    }

    HAL_UART_Transmit(&huart2, buffer, written, 1);

    written = vsnprintf((char*)buffer, 256, fmt, args);

    if (written == -1) {
        return;
    }

    HAL_UART_Transmit(&debug_uart, buffer, written, 10);
}

} //anonymous namespace

namespace cube_hw {

void log_info(const char * fmt, ...) {
    va_list args;
    write_message('i', fmt, args);
}

void log_warning(const char * fmt, ...) {
    va_list args;
    write_message('w', fmt, args);
}

void log_error(const char * fmt, ...) {
    va_list args;
    write_message('e', fmt, args);
}

} //namespace cube
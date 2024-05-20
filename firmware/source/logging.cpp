#include <cstdarg>
#include "hardware.hpp"
#include "peripherals.hpp"
#include "printf.h"

extern "C" {
// do nothing implementation for tiny printf
void _putchar(char) {
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

    HAL_UART_Transmit(&debug_uart, buffer, written, 5);

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

    va_start(args, fmt);
    write_message('i', fmt, args);
    va_end(args);
}

void log_warning(const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    write_message('w', fmt, args);
    va_end(args);
}

void log_error(const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    write_message('e', fmt, args);
    va_end(args);
}

} //namespace cube_hw

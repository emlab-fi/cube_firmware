#pragma once
#include <stdint.h>
#include "common/message.hpp"


namespace cube_hardware {

enum class error {
    no_error,
    error
};

constexpr uint8_t LED0_MASK = 0x1U;
constexpr uint8_t LED1_MASK = 0x2U;
constexpr uint8_t LED2_MASK = 0x4U;
constexpr uint8_t LED3_MASK = 0x8U;
constexpr uint8_t LED4_MASK = 0x10U;
constexpr uint8_t LED5_MASK = 0x20U;

constexpr uint8_t A_LIMIT_START = 0x1U;
constexpr uint8_t A_LIMIT_STOP = 0x2U;
constexpr uint8_t B_LIMIT_START = 0x4U;
constexpr uint8_t B_LIMIT_STOP = 0x8U;
constexpr uint8_t C_LIMIT_START = 0x10U;
constexpr uint8_t C_LIMIT_STOP = 0x20U;

void log_info(const char * fmt, ...);

void log_warning(const char * fmt, ...);

void log_error(const char * fmt, ...);

void set_leds(uint8_t state);

cube::encoded_message get_message();

error send_message(const cube::encoded_message& msg);

error do_steps(int32_t a, int32_t b, int32_t c);

error do_velocity(int32_t a, int32_t b, int32_t c);

std::pair<error, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload);

std::pair<error, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload);

error set_gpio_mode(cube::gpio_config_payload);

error set_gpio(cube::gpio_config_payload);

std::pair<error, bool> read_gpio(cube::gpio_config_payload);

uint8_t limits_status();

}
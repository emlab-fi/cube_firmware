#pragma once
#include <optional>
#include <cstdint>
#include "common/message.hpp"


namespace cube_hw {

enum class status {
    no_error,
    error,
    msg_send_error,
    endstop_triggered,
    i2c_transfer_error,
    spi_transfer_error,
    gpio_set_error,
    gpio_read_error,
    gpio_wrong_mode
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

std::optional<cube::encoded_message> get_message();

status send_message(const cube::encoded_message& msg);

status set_motor_power(bool enabled);

status do_steps(int32_t a, int32_t b, int32_t c);

status do_velocity(int32_t a, int32_t b, int32_t c);

std::pair<status, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload*);

std::pair<status, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload*);

status set_gpio_mode(cube::gpio_config_payload*);

status set_gpio(cube::gpio_config_payload*);

std::pair<status, bool> read_gpio(cube::gpio_config_payload*);

uint8_t limits_status();

} //namespace cube_hw
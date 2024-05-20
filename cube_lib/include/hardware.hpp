#pragma once
#include <optional>
#include <cstdint>
#include "common/message.hpp"


namespace cube_hw {

enum class status {
    no_error, ///< status ok
    error, ///< generic error
    msg_send_error, ///< incorrect msg type, timeouted or internal uart error
    endstop_triggered,
    i2c_transfer_error, ///< timeout or internal I2C error
    uart_transmit_error,
    spi_transfer_error,
    gpio_set_error,
    gpio_read_error,
    gpio_wrong_mode,
    param_id_invalid,
    param_set_error
};

constexpr uint8_t A_LIMIT_START = 0x1U;
constexpr uint8_t A_LIMIT_STOP = 0x2U;
constexpr uint8_t B_LIMIT_START = 0x4U;
constexpr uint8_t B_LIMIT_STOP = 0x8U;
constexpr uint8_t C_LIMIT_START = 0x10U;
constexpr uint8_t C_LIMIT_STOP = 0x20U;

void log_info(const char * fmt, ...);

void log_warning(const char * fmt, ...);

void log_error(const char * fmt, ...);

status init_hardware();

/// @brief get received message
/// @returns newest encoded_message or nullopt if no messages present 
std::optional<cube::encoded_message> get_message();


/// @brief attempts to send encoded message
/// @param encoded_message to be sent
status send_message(const cube::encoded_message& msg);

/// @brief enables driving current or freewheel mode
status set_motor_power(bool enabled);

/// @brief set hardware param corrsponding to provided index
status set_param(uint32_t index, uint32_t value);

/// @brief get hardware param corrsponding to provided index
status get_param(uint32_t index, uint32_t& value);

/// @brief executes given number of steps for each axis
/// @param a,b,c direction is set by param sign
status do_steps(int32_t a, int32_t b, int32_t c);

/// @brief moves each of the axis to the home location
status home();

/// @brief not currently implemented () 
status do_velocity(int32_t a, int32_t b, int32_t c);

status reset_pos();

std::pair<status, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload*);

std::pair<status, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload*);

status set_gpio_mode(cube::gpio_config_payload*);

status set_gpio(cube::gpio_config_payload*);

std::pair<status, bool> read_gpio(cube::gpio_config_payload*);

uint8_t limits_status();

} //namespace cube_hw
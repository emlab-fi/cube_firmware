/// @file
/// @brief This file contains MCP23008 I2C driver wrapper

#pragma once
#include <cstdint>
#include "hardware.hpp"

namespace cube_hw {

/// @brief MCP23008 8bit I2C GPIO expander wrapper 
class MCP23008 {
    /// @brief physical device address
    const uint8_t address;

    /// @brief modifies devices register with reg_addr
    /// @param reg_addr address modifying register
    /// @param index <0-7> register bit to be modified
    /// @param value bit to be written
    status modify_register(uint8_t reg_addr, uint8_t index, bool value) const;

public:
    constexpr MCP23008(uint8_t address) : address(address) {}

    /// @brief resets device configuration
    status reset_config() const;

    /// @brief sets GPIO pin mode
    /// @param index <0-7> GPIO pin index
    /// @param mode true = input mode, false = output mode
    status set_pin_mode(uint8_t index, bool mode) const;

    /// @brief writes value to GPIO pin 
    /// @param index <0-7> GPIO pin index
    /// @param value bit to write
    status pin_write(uint8_t index, bool value) const;

    /// @brief reads value of selected GPIO pin
    /// @param index <0-7> GPIO pin index
    std::pair<status, bool> pin_read(uint8_t index) const;
};

} //namespace cube_hw

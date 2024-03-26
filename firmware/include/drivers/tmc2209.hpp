/// @file
/// @brief This file contains TMC2209 step motor driver wrapper

#pragma once
#include <cstdint>
#include <stm32g4xx_hal.h>
#include "hardware.hpp"

namespace cube_hw {

/// @brief TMC step motor driver wrapper
class TMC2209 {
    /// @brief driver address
    const uint8_t _address;
    /// @brief GPIO port for driver enable pin
    GPIO_TypeDef* enable_pin_gpio;
    /// @brief Pin number for driver enable pin
    const uint16_t enable_pin;
    /// @brief drivers enable pin state
    bool enabled = false;
    /// @brief freewheeling state
    bool freewheel = false;
    /// @brief counter to verify that register writes were successful
    uint8_t _write_counter = 0;

    /// @brief write b0-b3 to the specified register using uart
    /// @param b0 lsb
    /// @param b3 msb
    status write_reg(const uint8_t reg_addr, const uint8_t b0, const uint8_t b1, const uint8_t b2, const uint8_t b3);

    /// @brief read 4B from the specified register
    /// @param result pointer to array of size >= 4
    status read_reg(const uint8_t reg_addr, uint8_t* result);

    /// @brief verifies that all write_reg functions were successful
    status verify_writes();

    /// @brief calculates crc according to data sheet
    void append_crc(uint8_t *data, uint8_t bytes);
    
    void set_enable_pin(bool enable);
    status set_general_config();
    status set_node_config();
    status set_current_config(const uint8_t stand_still=0xa, const uint8_t running=0x1f);
    status set_stealth_chop_config();

public:
    TMC2209(const uint8_t address, GPIO_TypeDef* enable_pin_gpio, const uint16_t enable_pin);

    /// @brief configuration function
    status configure();
    status set_freewheel(bool enable);
};
   
} // cube_hw

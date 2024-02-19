/// @file
/// @brief This file contains TMC2209 step motor driver wrapper

#pragma once
#include <cstdint>
#include "hardware.hpp"

namespace cube_hw {

/// @brief TMC step motor driver wrapper
class TMC2209 {
    /// @brief driver address
    const uint8_t _address;
    /// @brief number of successful transmissions 
    uint8_t _transmission_count;

    /// @brief write b0-b3 to the specified register using uart
    /// @param b0 lsb
    /// @param b3 msb
    status write_reg(const uint8_t reg_addr, const uint8_t b0, const uint8_t b1, const uint8_t b2, const uint8_t b3);

    /// @brief read 4B from the specified register
    /// @param result pointer to array of size >= 4
    status read_reg(const uint8_t reg_addr, uint8_t* result);

    /// @brief calculates crc according to data sheet
    void append_crc(uint8_t *data, uint8_t bytes);

    status set_general_config();
    status set_node_config();
    status set_current_config();
    status set_stealth_chop_config();

public:
    TMC2209(const uint8_t address);

    /// @brief configuration function
    status configure();
};
   
} // cube_hw

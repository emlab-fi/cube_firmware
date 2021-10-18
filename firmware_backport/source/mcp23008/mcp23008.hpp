#pragma once
#include <cstdint>
#include "hardware.hpp"

namespace cube_hw {

class MCP23008 {

    const uint8_t address;

    status modify_register(uint8_t reg_addr, uint8_t index, bool value) const;

public:
    constexpr MCP23008(uint8_t address) : address(address) {}

    status reset_config() const;
    status set_pin_mode(uint8_t index, bool mode) const;
    status pin_write(uint8_t index, bool value) const;
    std::pair<status, bool> pin_read(uint8_t index) const;
};

} //namespace cube_hw
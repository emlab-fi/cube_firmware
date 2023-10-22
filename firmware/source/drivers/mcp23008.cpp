#include "drivers/mcp23008.hpp"
#include <array>
#include "peripherals.hpp"

namespace {

HAL_StatusTypeDef hal_i2c_transfer(const uint8_t addr, const uint8_t* tx, const uint8_t tx_len, uint8_t* rx, const uint8_t rx_len) {

    if (tx_len > 0) {
        auto ret = HAL_I2C_Master_Transmit(&hi2c3, addr, const_cast<uint8_t*>(tx), tx_len, 10);
        if (ret != HAL_OK) {
            return ret;
        }
    };

    if (rx_len > 0) {
        auto ret = HAL_I2C_Master_Receive(&hi2c3, addr, rx, rx_len, 10);
        if (ret != HAL_OK) {
            return ret;
        }
    };

    return HAL_OK;
}

} // anonymous namespace

namespace cube_hw {

status MCP23008::reset_config() const {
    constexpr std::array<uint8_t, 7> data = {
        0x00, //ADDR zero
        0x00, //IODIR - set all to output
        0x00, //IPOL - normal porality
        0x00, //GPINTEN - disable interrupts
        0x00, //DEFVAL - all zero
        0x00, //INTCON - all zero
        0x00  //IOCON - default
    };

    if (hal_i2c_transfer(address, data.data(), 7, nullptr, 0) != HAL_OK) {
        return status::i2c_transfer_error;
    }
    return status::no_error;
}

status MCP23008::modify_register(uint8_t reg_addr, uint8_t index, bool value) const {
    log_info("cube_hw: mcp23008: modifying reg: %x at pos %x\n", reg_addr, index);
    uint8_t current_value = 0U;

    if (hal_i2c_transfer(address, &reg_addr, 1, &current_value, 1) != HAL_OK) {
        log_error("cube_hw: mcp23008: register read fail\n");
        return status::gpio_set_error;
    }

    uint8_t new_value = current_value;
    if (value) {
        new_value |= (1U << index);
    } else {
        new_value &= ~(1U << index);
    }

    std::array<uint8_t, 2> data = {
        reg_addr,
        new_value
    };

    if (hal_i2c_transfer(address, data.data(), 2, nullptr, 0) != HAL_OK) {
        log_error("cube_hw: mcp23008: register write fail\n");
        return status::gpio_set_error;
    }
    return status::no_error;
}

status MCP23008::set_pin_mode(uint8_t index, bool value) const {
    return modify_register(IO_DIR_REG_ADDR, index, value);
}

status MCP23008::pin_write(uint8_t index, bool value) const {
    return modify_register(GPIO_DATA_REG_ADDR, index, value);
}

std::pair<status, bool> MCP23008::pin_read(uint8_t index) const {
    constexpr uint8_t reg_addr = GPIO_DATA_REG_ADDR;
    uint8_t current_value = 0U;

    if (hal_i2c_transfer(address, &reg_addr, 1, &current_value, 1) != HAL_OK) {
        log_error("cube_hw: mcp23008: register read fail\n");
        return {status::gpio_read_error, false};
    }

    return {status::no_error, (current_value & (1U << index))};
}

} //namespace cube_hw

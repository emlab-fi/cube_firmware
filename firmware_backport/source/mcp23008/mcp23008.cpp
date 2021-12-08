#include "mcp23008.hpp"
#include <array>
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

namespace {

void delay(int counter) {
	for (int i = 0; i < counter; i++) {
		__asm volatile ("nop");
	}
}

status_t hal_i2c_transfer(const uint8_t addr, const uint8_t* tx, const uint8_t tx_len, uint8_t* rx, const uint8_t rx_len) {

    status_t result;
    result = I2C_MasterStart(I2C0_PERIPHERAL, addr, kI2C_Write);
    if (result != kStatus_Success) {
        I2C_MasterClearStatusFlags(I2C0_PERIPHERAL, kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag);
        I2C_MasterStop(I2C0_PERIPHERAL);
        return result;
    }
    delay(1200);
    result = I2C_MasterWriteBlocking(I2C0_PERIPHERAL, tx, tx_len, kI2C_TransferNoStopFlag);
    if (result != kStatus_Success) {
        I2C_MasterClearStatusFlags(I2C0_PERIPHERAL, kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag);
        I2C_MasterStop(I2C0_PERIPHERAL);
        return result;
    }
    delay(1200);
    result = I2C_MasterRepeatedStart(I2C0_PERIPHERAL, addr, kI2C_Read);
    if (result != kStatus_Success) {
        I2C_MasterClearStatusFlags(I2C0_PERIPHERAL, kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag);
        I2C_MasterStop(I2C0_PERIPHERAL);
        return result;
    }
    delay(1200);
    result = I2C_MasterReadBlocking(I2C0_PERIPHERAL, rx, rx_len, kI2C_TransferDefaultFlag);
    if (result != kStatus_Success) {
        I2C_MasterClearStatusFlags(I2C0_PERIPHERAL, kI2C_ArbitrationLostFlag | kI2C_IntPendingFlag);
        I2C_MasterStop(I2C0_PERIPHERAL);
        return result;
    }
    delay(1200);
    return result;
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
        0x00 //IOCON - default
    };

    if (hal_i2c_transfer(address, data.data(), 7, nullptr, 0) != kStatus_Success) {
        return status::i2c_transfer_error;
    }
    return status::no_error;
}

status MCP23008::modify_register(uint8_t reg_addr, uint8_t index, bool value) const {
    log_info("cube_hw: mcp23008: modifying reg: %x at pos %d\n", reg_addr, index);
    uint8_t current_value = 0U;

    if (hal_i2c_transfer(address, &reg_addr, 1, &current_value, 1) != kStatus_Success) {
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

    if (hal_i2c_transfer(address, data.data(), 2, nullptr, 0) != kStatus_Success) {
        log_error("cube_hw: mcp23008: register write fail\n");
        return status::gpio_set_error;
    }
    return status::no_error;
}

status MCP23008::set_pin_mode(uint8_t index, bool value) const {
    return modify_register(0x00, index, value);
}

status MCP23008::pin_write(uint8_t index, bool value) const {
    return modify_register(0x09, index, value);
}

std::pair<status, bool> MCP23008::pin_read(uint8_t index) const {
    constexpr uint8_t reg_addr = 0x09;
    uint8_t current_value = 0U;

    if (hal_i2c_transfer(address, &reg_addr, 1, &current_value, 1) != kStatus_Success) {
        log_error("cube_hw: mcp23008: register read fail\n");
        return {status::gpio_read_error, false};
    }

    return {status::no_error, (current_value & (1U << index))};
}

} //namespace cube_hw
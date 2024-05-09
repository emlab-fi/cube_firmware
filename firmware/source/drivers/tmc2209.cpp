#include <cmath>
#include "peripherals.hpp"
#include "config.hpp"

// header 0b10100000
// address 0..3

// Write 64bit
// schema [header, address, reg address + 1(write mode), data[4], crc]

// Read req 32bit
// schema [header, address, reg address + 0(read mode), crc]

// Read resp 64bit
// schema [header, address, reg address + 0(read mode), data[4], crc]

constexpr uint8_t TMC_UART_HEADER = 0b00000101;

constexpr uint8_t GCONF_REG_ADDR = 0x00;        // general config
constexpr uint8_t GSTAT_REG_ADDR = 0x01;        // status register
constexpr uint8_t IFCNT_REG_ADDR = 0x02;        // Interface transmission counter
constexpr uint8_t NODECONF_REG_ADDR = 0x03;     // Nodeconf - response delay
constexpr uint8_t IOIN_REG_ADDR = 0x06;         // Nodeconf - response delay
constexpr uint8_t FACTORY_CONF_REG_ADDR = 0x07; // Freq trim(12Mhz) and Over temperature trim(157˚C)

constexpr uint8_t IHOLD_IRUN_REG_ADDR = 0x10;   // current settings
constexpr uint8_t TPOWERDOWN_REG_ADDR = 0x11;   // power down delay after stst detection
constexpr uint8_t TSTEP_REG_ADDR = 0x12;
constexpr uint8_t TPWMTHRS_REG_ADDR = 0x13;     // automatic switching to chop cycle mode threshold

constexpr uint8_t MSCNT_REG_ADDR = 0x6A;
constexpr uint8_t MSCURACT_REG_ADDR = 0x6B;

constexpr uint8_t CHOPCONF_REG_ADDR = 0x6C;
constexpr uint8_t DRV_STATUS_REG_ADDR = 0x6F;
constexpr uint8_t PWMCONF_REG_ADDR = 0x70;
constexpr uint8_t PWM_SCALE_REG_ADDR = 0x71;
constexpr uint8_t PWM_AUTO_REG_ADDR = 0x72;

namespace cube_hw {

TMC2209::TMC2209(const uint8_t address, GPIO_TypeDef* enable_pin_gpio, const uint16_t enable_pin) : 
    _address(address),
    enable_pin_gpio(enable_pin_gpio),
    enable_pin(enable_pin) {}

status TMC2209::configure(uint32_t mode_tresh) {
    uint8_t read_buffer[4];

    set_enable_pin(true);

    // init write counter
    auto retval = read_reg(IFCNT_REG_ADDR, read_buffer);
    if (retval != status::no_error)
        return retval;

    _write_counter = read_buffer[0];

    // write default configuration
    if (set_node_config() != status::no_error ||
        set_general_config() != status::no_error ||
        set_current_config() != status::no_error ||
        set_stealth_chop_config(mode_tresh) != status::no_error)
        return status::uart_transmit_error;
    
    // check that driver received configuration 
    return verify_writes();
}

void TMC2209::set_enable_pin(bool enable) {
    if (enable && !enabled) {
        HAL_GPIO_WritePin(enable_pin_gpio, enable_pin, GPIO_PIN_RESET); // actual pin is reversed
    } else if (!enable && enabled) {
        HAL_GPIO_WritePin(enable_pin_gpio, enable_pin, GPIO_PIN_SET); // actual pin is reversed
    }
    enabled = enable;
}

status TMC2209::set_freewheel(bool enable_freewheel) {
    status retval;
    // check previous state
    if (enable_freewheel && !freewheel) {
        retval = set_current_config(0, 0);
    } else if (!enable_freewheel && freewheel) {
        retval = set_current_config();
    } else {
        return status::no_error;
    }

    if (retval != status::no_error) {
        return retval;
    }

    uint8_t read_buffer[4];
    retval = read_reg(PWMCONF_REG_ADDR, read_buffer);
    if (retval != status::no_error) {
        return retval;
    }

    // bits 21-20: %00=normal; %01=freewheel
    read_buffer[2] &= ~0xf0;
    read_buffer[2] |= enable_freewheel ? 0x10 : 0;

    retval = write_reg(PWMCONF_REG_ADDR, read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3]);
    if (retval != status::no_error)
        return retval;

    freewheel = enable_freewheel;
    return verify_writes();
}

void TMC2209::append_crc(uint8_t* data, uint8_t bytes) {
    int i, j;
    uint8_t* crc = data + (bytes - 1); // CRC located in last byte of message
    uint8_t currentByte;
    *crc = 0;
    for (i = 0; i < (bytes - 1); ++i) {
        currentByte = data[i];
        for (j = 0; j < 8; ++j) {
            if ((*crc >> 7) ^ (currentByte & 0x01)) { // update CRC based result of XOR operation
                *crc = (*crc << 1) ^ 0x07;
            }
            else {
                *crc = (*crc << 1);
            }
            currentByte = currentByte >> 1;
        }
    }
}

status TMC2209::write_reg(const uint8_t reg_addr, const uint32_t value) {
    return write_reg(reg_addr, 
        value & 0xff,
        (value >> 8) & 0xff,
        (value >> 16) & 0xff,
        (value >> 24) & 0xff);
}

status TMC2209::write_reg(const uint8_t reg_addr, const uint8_t b0, const uint8_t b1, const uint8_t b2, const uint8_t b3) {
    uint8_t data[8];
    data[0] = TMC_UART_HEADER;          
    data[1] = _address;
    data[2] = reg_addr | 0x80; // write flag
    
    data[3] = b3;
    data[4] = b2;
    data[5] = b1;
    data[6] = b0;

    append_crc(data, 8);

    if (HAL_HalfDuplex_EnableTransmitter(&tmc_uart) != HAL_OK)
        return status::error;

    if (HAL_UART_Transmit(&tmc_uart, data, 8, 10) != HAL_OK)
        return status::uart_transmit_error;

    ++_write_counter;
    return status::no_error;
}
 
status TMC2209::read_reg(const uint8_t reg_addr, uint8_t* result) {
    uint8_t request_data[4], response[8];
    request_data[0] = TMC_UART_HEADER;
    request_data[1] = _address;
    request_data[2] = reg_addr;
    append_crc(request_data, 4);

    if (HAL_HalfDuplex_EnableTransmitter(&tmc_uart) != HAL_OK) 
        return status::error;

    if (HAL_UART_Transmit(&tmc_uart, request_data, 4, 10) != HAL_OK)
        return status::uart_transmit_error;

    if (HAL_HalfDuplex_EnableReceiver(&tmc_uart) != HAL_OK) 
        return status::error;
    
    if (HAL_UART_Receive(&tmc_uart, response, 8, 100) != HAL_OK)
        return status::uart_transmit_error;
    
    result[0] = response[6];
    result[1] = response[5];
    result[2] = response[4];
    result[3] = response[3];

    return status::no_error;
}

status TMC2209::verify_writes() {
    uint8_t read_buffer[4];
    const auto retval = read_reg(IFCNT_REG_ADDR, read_buffer);
    if (retval != status::no_error)
        return retval;

    if (read_buffer[0] != _write_counter)
        return status::uart_transmit_error;

    return status::no_error;
}

status TMC2209::set_general_config() {
    // byte 0
    // b0 0 -> internal reference,          1 -> use VREF
    // b1 0 -> external sense resistor      1 -> internal sense resistor
    // b2 0 -> stealth chop                 1 -> spread cycle             
    // b3 0 -> default direction,           1 -> inverse direction
    // b4 0 -> Index = microstep pos        1 -> Index = overtemperature prewarning
    // b5 0 -> Index as previous b4         1 -> Index = internal pulse generator output
    // b6 0 -> PDN_UART available           1 -> PDN_UART pin disabled
    // b7 0 -> microstep resol by MS1 MS2   1 -> microstep resol in MSER reg
    constexpr uint8_t b0 = 0b11000000;

    // byte 1
    // b0 0 -> No filtering of STEP pulses  1 -> Software pulse generator optimization enabled
    // b1 0 -> normal operation             1 -> test mode 
    constexpr uint8_t b1 = 0b00000000;

    return write_reg(GCONF_REG_ADDR, b0, b1, 0, 0);
}

status TMC2209::set_node_config() {
    constexpr uint8_t response_delay = 0x3; // 3 bauds response delay
    return write_reg(NODECONF_REG_ADDR, 0, response_delay, 0, 0);
}

status TMC2209::set_current_config(const uint8_t stand_still, const uint8_t running) {
    //constexpr uint8_t stand_still = 0x0a;   // lowest standstill current
    //constexpr uint8_t running = 0x1f;       // max running current
    constexpr uint8_t stop_delay = 0x3;     // n * 2^18 clocks

    return write_reg(IHOLD_IRUN_REG_ADDR, stand_still, running, stop_delay, 0);
}

status TMC2209::set_stealth_chop_config(uint32_t threshold) {
    // disables switching to spread cycle
    if (threshold == 0) {
        return write_reg(TPWMTHRS_REG_ADDR, 0, 0, 0, 0);
    }
    return write_reg(TPWMTHRS_REG_ADDR, threshold);
}

uint32_t TMC2209::get_tstep() {
    uint8_t read_buffer[4];
    const auto retval = read_reg(TSTEP_REG_ADDR, read_buffer);
    if (retval != status::no_error)
        return 0xffffffff;

    return read_buffer[0] | (read_buffer[1] << 8) | (read_buffer[2] << 16);
}

status TMC2209::set_tpwm_thrs(uint32_t value) {
    const auto retval = write_reg(TPWMTHRS_REG_ADDR, value);
    if (retval != status::no_error)
        return retval;

    return verify_writes();
}

} // cube_hw

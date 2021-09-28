#include <stdint.h>
#include <array>

#include "TMC429.hpp"
#include "fsl_common.h"
#include "MK64F12.h"
#include "fsl_dspi.h"

namespace {
/*
converts int to uint in TwosComplement
then cuts off highest bytes to make it only 24bit/12bit
works via unsigned int overflow/underflow, is hacky
surprisingly, works even with -03   O_o
*/
uint32_t twosComplement24bit(int32_t input) {
    uint32_t result = 0;
    result += input;
    return result & 0x00FFFFFF;
}

uint32_t twosComplement12bit(int16_t input) {
    uint32_t result = 0;
    result += input;
    return result & 0x00000FFF;
}

} // namespace


namespace TMC429_Driver {

Status TMC429::readRegister(uint8_t address, uint32_t* data) {
    uint8_t rx[4];
    uint8_t tx[4] = {0, 0, 0, 0};

    tx[0] = (address << ADDR_OFFSET) | 1U;
    Status result = config.transfer(tx, rx, 4);
    if (result != Status::success) {
        return result;
    }

    uint32_t temp = (rx[0] << 24) | (rx[1] << 16) | (rx[2] << 8) | rx[3];
    *data = temp;
    return result;
}


Status TMC429::writeRegister(uint8_t address, uint32_t data) {
    uint8_t rx[4];
    uint8_t tx[4];
    
    tx[0] = (address << ADDR_OFFSET) | 0U;
    tx[1] = (data & 0x00FF0000) >> 16;
    tx[2] = (data & 0x0000FF00) >> 8;
    tx[3] = (data & 0x000000FF);

    return config.transfer(tx, rx, 4);
}

Status TMC429::uploadStepperConfig(uint8_t stepper) {
    struct data{
         uint8_t addr;
         uint32_t data;
    };
    uint8_t prefix = stepper << ADDR_PREFIX_OFFSET;
    const StepperConfig & c = config.steppers[stepper];

    std::array< data, 7 > dataset;
    dataset[0] = {prefix | V_MIN_ADDR,            c.v_min};
    dataset[1] = {prefix | V_MAX_ADDR,            c.v_max};
    dataset[2] = {prefix | A_MAX_ADDR,            c.a_max};
    dataset[3] = {prefix | PMUL_PDIV_ADDR,        c.pmul << PMUL_OFFSET | c.pdiv << PDIV_OFFSET};
    dataset[4] = {prefix | CONF_ADDR,             0b1111 << REF_CONF_OFFSET | RampMode::POSITION << R_M_OFFSET};
    dataset[5] = {prefix | INTERRUPT_ADDR,        1U << INTERRUPT_MASK_OFFSET};
    dataset[6] = {prefix | PULSEDIV_RAMPDIV_ADDR, c.pulse_div << PULSE_DIV_OFFSET | c.ramp_div << RAMP_DIV_OFFSET};

    for(const data & d : dataset ){
        Status result = writeRegister(d.addr, d.data);
        if (result != Status::success) {
            return result;
        }
    }
    return Status::success;
}


Status TMC429::uploadConfig() {
    configValid = false;
    Status result;

    result = writeRegister(IF_CONF_ADDR, 0b000100010);
    if (result != Status::success) {
            return result;
    }
    result = writeRegister(MAIN_CONF_ADDR, 15U << STPDIV_OFFSET);
    if (result != Status::success) {
            return result;
    }

    for (uint8_t i = 0; i < 3U; i++) {
        result = uploadStepperConfig(i);
        if (result != Status::success) {
            return result;
        }
    }

    configValid = true;
    return Status::success;
}


StepperStatus TMC429::getStepperStatus(void) {
    uint32_t data = 0;
    StepperStatus result;
    result.status = readRegister(VERSION_ADDR, &data);
    result.stepper1_pos = data & STEPPER1_POS_FLAG;
    result.stepper2_pos = data & STEPPER2_POS_FLAG;
    result.stepper3_pos = data & STEPPER3_POS_FLAG;
    return result;
}

Status TMC429::setMode(RampMode mode) {
    for (uint8_t i = 0; i < 3U; i++) {
        uint8_t addr = i << ADDR_PREFIX_OFFSET | CONF_ADDR;
        uint32_t data = 0b1111 << REF_CONF_OFFSET | mode << R_M_OFFSET;
        Status result = writeRegister(addr, data);

        if (result != Status::success) {
            configValid = false;
            return result;
        }
    }

    currentMode = mode;
    return Status::success;
}

Status TMC429::doSteps(int32_t axis1, int32_t axis2, int32_t axis3) {
    if (currentMode != RampMode::POSITION) {
        return Status::wrong_mode;
    }

    uint32_t data[3] = {axis1, axis2, axis3};

    for (int i = 0; i < 3; i++) {
        Status result = writeRegister(i << ADDR_PREFIX_OFFSET | X_TARGET_ADDR, twosComplement24bit(data[i])); 
        if (result != Status::success) {
            return result;
        }
    }
    return Status::success;
}

Status TMC429::setSpeed(int16_t axis1, int16_t axis2, int16_t axis3) {
    if (currentMode != RampMode::VELOCITY) {
        return Status::wrong_mode;
    }
    
    uint32_t data[3] = {axis1, axis2, axis3};

    for (int i = 0; i < 3; i++) {
        Status result = writeRegister(i << ADDR_PREFIX_OFFSET | V_TARGET_ADDR, twosComplement12bit(data[i])); 
        if (result != Status::success) {
            return result;
        }
    }

    return Status::success;
}

Status TMC429::setPosition(int32_t axis1, int32_t axis2, int32_t axis3) {
    if (currentMode != RampMode::VELOCITY) {
        return Status::wrong_mode;
    }

    uint32_t data[3] = {axis1, axis2, axis3};

    for (int i = 0; i < 3; i++) {
        Status result = writeRegister(i << ADDR_PREFIX_OFFSET | X_TARGET_ADDR, twosComplement24bit(data[i])); 
        if (result != Status::success) {
            return result;
        }
        result = writeRegister(i << ADDR_PREFIX_OFFSET | X_ACTUAL_ADDR, data[i]); 
        if (result != Status::success) {
            return result;
        }
    }
    return Status::success;
}

Status TMC429::setMaxSpeed(uint16_t axis1, uint16_t axis2, uint16_t axis3) {
    uint16_t data[3] = {axis1, axis2, axis3};

    for (int i = 0; i < 3; i++) {
        Status result = writeRegister(i << ADDR_PREFIX_OFFSET | V_MAX_ADDR, data[i]); 
        if (result != Status::success) {
            return result;
        }
    }
    return Status::success;
}

} // namespace TMC429_Driver
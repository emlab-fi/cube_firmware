#pragma once

#include <stdint.h>

#include "fsl_common.h"
#include "MK64F12.h"


namespace TMC429_Driver {


//Register masks, offsets, addresses
constexpr uint8_t ADDR_OFFSET = 1;
constexpr uint8_t ADDR_PREFIX_OFFSET = 4;

constexpr uint8_t X_TARGET_ADDR = 0b0000;
constexpr uint8_t X_ACTUAL_ADDR = 0b0001;
constexpr uint8_t V_MIN_ADDR = 0b0010;
constexpr uint8_t V_MAX_ADDR = 0b0011;
constexpr uint8_t V_TARGET_ADDR = 0b0100;
constexpr uint8_t A_MAX_ADDR = 0b0110;
constexpr uint8_t PMUL_PDIV_ADDR = 0b1001;
constexpr uint8_t CONF_ADDR = 0b1010;
constexpr uint8_t INTERRUPT_ADDR = 0b1011;
constexpr uint8_t PULSEDIV_RAMPDIV_ADDR = 0b1100;

constexpr uint8_t IF_CONF_ADDR = 0b0110100;
constexpr uint8_t MAIN_CONF_ADDR = 0b0111111;
constexpr uint8_t VERSION_ADDR = 0b0111001;

constexpr uint8_t PDIV_OFFSET = 0;
constexpr uint8_t PMUL_OFFSET = 8;

constexpr uint8_t R_M_OFFSET = 0;
constexpr uint8_t REF_CONF_OFFSET = 8;

constexpr uint8_t INTERRUPT_FLAGS_OFFSET = 0;
constexpr uint8_t INTERRUPT_MASK_OFFSET = 8;

constexpr uint8_t PULSE_DIV_OFFSET = 12;
constexpr uint8_t RAMP_DIV_OFFSET = 8;

constexpr uint8_t STPDIV_OFFSET = 8;

constexpr uint32_t STEPPER1_POS_FLAG = (1U << 24);
constexpr uint32_t STEPPER2_POS_FLAG = (1U << 26);
constexpr uint32_t STEPPER3_POS_FLAG = (1U << 28);


enum class Status {
    success,
    fail,
    wrong_mode
};

struct StepperConfig {
    uint16_t v_min;
    uint16_t v_max;
    uint16_t a_max;
    uint8_t pmul;
    uint8_t pdiv;
    uint8_t ramp_div;
    uint8_t pulse_div;
};

struct Config {
    Status (*transfer)(uint8_t* tx, uint8_t* rx, uint8_t length);
    StepperConfig steppers[3];
};

struct StepperStatus {
    Status status;
    bool stepper1_pos;
    bool stepper2_pos;
    bool stepper3_pos;
};

enum RampMode : uint8_t {
    VELOCITY = 2,
    POSITION = 0
};

class TMC429 {
    Config config;
    
    Status readRegister(uint8_t address, uint32_t* data);
    Status writeRegister(uint8_t address, uint32_t data);
    Status uploadStepperConfig(uint8_t stepper);
    Status uploadConfig();
public:
    bool configValid;
    RampMode currentMode;
    TMC429(const Config& conf) : config(conf),
                                   configValid(false),
                                   currentMode(RampMode::POSITION)
    {
        uploadConfig();
    }

    const Status changeConfig(Config& conf) {
        config = conf;
        return uploadConfig();
    }

    StepperStatus getStepperStatus(void);
    
    Status setMode(RampMode mode);

    
    Status doSteps(int32_t axis1, int32_t axis2, int32_t axis3);
    Status setSpeed(int16_t axis1, int16_t axis2, int16_t axis3);
    Status setPosition(int32_t axis1, int32_t axis2, int32_t axis3);
    Status setMaxSpeed(uint16_t axis1, uint16_t axis2, uint16_t axis3);
};

} // namespace TMC429_Driver
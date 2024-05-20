#pragma once
#include <array>
#include <vector>
#include <cinttypes>
#include <cmath>
#include <stm32g4xx_hal.h>
#include "hardware.hpp"
#include "config.hpp"


namespace cube_hw
{

enum class motor_state{
    RESET,      // not initialized
    IDLE,       // standby
    READY,      // dma ready
    SKIP,       // dma ready with 0 steps
    BUSY,       // running    
    VELOCITY    // in velocity mode
};

struct MotorPins {
    TIM_HandleTypeDef& htim;
    const uint16_t channel;
    GPIO_TypeDef* dir_gpio;
    const uint16_t dir_pin;

    MotorPins(TIM_HandleTypeDef& htim, const uint16_t channel, GPIO_TypeDef* dir_gpio, const uint16_t dir_pin);
    MotorPins(const MotorPins&) = delete;
    MotorPins& operator=(const MotorPins&) = delete;
};

/// @brief generator of PWM for stepper motor
class StepperGenerator {
    const MotorPins& pins;
    int dma_burst_padding;
    uint32_t dma_burst_length;
    const unsigned _steps_per_mm;
    motor_state _state = motor_state::RESET;
    float ratio = 1.0f;
    motor_config config;

    // Dma
    int acc_idx = 0;
    int dec_idx = 0;
    std::vector<uint16_t> acceleration_mem;
    std::vector<uint16_t> deceleration_mem;
    void insert_section(uint16_t auto_reload, uint16_t repetition_c, bool is_acceleration=true);
    void finalize_dma(uint16_t auto_reload);

    // Math helpers
    int32_t acceleration_steps(const float v0, const float v1, const float a);
    int32_t constant_steps(const float v);
    uint16_t get_arr(const float speed);
    
    // Steps preparation
    void set_direction(bool forward);
    std::pair<int32_t, int32_t> ramp_steps(const unsigned ramp, const float target);
    bool fit_ramp(const unsigned ramp, int32_t& steps);
    float create_reduced_ramp(const unsigned ramp, int32_t& steps);
    void generate_slope(const int32_t steps, const unsigned ramp, const uint16_t target_arr, bool is_acceleration=true);
    void generate_constant(int32_t steps, const float speed);

public:
    StepperGenerator(const MotorPins& pins, unsigned resolution, motor_config config);
    StepperGenerator(const StepperGenerator&) = delete;
    StepperGenerator& operator=(const StepperGenerator&) = delete;

    /// @brief starts the timer base
    status start_tim_base();

    /// @brief set generator parameter
    status set_param(uint32_t index, uint32_t value);

    /// @brief set generator parameter
    status get_param(uint32_t index, uint32_t& value);

    /// @brief htim tied to this step generator
    const TIM_HandleTypeDef* htim() const;

    /// @brief get current motor state
    motor_state state() const;

    /// @brief precalculates acceleration curve
    status prepare_dma(int32_t steps, float ratio=1.0f);

    /// @brief starts dma transfer
    status start();

    /// @brief moves at speed until according limit is hit
    status do_velocity(float speed);

    /// @brief must be called after the dma finished transfer
    void finished_callback();

    /// @brief called in interrupt handler 
    void limit_hit();
};

} // namespace cube_hw

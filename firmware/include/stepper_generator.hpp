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

constexpr int RAMPS = 3;

enum class motor_state{
    RESET,      // not initialized
    IDLE,       // standby
    READY,      // dma ready
    SKIP,       // dma ready with 0 steps
    BUSY,       // running
    VELOCITY    // in velocity mode
};

/// @brief generator of PWM for stepper motor
class StepperGenerator {
    TIM_HandleTypeDef& _htim;
    const unsigned _channel;
    const unsigned _steps_per_mm;
    motor_state _state = motor_state::RESET;

    // Variables for 8 point movement
    float ratio = 1.0f;
    float _const_speed_t = 0.1;
    float _section_duration = 0.02;
    unsigned start_v = 10;
    unsigned end_v = 20;
    std::array<float, RAMPS> speed_points = {30, 100, 130};
    std::array<float, RAMPS> accelerations = {100, 300, 100};
    std::array<float, RAMPS> decelerations = {150, 400, 100};

    // Dma storage
    std::vector<uint16_t> acceleration_mem;
    std::vector<uint16_t> deceleration_mem;

    int32_t acceleration_steps(const float v0, const float v1, const float a);
    int32_t constant_steps(const float v);
    uint16_t get_arr(const float speed);
    std::pair<int32_t, int32_t> ramp_steps(const unsigned ramp, const float target);
    float start_speed(const unsigned ramp, bool is_acceleration=true);
    float target_speed(const unsigned ramp);
    float acceleration(const unsigned ramp);
    float deceleration(const unsigned ramp);
    bool fit_ramp(const unsigned ramp, int32_t& steps);
    float create_reduced_ramp(const unsigned ramp, int32_t& steps);
    float get_reduced_target(const unsigned ramp, const float reducer);

    void insert_section(uint16_t auto_reload, uint16_t repetition_c, bool is_acceleration=true);
    void generate_slope(const int32_t steps, const unsigned ramp, const uint16_t target_arr, bool is_acceleration=true);
    void generate_constant(int32_t steps, const float speed);
    void finalize_dma(uint16_t auto_reload);

public:
    StepperGenerator(TIM_HandleTypeDef& htim, unsigned channel, unsigned resolution);

    /// @brief starts the timer base
    status start_tim_base();

    /// @brief htim tied to this step generator
    const TIM_HandleTypeDef* htim() const;

    /// @brief get current motor state
    motor_state state() const;

    /// @brief sets the given direction pin
    void set_direction(bool forward, GPIO_TypeDef* PORT, const int PIN);

    /// @brief precalculates acceleration curve
    status prepare_dma(int32_t steps, float ratio=1.0f);

    /// @brief starts dma transfer
    status start();

    /// @brief must be called after the dma finished transfer
    void finished_callback();
};

} // namespace cube_hw
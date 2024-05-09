#pragma once
#include "hardware.hpp"
#include "planner.hpp"
#include "params.hpp"

/// @brief hardware ratios for cube_lib
constexpr cube::planner_config planner_conf = {
        .machine = cube::planner_machine::corexy,
        .bounds = cube::point(150.0f, 150.0f, 150.0f),
        .step_resolution_a = 1600U,
        .step_resolution_b = 1600U,
        .step_resolution_c = 12800U
};

namespace cube_hw
{

constexpr uint32_t DEFAULT_CONST_SPEED_T = 10;  // ms
constexpr uint32_t DEFAULT_SECTION_T = 20;      // ms
constexpr int32_t TIM_CLOCK = 16000000;         // Hz
constexpr int RAMPS = 3;

// non const attributes are configurable
struct motor_config {
    // Time
    uint32_t _const_speed_t = DEFAULT_CONST_SPEED_T;
    uint32_t _section_t = DEFAULT_SECTION_T;

    // Ramping
    uint32_t start_v;
    uint32_t end_v;
    std::array<uint32_t, RAMPS> speed_points;  // section speed id=0x10-0x1f
    std::array<uint32_t, RAMPS> accelerations; // section speed id=0x20-0x2f
    std::array<uint32_t, RAMPS> decelerations; // section speed id=0x30-0x3f
    uint32_t max_speed;
    uint32_t max_acceleration;
    
    // Velocity
    uint32_t _homing_velocity;
    const float min_velocity;
    const float max_velocity;

    // Float access
    float ratio = 1.0f;
    float const_speed_t() const;
    float section_t() const;
    float homing_velocity() const;
    float start_speed(const unsigned ramp, bool is_acceleration=true) const;
    float target_speed(const unsigned ramp) const;
    float acceleration(const unsigned ramp) const;
    float deceleration(const unsigned ramp) const;
    float reduced_target(const unsigned idx, const float reducer) const;
    
    // Params
    uint32_t* param(uint32_t param_id);
    status set_param(uint32_t param_id, uint32_t value);
    status get_param(uint32_t param_id, uint32_t& value);
};
} // namespace cube_hw

constexpr cube_hw::motor_config CoreConfig = {
    .start_v = 30,
    .end_v = 40,
    .speed_points = {70, 160, 200},
    .accelerations = {300, 800, 300},
    .decelerations = {400, 900, 400},
    .max_speed = 1000,
    .max_acceleration = 2000,
    ._homing_velocity = 40,
    .min_velocity = 1.0f,
    .max_velocity = 60.0f,
};

constexpr cube_hw::motor_config VerticalConfig = {
    .start_v = 2,
    .end_v = 4,
    .speed_points = {4, 12, 15},
    .accelerations = {10, 40, 10},
    .decelerations = {20, 50, 20},
    .max_speed = 20,
    .max_acceleration = 100,
    ._homing_velocity = 5,
    .min_velocity = 0.1f,
    .max_velocity = 10.0f
};

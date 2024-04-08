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

constexpr float DEFAULT_CONST_SPEED_T = 0.1f;
constexpr float DEFAULT_SECTION_DURATION = 0.02f;
constexpr int32_t TIM_CLOCK = 16000000; // Hz
constexpr int RAMPS = 3;

// non const attributes are configurable
struct motor_config {
    // Time
    float const_speed_t = DEFAULT_CONST_SPEED_T;
    float section_duration = DEFAULT_SECTION_DURATION;

    // Ramping
    float start_v;
    float end_v;
    std::array<float, RAMPS> speed_points;  // section speed id=0x10-0x1f
    std::array<float, RAMPS> accelerations; // section speed id=0x20-0x2f
    std::array<float, RAMPS> decelerations; // section speed id=0x30-0x3f
    float max_speed;
    float max_acceleration;
    
    // Velocity
    const float homing_velocity;
    const float min_velocity;
    const float max_velocity;

    // Convenient access
    float ratio = 1.0f;
    float start_speed(const unsigned ramp, bool is_acceleration=true);
    float target_speed(const unsigned ramp);
    float acceleration(const unsigned ramp);
    float deceleration(const unsigned ramp);
    float reduced_target(const unsigned idx, const float reducer);
    
    // Params
    status set_param(uint32_t param_id, uint32_t value);
    status get_param(uint32_t param_id, uint32_t& value);
};
} // namespace cube_hw

constexpr cube_hw::motor_config CoreConfig = {
    .start_v = 10.0f,
    .end_v = 20.0f,
    .speed_points = {30, 120, 150},
    .accelerations = {100, 400, 100},
    .decelerations = {200, 500, 200},
    .max_speed = 170.0f,
    .max_acceleration = 700,
    .homing_velocity = 20.0f,
    .min_velocity = 1.0f,
    .max_velocity = 50.0f
};

constexpr cube_hw::motor_config VerticalConfig = {
    .start_v = 2.0f,
    .end_v = 4.0f,
    .speed_points = {4, 12, 15},
    .accelerations = {10, 40, 10},
    .decelerations = {20, 50, 20},
    .max_speed = 20.0f,
    .max_acceleration = 100,
    .homing_velocity = 5.0f,
    .min_velocity = 0.1f,
    .max_velocity = 10.0f
};

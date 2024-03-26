#pragma once
#include "planner.hpp"

constexpr cube::planner_config planner_conf = {
        .machine = cube::planner_machine::corexy,
        .bounds = cube::point(150.0f, 150.0f, 150.0f),
        .step_resolution_a = 1600U,
        .step_resolution_b = 1600U,
        .step_resolution_c = 1600U
};

constexpr int32_t TIM_CLOCK = 16000000; // Hz
constexpr int32_t TIM_CLOCK_kHz = TIM_CLOCK / 1000;
constexpr float HOMING_SPEED = 20.0f;
constexpr float MIN_VELOCITY = 1.0f;
constexpr float MAX_VELOCITY = 50.0f;

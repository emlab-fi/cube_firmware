#pragma once
#include "planner.hpp"

constexpr cube::planner_config planner_conf = {
        .machine = cube::planner_machine::corexy,
        .bounds = cube::point(300.0f, 300.0f, 100.0f),
        .step_resolution_a = 1600U,
        .step_resolution_b = 1600U,
        .step_resolution_c = 1600U
};

constexpr int START_RESOLUTION = 12;
constexpr int STOP_RESOLUTION = 8;
constexpr int MAX_SPEED_PADDING = planner_conf.step_resolution_a * planner_conf.bounds[0] / 65000 + 1;
constexpr int STOP_PADDING = 2; // (DMA) padding used for triggering interrupt at correct time
constexpr bool USE_LINEAR = true;

// 120 - 360 / 450
// 120 - 450 / 600
// 120 - 720 / 1200
// 160 - 640 / 800
// 160 - 800 / 1280

constexpr int MAX_SPEED = 120;          // mm/s
constexpr int ACCELERATION = 450;       // mm/s^2
constexpr int DECELERATION = 600;      // mm/s^2
constexpr int32_t TIM_CLOCK = 16000000; // Hz

constexpr int DMA_MEM_SIZE = (START_RESOLUTION + MAX_SPEED_PADDING + STOP_RESOLUTION + STOP_PADDING) * 3;

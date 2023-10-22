#pragma once
#include "planner.hpp"

cube::planner_config planner_conf = {
        .machine = cube::planner_machine::corexy,
        .bounds = cube::point(5.5f, 5.5f, 3.5f),
        .step_resolution_a = 10U,
        .step_resolution_b = 10U,
        .step_resolution_c = 10U
};

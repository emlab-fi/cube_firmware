#pragma once
#include <array>
#include <stdint.h>
#include "common/coordinate.hpp"

namespace cube {


enum class planner_mode {
    cartesian,
    polar
};

enum class planner_machine {
    corexy,
    cartesian
};

enum class planner_error {
    none,
    out_of_bounds,
    misc_error
};

struct planner_config {
    planner_machine machine;
    float max_x;
    float max_y;
    float max_z;
    uint32_t step_resolution_x;
    uint32_t step_resolution_y;
    uint32_t step_resolution_z;
};

struct planner_result {
    planner_error err;
    uint32_t steps_a;
    uint32_t steps_b;
    uint32_t steps_c;
};


class planner {
    planner_config conf;
    planner_mode mode;

public:
    planner_result do_move(point target);
    void set_config(const planner_config& new_config);

};

} //namespace cube
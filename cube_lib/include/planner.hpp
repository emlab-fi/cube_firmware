#pragma once
#include <array>
#include <cstdint>
#include "common/coordinate.hpp"

namespace cube {

enum class planner_mode {
    cartesian,
    cylindrical,
    spherical
};

enum class planner_machine {
    corexy,
    cartesian
};

enum class planner_error {
    none,
    out_of_bounds,
    conversion_error,
    misc
};

struct planner_config {
    planner_machine machine;
    point bounds;
    uint32_t step_resolution_a;
    uint32_t step_resolution_b;
    uint32_t step_resolution_c;
};

struct planner_result {
    planner_error err;
    int32_t steps_a;
    int32_t steps_b;
    int32_t steps_c;
};


class planner {
    planner_config config;
    planner_mode mode;
    point current_pos;
    point zero_pos;

public:

    planner(const planner_config& conf)
        : config(conf), mode(planner_mode::cartesian),
          current_pos(0.0f, 0.0f, 0.0f), zero_pos(0.0f, 0.0f, 0.0f)
    {}

    planner_result do_move(const point& target);

    point get_absolute_pos() const;

    point get_relative_pos() const;

    void set_zero_pos() { zero_pos = current_pos; }

    void reset_zero_pos() { zero_pos = point(0.0f, 0.0f, 0.0f); }

    void reset_absolute_pos() { current_pos = point(0.0f, 0.f, 0.0f); }

    void set_config(const planner_config& new_config) { config = new_config; }

    void set_mode(const planner_mode new_mode) { mode = new_mode; }

    planner_mode get_mode() const { return mode; }

    planner_machine get_machine() const { return config.machine; }

};

} //namespace cube
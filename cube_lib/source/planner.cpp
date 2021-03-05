#include "common/coordinate.hpp"
#include "planner.hpp"

namespace {

//for now un-implemented
cube::point convert_to_polar(const cube::point& input, cube::planner_mode mode) {
    return input;
}

cube::point convert_to_cartesian(const cube::point& input, cube::planner_mode mode) {
    return input;
}

} //anonymous namespace

namespace cube {

planner_result planner::do_move(const point& pos) {

    // convert the point to cartesian vector
    vec relative = vec(pos);

    point target = zero_pos + relative;

    //check bounds
    if (target < point(0.0f, 0.0f, 0.0f) || target > config.bounds) {
        return {planner_error::out_of_bounds, 0, 0, 0};
    }

    vec distance = target - current_pos;

    planner_result output{planner_error::none, 0U, 0U, 0U};

    switch(config.machine) {
    case planner_machine::cartesian:
        output.steps_a = distance[0] * config.step_resolution_x;
        output.steps_b = distance[1] * config.step_resolution_y;
        output.steps_c = distance[2] * config.step_resolution_z;
    break;

    case planner_machine::corexy:
        output.steps_a = 0.5f * (distance[0] + distance[1]) * config.step_resolution_x;
        output.steps_b = 0.5f * (distance[0] - distance[1]) * config.step_resolution_y;
        output.steps_c = distance[2] * config.step_resolution_z;
    break;

    default:
        output.err = planner_error::misc;
    break;
    }

    return output;
}

point planner::get_absolute_pos() const {
    if (mode == planner_mode::cartesian) {
        return current_pos;
    }
    return convert_to_polar(current_pos, mode);
}

point planner::get_relative_pos() const {
    vec rel_pos = current_pos - zero_pos;
    if (mode == planner_mode::cartesian) {
        return rel_pos;
    }
    return convert_to_polar(rel_pos, mode);
}

} //namespace cube
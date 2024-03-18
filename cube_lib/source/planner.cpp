#include <optional>
#include <cmath>
#include "common/coordinate.hpp"
#include "planner.hpp"

namespace {

cube::point convert_to_polar(const cube::point& input, cube::planner_mode mode) {
    cube::point output;
    
    switch(mode) {
    case cube::planner_mode::cylindrical:
        // r = sqrt(x^2 + y^2)
        output[0] = hypotf(input[0], input[1]);
        // phi = atan2(y, x)
        output[1] = atan2f(input[1], input[0]);
        // z = z
        output[2] = input[2];
    break;

    case cube::planner_mode::spherical:
        // r = sqrt(R^2 + z^2), where R = sqrt(x^2 + y^2)
        output[0] = sqrtf(input[0] * input[0] + input[1] * input[1] + input[2] * input[2]);
        // theta = atan2(R, z)
        output[1] = atan2f(hypotf(input[0], input[1]), input[2]);
        // phi = atan2(y, x)
        output[2] = atan2f(input[1], input[0]);
    break;

    default:
        output = input;
    break;
    }

    return output;
}

std::optional<cube::point> convert_to_cartesian(const cube::point& input, cube::planner_mode mode) {

    cube::point output;

    switch(mode) {
    case cube::planner_mode::cylindrical:
        output[0] = input[0] * cosf(input[1]);
        output[1] = input[0] * sinf(input[1]);
        output[2] = input[2];
    break;

    case cube::planner_mode::spherical:
        output[0] = input[0] * sinf(input[1]) * cosf(input[2]);
        output[1] = input[0] * sinf(input[1]) * sinf(input[2]);
        output[2] = input[0] * cosf(input[1]);
    break;

    default:
        return {};
    }

    return output;
}

} //anonymous namespace

namespace cube {

planner_result planner::do_move(const point& pos) {

    // convert the point to cartesian vector
    vec relative;
    if (mode != planner_mode::cartesian) {
        auto conv_result = convert_to_cartesian(pos, mode);
        if (!conv_result) {
            return {planner_error::conversion_error, 0U, 0U, 0U};
        }
        relative = static_cast<vec>(*conv_result);

    } else {
        relative = static_cast<vec>(pos);
    }

    point target = zero_pos + relative;

    //check bounds
    if (target < point(0.0f, 0.0f, 0.0f) || target > config.bounds) {
        return {planner_error::out_of_bounds, 0, 0, 0};
    }

    vec distance = target - current_pos;

    planner_result output{planner_error::none, 0U, 0U, 0U};

    switch(config.machine) {
    case planner_machine::cartesian:
        output.steps_a = distance[0] * config.step_resolution_a;
        output.steps_b = distance[1] * config.step_resolution_b;
        output.steps_c = distance[2] * config.step_resolution_c;
    break;

    case planner_machine::corexy:
        //the 0.5f should not be here? we'll see
        output.steps_a = (distance[0] + distance[1]) * config.step_resolution_a;
        output.steps_b = (distance[0] - distance[1]) * config.step_resolution_b;
        output.steps_c = distance[2] * config.step_resolution_c;
    break;

    default:
        output.err = planner_error::misc;
    break;
    }

    // TODO what if HW fails?
    if (output.err != planner_error::misc) {
        current_pos = std::move(target);
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
        return static_cast<point>(rel_pos);
    }
    return convert_to_polar(static_cast<point>(rel_pos), mode);
}

} //namespace cube

#include "config.hpp"

namespace cube_hw {

bool is_between_adjacent(const auto& arr, const auto it) {
    if (it == arr.begin()) {
        return *it < *std::next(it);
    }
    if (it == arr.end()) {
        return *it > *std::prev(it);
    }
    return (*std::prev(it) < *it && *it < *std::next(it));
}

float motor_config::start_speed(const unsigned idx, bool is_acceleration) {
    if (idx > 0)
        return speed_points[idx - 1] * ratio;
    return (is_acceleration ? start_v : end_v) * ratio;
}

float motor_config::target_speed(const unsigned idx) {
    return speed_points[idx] * ratio;
}

float motor_config::acceleration(const unsigned idx) {
    return accelerations[idx] * ratio;
}

float motor_config::deceleration(const unsigned idx) {
    return decelerations[idx] * ratio;
}

float motor_config::reduced_target(const unsigned idx, const float reducer) {
    const float v0 = start_speed(idx, false);
    const float v1 = target_speed(idx);
    return v0 + reducer * (v1 - v0);
}

status motor_config::set_param(uint32_t param_id, uint32_t value) {
    const unsigned idx = param_id & 0xf;
    switch (param_id & 0xf0) {
        case 0x00: // base params
            switch (idx) {
                case 0x1: const_speed_t = value; break;
                case 0x2: section_duration = value; break;
                case 0x3:
                    if (value > max_velocity || value < min_velocity) {
                        return cube_hw::status::param_set_error;
                    }
                    start_v = value;
                    break;
                case 0x4:
                    if (value > max_velocity || value < min_velocity) {
                        return cube_hw::status::param_set_error;
                    }
                    end_v = value;
                    break;
                default:
                    return cube_hw::status::param_nonexistant;
            }
            break;
        case 0x10: 
            if (!is_between_adjacent(speed_points, speed_points.begin() + idx) || value > max_speed) {
                return cube_hw::status::param_set_error;
            }
            speed_points[idx] = value;
            break;
        case 0x20:
            if (!is_between_adjacent(accelerations, accelerations.begin() + idx) || value > max_acceleration) {
                return cube_hw::status::param_set_error;
            }
            speed_points[idx] = value;
            break;
        case 0x30:
            if (!is_between_adjacent(decelerations, decelerations.begin() + idx) || value > max_acceleration) {
                return cube_hw::status::param_set_error;
            }
            speed_points[idx] = value;
            break;
        default:
            return cube_hw::status::param_nonexistant;
    }
    return cube_hw::status::no_error;
}

status motor_config::get_param(uint32_t param_id, uint32_t& value) {
    const unsigned idx = param_id & 0xf;
    switch (param_id & 0xf0) {
        case 0x00: // base params
            switch (idx) {
                case 0x1: value = const_speed_t; break;
                case 0x2: value = section_duration; break;
                case 0x3: value = start_v; break;
                case 0x4: value = end_v; break;
                default:
                    return cube_hw::status::param_nonexistant;
            }
            break;
        case 0x10: value = speed_points[idx]; break;
        case 0x20: value = accelerations[idx]; break;
        case 0x30: value = decelerations[idx]; break;
        default:
            return cube_hw::status::param_nonexistant;
    }
    return cube_hw::status::no_error;
}

} // namespace cube_hw

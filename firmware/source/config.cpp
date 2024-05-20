#include "config.hpp"

namespace cube_hw {

float motor_config::const_speed_t() const {
    return static_cast<float>(_const_speed_t) / 1000.0f;
}

float motor_config::section_t() const {
    return static_cast<float>(_section_t) / 1000.0f;
}

float motor_config::homing_velocity() const {
    return static_cast<float>(_homing_velocity);
}

float motor_config::start_speed(const unsigned idx, bool is_acceleration) const {
    if (idx > 0)
        return static_cast<float>(speed_points[idx - 1]) * ratio;
    return (is_acceleration ? static_cast<float>(start_v) : static_cast<float>(end_v)) * ratio;
}

float motor_config::target_speed(const unsigned idx) const {
    return static_cast<float>(speed_points[idx]) * ratio;
}

float motor_config::acceleration(const unsigned idx) const {
    return static_cast<float>(accelerations[idx]) * ratio;
}

float motor_config::deceleration(const unsigned idx) const {
    return static_cast<float>(decelerations[idx]) * ratio;
}

float motor_config::reduced_target(const unsigned idx, const float reducer) const {
    const float v0 = start_speed(idx, false);
    const float v1 = target_speed(idx);
    return v0 + reducer * (v1 - v0);
}

int motor_config::ramp_stairs() const {
    int frames = 0;
    for (int i = 0; i < RAMPS; ++i) {
        float accel_s = (target_speed(i) - start_speed(i)) / acceleration(i);
        float decel_s = (target_speed(i) - start_speed(i)) / deceleration(i);
        frames += static_cast<int>(accel_s / section_t()) + 1;
        frames += static_cast<int>(decel_s / section_t()) + 1;
    }
    return frames;
}

uint32_t* motor_config::param(uint32_t param_id) {
    const unsigned idx = param_id & 0xf;
    switch (param_id & 0xf0) {
        case 0x00: // base params
            switch (idx) {
                case 0x1: return &_const_speed_t;
                case 0x2: return &_section_t;
                case 0x3: return &start_v;
                case 0x4: return &end_v;
            }
            break;
        case 0x10: return &speed_points[idx]; break;
        case 0x20: return &accelerations[idx]; break;
        case 0x30: return &decelerations[idx]; break;
    }
    return nullptr;
}

status motor_config::set_param(uint32_t param_id, uint32_t value) {
    auto parameter = param(param_id);
    
    if (parameter != nullptr) {
        auto old_value = *parameter;
        *parameter = value;
        if (ramp_stairs() > DMA_FRAMES_MAX - DMA_FRAMES_RESERVE) {
            // this setting would cause buffer overflow
            *parameter = old_value;
            return status::param_set_error;
        }
        return status::no_error;
    }
    return status::param_id_invalid;
}

status motor_config::get_param(uint32_t param_id, uint32_t& value) {
    auto parameter = param(param_id);

    if (parameter != nullptr) {
        value = static_cast<uint32_t>(*parameter);
        return status::no_error;
    }
    return status::param_id_invalid;
}

} // namespace cube_hw

#include <vector>
#include <algorithm>
#include "stepper_generator.hpp"

namespace cube_hw {

StepperGenerator::StepperGenerator(TIM_HandleTypeDef& htim, unsigned channel, unsigned steps_for_mm) :
    _htim(htim),
    _channel(channel),
    _steps_per_mm(steps_for_mm) {}

status StepperGenerator::start_tim_base() {
    if (_state != motor_state::RESET)
        return status::error;

    if (HAL_TIM_Base_Start(&_htim) != HAL_OK)
        cube_hw::log_error("Failed to start timer base for stepper motor");

    _state = motor_state::IDLE;
    return status::no_error;
}

const TIM_HandleTypeDef* StepperGenerator::htim() const {return &_htim;}
motor_state StepperGenerator::state() const {return _state;}

void StepperGenerator::finished_callback() {
    if (_state != motor_state::BUSY) {
        log_error("Attempt to stop motor which is not running");
        return;
    }

    HAL_TIM_PWM_Stop(&_htim, _channel);
    _state = motor_state::IDLE;
}

void StepperGenerator::limit_hit() {
    if (_state == motor_state::VELOCITY) {
        HAL_TIM_PWM_Stop(&_htim, _channel);
        _state = motor_state::IDLE;
    }
}

void StepperGenerator::set_direction(bool forward, GPIO_TypeDef* PORT, const int PIN) {
    HAL_GPIO_WritePin(PORT, PIN, forward ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int32_t StepperGenerator::acceleration_steps(const float v0, const float v1, const float a) {
    return static_cast<int32_t>((v1 * v1 - v0 * v0) / (a * 2.0f) * _steps_per_mm);;
}

int32_t StepperGenerator::constant_steps(const float v) {
    // v = s / t
    return static_cast<int32_t>(v * _const_speed_t * _steps_per_mm);
}

uint16_t StepperGenerator::get_arr(const float speed) {
    // v = s / t
    // v = speed [mm/s]
    // s = 1 step = 1 / STEP_PER_MM [mm]
    // t = arr * (1 / TIM_CLOCK) [s]
    // speed = (1 / STEP_PER_MM) / (arr * (1 / TIM_CLOCK))
    // speed = TIM_Clock / (arr * STEP_PER_MM)
    return static_cast<uint16_t>(TIM_CLOCK / (speed * _steps_per_mm));
}

float StepperGenerator::start_speed(const unsigned idx, bool is_acceleration) {
    if (idx > 0)
        return speed_points[idx - 1] * ratio;
    return (is_acceleration ? start_v : end_v) * ratio;
}

float StepperGenerator::target_speed(const unsigned idx) {
    return speed_points[idx] * ratio;
}

float StepperGenerator::acceleration(const unsigned idx) {
    return accelerations[idx] * ratio;
}

float StepperGenerator::deceleration(const unsigned idx) {
    return decelerations[idx] * ratio;
}

float StepperGenerator::get_reduced_target(const unsigned idx, const float reducer) {
    const float v0 = start_speed(idx, false);
    const float v1 = target_speed(idx);
    return v0 + reducer * (v1 - v0);
}

std::pair<int32_t, int32_t> StepperGenerator::ramp_steps(const unsigned idx, const float target) {
    int32_t steps_acc = acceleration_steps(start_speed(idx), target, acceleration(idx));
    int32_t steps_dec = acceleration_steps(start_speed(idx, false), target, deceleration(idx));
    return {steps_acc, steps_dec};
}

bool StepperGenerator::fit_ramp(const unsigned idx, int32_t& steps) {
    const float target = target_speed(idx);
    auto [steps_acc, steps_dec] = ramp_steps(idx, target);
    if (constant_steps(target) + steps_acc + steps_dec < steps) {
        steps -= steps_acc + steps_dec;
        const float target_arr = get_arr(target);
        generate_slope(steps_acc, idx, target_arr);
        generate_slope(steps_dec, idx, target_arr, false);
        return true;
    }
    return false;
}

float StepperGenerator::create_reduced_ramp(const unsigned idx, int32_t& steps) {
    for (int reducer = 0; reducer <= 10; ++reducer) {
        float reduced_target = get_reduced_target(idx, reducer / 10.0f);
        auto [steps_acc, steps_dec] = ramp_steps(idx, reduced_target);
        if (constant_steps(reduced_target) + steps_acc + steps_dec > steps) {
            const float target_arr = get_arr(reduced_target);
            if (reduced_target == end_v) {
                steps_acc = std::min(steps_acc, steps);
                steps -= steps_acc;
                generate_slope(steps_acc, idx, target_arr);
            } else {
                steps -= steps_acc + steps_dec;
                generate_slope(steps_acc, idx, target_arr);
                generate_slope(steps_dec, idx, target_arr, false);
            }
            return reduced_target;
        }
    }
    // should not reach here
    return start_speed(idx);
}

void StepperGenerator::insert_section(uint16_t arr, uint16_t rcr, bool is_acceleration) {
    if (is_acceleration) {
        acceleration_mem.emplace_back(arr -1);
        acceleration_mem.emplace_back(rcr -1);
        acceleration_mem.emplace_back(arr / 2 -1);
    } else {
        // needs to be put in reverse
        deceleration_mem.emplace_back(arr / 2 -1);        
        deceleration_mem.emplace_back(rcr -1);
        deceleration_mem.emplace_back(arr -1);
    }
}

void StepperGenerator::finalize_dma(uint16_t arr) {
    acceleration_mem.insert(acceleration_mem.end(), deceleration_mem.rbegin(), deceleration_mem.rend());
    this->ratio = 1.0f;

    acceleration_mem.emplace_back(arr);
    acceleration_mem.emplace_back(0);
    acceleration_mem.emplace_back(0);
    acceleration_mem.emplace_back(arr);
    acceleration_mem.emplace_back(0);
    acceleration_mem.emplace_back(0);
}

void StepperGenerator::generate_slope(const int32_t steps, const unsigned ramp, const uint16_t target_arr, bool is_acceleration) {
    int32_t steps_done = 0;
    const float section_duration = std::max(_section_duration, static_cast<float>(target_arr) / TIM_CLOCK);
    const float speed_increment = (is_acceleration ? acceleration(ramp) : deceleration(ramp)) * section_duration;
    float speed = std::max(start_speed(ramp, is_acceleration), 1.0f);

    while (steps_done < steps) {
        uint16_t arr = get_arr(speed);
        if (arr < target_arr)
            arr = target_arr;

        // t = arr * rcr * (1 / TIM_CLOCK)
        uint16_t rcr = static_cast<uint16_t>(section_duration * TIM_CLOCK / arr);
        if (steps_done + rcr > steps)
            rcr = static_cast<uint16_t>(steps - steps_done);

        insert_section(arr, rcr, is_acceleration);
        speed += speed_increment;
        steps_done += rcr;
    }
}

void StepperGenerator::generate_constant(int32_t steps, const float speed) {
    const uint16_t arr = get_arr(speed);

    while (steps > 0) {
        uint16_t rcr = (steps >= 0xfff0) ? 0xfff0 : steps;
        insert_section(arr, rcr);
        steps -= rcr;
    }
}

status StepperGenerator::prepare_dma(int32_t steps, float ratio) {
    if (steps == 0) {
        _state = motor_state::SKIP;
        return status::no_error;
    }

    if (_state != motor_state::IDLE) {
        return status::error;
    }

    steps = std::abs(steps);
    acceleration_mem.clear();
    deceleration_mem.clear();
    this->ratio = ratio;

    int ramp = 0;
    while (ramp < RAMPS && fit_ramp(ramp, steps)) {
        ++ramp;
    }

    const float max_speed = ramp < RAMPS ? create_reduced_ramp(ramp, steps) : target_speed(ramp - 1);    
    if (steps > 0) {
        generate_constant(steps, max_speed);
    }
    
    finalize_dma(100);
    _state = motor_state::READY;
    return status::no_error;
}

status StepperGenerator::start() {
    if (_state == motor_state::SKIP) {
        _state = motor_state::IDLE;
        return status::no_error;
    }
    if (_state != motor_state::READY) {
        cube_hw::log_error("stepper_generator: Motor is not ready\n");
        return status::error;
    }

    _state = motor_state::BUSY;

    auto retval = HAL_TIM_PWM_Start(&_htim, _channel);
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start PWM on TIM8 with rv:%d\n", retval);
        return status::error;
    }

    retval = HAL_TIM_DMABurst_MultiWriteStart(&_htim, TIM_DMABASE_ARR, TIM_DMA_UPDATE, (uint32_t*)acceleration_mem.data(), TIM_DMABURSTLENGTH_3TRANSFERS, acceleration_mem.size());
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start DMA PWM with rv:%d\n", retval);
        return status::error;
    }

    return status::no_error;
}

status StepperGenerator::do_velocity(float speed) {
    if (_state != motor_state::IDLE) {
        cube_hw::log_error("stepper_generator: Motor is not idle\n");
        return status::error;
    }

    speed = abs(speed);
    if (speed < MIN_VELOCITY || speed > MAX_VELOCITY) {
        cube_hw::log_error("stepper_generator: velocity speed out of range\n");
        return status::error;
    }

    _state = motor_state::VELOCITY;
    const uint16_t arr = get_arr(speed);
    _htim.Instance->ARR = arr - 1;
    _htim.Instance->CCR1 = arr / 2;

    auto retval = HAL_TIM_PWM_Start(&_htim, _channel);
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start PWM on TIM8 with rv:%d\n", retval);
        return status::error;
    }

    return status::no_error;
}

} //cube_hw

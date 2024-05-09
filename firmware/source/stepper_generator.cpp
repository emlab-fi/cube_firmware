#include <vector>
#include <algorithm>
#include "stepper_generator.hpp"

namespace cube_hw {

MotorPins::MotorPins(TIM_HandleTypeDef& htim, const uint16_t channel, GPIO_TypeDef* dir_gpio, const uint16_t dir_pin) :
    htim(htim),
    channel(channel),
    dir_gpio(dir_gpio),
    dir_pin(dir_pin) {}

StepperGenerator::StepperGenerator(const MotorPins& pins, unsigned steps_for_mm, motor_config config) :
    pins(pins),
    _steps_per_mm(steps_for_mm),
    config(config) {
    switch (pins.channel) {
        case TIM_CHANNEL_1: dma_burst_padding = 0; dma_burst_length = TIM_DMABurstLength_3Transfers; break;
        case TIM_CHANNEL_2: dma_burst_padding = 1; dma_burst_length = TIM_DMABurstLength_4Transfers; break;
        case TIM_CHANNEL_3: dma_burst_padding = 2; dma_burst_length = TIM_DMABurstLength_5Transfers; break;
        case TIM_CHANNEL_4: dma_burst_padding = 3; dma_burst_length = TIM_DMABurstLength_6Transfers; break;
        case TIM_CHANNEL_5: dma_burst_padding = 4; dma_burst_length = TIM_DMABurstLength_7Transfers; break;
        case TIM_CHANNEL_6: dma_burst_padding = 5; dma_burst_length = TIM_DMABurstLength_8Transfers; break;
    }
}

status StepperGenerator::start_tim_base() {
    if (_state != motor_state::RESET)
        return status::error;

    if (HAL_TIM_Base_Start(&pins.htim) != HAL_OK)
        cube_hw::log_error("Failed to start timer base for stepper motor");

    _state = motor_state::IDLE;
    return status::no_error;
}

status StepperGenerator::set_param(uint32_t index, uint32_t value) {
    return config.set_param(index, value);
}

status StepperGenerator::get_param(uint32_t index, uint32_t& value) {
    return config.get_param(index, value);
}

const TIM_HandleTypeDef* StepperGenerator::htim() const {return &pins.htim;}

motor_state StepperGenerator::state() const {return _state;}

void StepperGenerator::finished_callback() {
    if (_state != motor_state::BUSY) {
        log_error("Attempt to stop motor which is not running");
        return;
    }

    HAL_TIM_PWM_Stop(&pins.htim, pins.channel);
    _state = motor_state::IDLE;
}

void StepperGenerator::limit_hit() {
    if (_state == motor_state::VELOCITY) {
        HAL_TIM_PWM_Stop(&pins.htim, pins.channel);
        _state = motor_state::IDLE;
    }
}

void StepperGenerator::set_direction(bool forward) {
    HAL_GPIO_WritePin(pins.dir_gpio, pins.dir_pin, forward ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

int32_t StepperGenerator::acceleration_steps(const float v0, const float v1, const float a) {
    return static_cast<int32_t>((v1 * v1 - v0 * v0) / (a * 2.0f) * _steps_per_mm);
}

int32_t StepperGenerator::constant_steps(const float v) {
    // v = s / t
    // s = v * t
    return static_cast<int32_t>(v * config.const_speed_t() * _steps_per_mm);
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

std::pair<int32_t, int32_t> StepperGenerator::ramp_steps(const unsigned idx, const float target) {
    int32_t steps_acc = acceleration_steps(config.start_speed(idx, true ), target, config.acceleration(idx));
    int32_t steps_dec = acceleration_steps(config.start_speed(idx, false), target, config.deceleration(idx));
    return {steps_acc, steps_dec};
}

bool StepperGenerator::fit_ramp(const unsigned idx, int32_t& steps) {
    const float target = config.target_speed(idx);
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
    float reduced_target_prev = config.reduced_target(idx, 0);

    for (int reducer = 0; reducer <= 100; ++reducer) {
        float reduced_target = config.reduced_target(idx, reducer / 100.0f);
        auto [steps_acc, steps_dec] = ramp_steps(idx, reduced_target);

        // constant speed section long enough
        if (constant_steps(reduced_target) + steps_acc + steps_dec > steps) {
            if (steps_acc + steps_dec > steps) {
                // would overshoot, use previous values
                auto [steps_acc_prev, steps_dec_prev] = ramp_steps(idx, reduced_target_prev);
                steps_acc = steps_acc_prev;
                steps_dec = steps_dec_prev;
            }
            
            const float target_arr = get_arr(reduced_target);
            if (reduced_target == config.start_speed(idx, false)) {
                // does not have decelerating section
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
        reduced_target_prev = reduced_target;
    }
    // should not reach here
    return config.start_speed(idx, ratio);
}

void StepperGenerator::insert_section(uint16_t arr, uint16_t rcr, bool is_acceleration) {
    if (is_acceleration) {
        acceleration_mem.emplace_back(arr -1);
        acceleration_mem.emplace_back(rcr -1);
        acceleration_mem.insert(acceleration_mem.end(), dma_burst_padding, 0);
        acceleration_mem.emplace_back(arr / 2 -1);
    } else {
        // needs to be put in reverse
        deceleration_mem.emplace_back(arr / 2 -1);
        deceleration_mem.insert(deceleration_mem.end(), dma_burst_padding, 0);        
        deceleration_mem.emplace_back(rcr -1);
        deceleration_mem.emplace_back(arr -1);
    }
}

void StepperGenerator::finalize_dma(uint16_t arr) {
    acceleration_mem.insert(acceleration_mem.end(), deceleration_mem.rbegin(), deceleration_mem.rend());

    //padding
    acceleration_mem.emplace_back(arr);
    acceleration_mem.insert(acceleration_mem.end(), dma_burst_padding + 2, 0);
    acceleration_mem.emplace_back(arr);
    acceleration_mem.insert(acceleration_mem.end(), dma_burst_padding + 2, 0);
}

void StepperGenerator::generate_slope(const int32_t steps, const unsigned ramp, const uint16_t target_arr, bool is_acceleration) {
    int32_t steps_done = 0;
    const float section_duration = std::max(config.section_t(), static_cast<float>(target_arr) / TIM_CLOCK);
    const float speed_increment = (is_acceleration ? config.acceleration(ramp) : config.deceleration(ramp)) * section_duration;
    float speed = std::max(config.start_speed(ramp, is_acceleration), 1.0f);

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

    set_direction(steps > 0);
    steps = std::abs(steps);
    acceleration_mem.clear();
    deceleration_mem.clear();
    config.ratio = ratio;

    int ramp = 0;
    while (ramp < RAMPS && fit_ramp(ramp, steps)) {
        ++ramp;
    }

    const float max_speed = ramp < RAMPS ? create_reduced_ramp(ramp, steps) : config.target_speed(ramp - 1);
    if (steps > 0) {
        generate_constant(steps, max_speed);
    }
    
    finalize_dma(100);
    config.ratio = 1.0f;
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

    auto retval = HAL_TIM_PWM_Start(&pins.htim, pins.channel);
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start PWM on TIM8 with rv:%d\n", retval);
        return status::error;
    }

    retval = HAL_TIM_DMABurst_MultiWriteStart(&pins.htim, TIM_DMABASE_ARR, TIM_DMA_UPDATE, (uint32_t*)acceleration_mem.data(), dma_burst_length, acceleration_mem.size());
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

    set_direction(speed > 0);
    speed = abs(speed);
    if (speed < config.min_velocity || speed > config.max_velocity) {
        cube_hw::log_error("stepper_generator: velocity speed out of range\n");
        return status::error;
    }

    _state = motor_state::VELOCITY;
    const uint16_t arr = get_arr(speed);
    __HAL_TIM_SET_AUTORELOAD(&pins.htim, arr);
    __HAL_TIM_SET_COMPARE(&pins.htim, pins.channel, arr / 2);

    auto retval = HAL_TIM_PWM_Start(&pins.htim, pins.channel);
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start PWM on TIM8 with rv:%d\n", retval);
        return status::error;
    }

    return status::no_error;
}

} //cube_hw

#include "stepper_generator.hpp"

namespace cube_hw {

StepperGenerator::StepperGenerator(TIM_HandleTypeDef& htim, unsigned channel) :
    _htim(htim),
    _channel(channel) {}

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
float StepperGenerator::dma_ms() const {return _dma_ms;}

void StepperGenerator::finished_callback() {
    // TODO
    if (_state != motor_state::BUSY) {
        log_error("Attempt to stop which is not running");
        return;
    }

    HAL_TIM_PWM_Stop(&_htim, _channel);
    _dma_idx = 0;
    _dma_ms = 0;
    _state = motor_state::IDLE;
}

double ramping_value(float x) {
    return 1.0f / (1 + std::exp(-6.0f * (x - 0.5f)));
}

void StepperGenerator::flip_direction() {
    if (_state == motor_state::IDLE) {
        _flip_direction = !_flip_direction;
    }
}

void StepperGenerator::set_direction(bool forward, GPIO_TypeDef* PORT, const int PIN) {
    auto direction = forward ? GPIO_PIN_SET : GPIO_PIN_RESET;
    if (_flip_direction) {
        direction = GPIO_PIN_SET ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }
    HAL_GPIO_WritePin(PORT, PIN, direction);
}

void StepperGenerator::insert_dma(int auto_reload, int repetition_c) {
    _dma_ms += 0.0000625 * static_cast<float>((auto_reload + 1) * repetition_c);  
    _dma_buffer[_dma_idx++] = auto_reload;
    _dma_buffer[_dma_idx++] = repetition_c - 1;
    _dma_buffer[_dma_idx++] = auto_reload / 2;
}

void StepperGenerator::insert_dma_padding(int auto_reload) {
    _dma_buffer[_dma_idx++] = auto_reload;
    _dma_buffer[_dma_idx++] = 0;
    _dma_buffer[_dma_idx++] = 0;
    _dma_buffer[_dma_idx++] = auto_reload;
    _dma_buffer[_dma_idx++] = 0;
    _dma_buffer[_dma_idx++] = 0;
}

status StepperGenerator::prepare_dma(int32_t steps) {
    steps = std::abs(steps);
    if (steps == 0) {
        _state = motor_state::SKIP;
        return status::no_error;
    }

    if (_state != motor_state::IDLE) {
        return status::error;
    }

    int32_t start_steps = static_cast<int32_t>((MAX_SPEED * MAX_SPEED) / (ACCELERATION * 2.0) * planner_conf.step_resolution_a);
    int32_t stop_steps = static_cast<int32_t>((MAX_SPEED * MAX_SPEED) / (DECELERATION * 2.0) * planner_conf.step_resolution_a); 
    int32_t const_steps = std::max(0l, steps - start_steps - stop_steps);
    const int32_t start_limit = static_cast<int32_t>((steps) * DECELERATION / (ACCELERATION + DECELERATION));
    
    const int max_speed = start_limit < start_steps ? (MAX_SPEED * start_limit) / start_steps : MAX_SPEED;
    start_steps = std::min(start_steps, start_limit);
    stop_steps = std::min(stop_steps, steps - start_limit);
    int start_steps_rcr = start_steps / START_RESOLUTION;
    int stop_steps_rcr = stop_steps / STOP_RESOLUTION;
    const int const_arr = TIM_CLOCK / (planner_conf.step_resolution_a * MAX_SPEED);

    float one_p = 1.0 / START_RESOLUTION;
    float half_p = one_p / 2.0;
    for (int i = 0; i < START_RESOLUTION; ++i) {
        int speed = static_cast<int>(max_speed * ramping_value(i * one_p + half_p));
        if (i >= START_RESOLUTION - 1)
            start_steps_rcr += start_steps % START_RESOLUTION;
        insert_dma(TIM_CLOCK / (planner_conf.step_resolution_a * std::max(speed, 2)), start_steps_rcr);
    }
    while (const_steps > 0) {
        int cnt = const_steps >= 65000l ? 65000 : const_steps;
        insert_dma(const_arr, cnt);
        const_steps -= cnt;
    }
    one_p = 1.0 / STOP_RESOLUTION;
    half_p = one_p / 2.0;
    for (int i = 0; i < STOP_RESOLUTION; ++i) {
        int speed = max_speed - static_cast<int>(max_speed * ramping_value(i * one_p + half_p));
        if (i >= STOP_RESOLUTION - 1)
            stop_steps_rcr += stop_steps % STOP_RESOLUTION;
        insert_dma(TIM_CLOCK / (planner_conf.step_resolution_a * std::max(speed, 2)), stop_steps_rcr);
    }

    insert_dma_padding(99);
    _state = motor_state::READY;
    return status::no_error;
}

void StepperGenerator::adjust_timings(float max_time, int32_t steps) {
    if (_state != motor_state::READY || (_dma_ms + 1.0f > max_time && max_time < _dma_ms + 1.0f)) {
        return;
    }

    steps = std::abs(steps);
    const float ms_p_step = (max_time - _dma_ms) / static_cast<float>(steps);
    float arr_offset_f = ms_p_step / 0.0000625;
    int arr_offset = static_cast<int>(arr_offset_f);

    for (unsigned i = 0; i < _dma_idx; i += 3) {
        _dma_buffer[i] += arr_offset;
        _dma_buffer[i + 2] = _dma_buffer[i] / 2;
    }
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

    retval = HAL_TIM_DMABurst_MultiWriteStart(&_htim, TIM_DMABASE_ARR, TIM_DMA_UPDATE, (uint32_t*)_dma_buffer.data(), TIM_DMABURSTLENGTH_3TRANSFERS, _dma_idx);
    if (HAL_OK != retval) {
        cube_hw::log_info("stepper_generator: Failed to start DMA PWM with rv:%d\n", retval);
        return status::error;
    }

    return status::no_error;
}

} //cube_hw

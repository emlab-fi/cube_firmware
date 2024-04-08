#include "peripherals.hpp"
#include "hardware.hpp"

namespace cube_hw {

status set_motor_power(bool enabled) {
    // if (enabled) {
    //     log_info("cube_hw: motors on\n");
    //     HAL_GPIO_WritePin(ENABLE_OUT1_GPIO_Port, ENABLE_OUT1_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(ENABLE_OUT2_GPIO_Port, ENABLE_OUT2_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(ENABLE_OUT3_GPIO_Port, ENABLE_OUT3_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(ENABLE_OUT4_GPIO_Port, ENABLE_OUT4_Pin, GPIO_PIN_RESET);
    // } else {
    //     return status::no_error;    
    //     //log_info("cube_hw: motors off\n");
    //     HAL_GPIO_WritePin(ENABLE_OUT1_GPIO_Port, ENABLE_OUT1_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(ENABLE_OUT2_GPIO_Port, ENABLE_OUT2_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(ENABLE_OUT3_GPIO_Port, ENABLE_OUT3_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(ENABLE_OUT4_GPIO_Port, ENABLE_OUT4_Pin, GPIO_PIN_SET);
    // }
    return status::no_error;
}

// not used anymore
uint8_t limits_status() {
    log_info("cube_hw: reading limit switches\n");
    uint8_t pin_state = 0U;

    //x axis
    if (HAL_GPIO_ReadPin(IO1_GPIO_Port, IO1_Pin)) {
        pin_state |= A_LIMIT_START;
    }
    if (HAL_GPIO_ReadPin(IO2_GPIO_Port, IO2_Pin)) {
        pin_state |= A_LIMIT_STOP;
    }

    //y axis
    if (HAL_GPIO_ReadPin(IO3_GPIO_Port, IO3_Pin)) {
        pin_state |= B_LIMIT_START;
    }
    if (HAL_GPIO_ReadPin(IO4_GPIO_Port, IO4_Pin)) {
        pin_state |= B_LIMIT_STOP;
    }

    //z axis
    if (HAL_GPIO_ReadPin(IO5_GPIO_Port, IO5_Pin)) {
        pin_state |= C_LIMIT_START;
    }
    if (HAL_GPIO_ReadPin(IO6_GPIO_Port, IO6_Pin)) {
        pin_state |= C_LIMIT_STOP;
    }

    return pin_state;
}


status do_steps(int32_t a, int32_t b, int32_t c) {
    stepper_generator_z.prepare_dma(c);
    stepper_generator_z.start();

    core_xy.move(a, b);

    // busy wait
    while(!core_xy.is_idle() && stepper_generator_z.state() != motor_state::IDLE);
    return status::no_error;
}

status home() {
    status core_result, vertical_result;

    if (HAL_GPIO_ReadPin(LIMIT_GPIO_BASE, LIMIT_Z_START) == GPIO_PinState::GPIO_PIN_SET) {
        vertical_result = stepper_generator_z.do_velocity(-VerticalConfig.homing_velocity);
    }

    core_result = core_xy.home();
    if (vertical_result != status::no_error || core_result != status::no_error) {
        return status::error;
    }

    // busy wait
    while(!core_xy.is_idle() || stepper_generator_z.state() != motor_state::IDLE);
    return status::no_error;
}

status do_velocity(int32_t a, int32_t b, int32_t c) { return status::error; }

status reset_pos() {
    // nothing to do 
    return status::no_error;
}

} //namespace cube_hw

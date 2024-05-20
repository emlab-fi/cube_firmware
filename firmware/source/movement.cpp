#include "peripherals.hpp"
#include "hardware.hpp"

namespace cube_hw {

status set_motor_power(bool enabled) {
    tmc_driver_x.set_freewheel(!enabled);
    tmc_driver_y.set_freewheel(!enabled);
    tmc_driver_z1.set_freewheel(!enabled);
    tmc_driver_z2.set_freewheel(!enabled);

    enabled ? log_info("cube_hw: motors enabled\n") : log_info("cube_hw: motors freewheel\n");
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

status set_param(uint32_t index, uint32_t value) {
    switch(index & 0xff000000) {
        case cube::params::motors:
            switch(index & 0xff0000) {
                case cube::params::motor_cat::motor_x: return stepper_generator_x.set_param(index, value);
                case cube::params::motor_cat::motor_y: return stepper_generator_y.set_param(index, value);
                case cube::params::motor_cat::motor_z: return stepper_generator_z.set_param(index, value);
                case cube::params::motor_cat::core_xy: return core_xy.set_param(index, value);  
            }
            break;
    }
    return status::param_id_invalid;
}

status get_param(uint32_t index, uint32_t& value) {
    switch(index & 0xff000000) {
        case cube::params::motors:
            switch(index & 0xff0000) {
                case cube::params::motor_cat::core_xy:
                case cube::params::motor_cat::motor_x: return stepper_generator_x.get_param(index, value);
                case cube::params::motor_cat::motor_y: return stepper_generator_y.get_param(index, value);
                case cube::params::motor_cat::motor_z: return stepper_generator_z.get_param(index, value);
            }
            break;
    }
    return status::param_id_invalid;
}


status do_steps(int32_t a, int32_t b, int32_t c) {
    if (stepper_generator_z.prepare_dma(c) != status::no_error ||
        stepper_generator_z.start() != status::no_error ||
        core_xy.move(a, b) != status::no_error) {    
        return status::error;
    }

    // busy wait
    while(!core_xy.is_idle() || stepper_generator_z.state() != motor_state::IDLE);
    return status::no_error;
}

status home() {
    status core_result = status::no_error;
    status vertical_result = status::no_error;

    if (HAL_GPIO_ReadPin(LIMIT_GPIO_BASE, LIMIT_Z_START) == GPIO_PinState::GPIO_PIN_SET) {
        vertical_result = stepper_generator_z.do_velocity(-VerticalConfig.homing_velocity());
    }

    core_result = core_xy.home();
    if (vertical_result != status::no_error || core_result != status::no_error) {
        return status::error;
    }

    // busy wait
    while(!core_xy.is_idle() || stepper_generator_z.state() != motor_state::IDLE);
    return status::no_error;
}

status do_velocity(int32_t , int32_t , int32_t ) { return status::error; }

status reset_pos() {
    // nothing to do 
    return status::no_error;
}

} //namespace cube_hw

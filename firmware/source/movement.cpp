#include "hardware.hpp"
#include "main.hpp"

namespace cube_hw {

status set_motor_power(bool enabled) {
    if (enabled) {
        HAL_GPIO_WritePin(ENABLE_OUT1_GPIO_Port, ENABLE_OUT1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ENABLE_OUT2_GPIO_Port, ENABLE_OUT2_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ENABLE_OUT3_GPIO_Port, ENABLE_OUT3_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(ENABLE_OUT4_GPIO_Port, ENABLE_OUT4_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(ENABLE_OUT1_GPIO_Port, ENABLE_OUT1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ENABLE_OUT2_GPIO_Port, ENABLE_OUT2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ENABLE_OUT3_GPIO_Port, ENABLE_OUT3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ENABLE_OUT4_GPIO_Port, ENABLE_OUT4_Pin, GPIO_PIN_SET);
    }
    return status::no_error;
}

uint8_t limits_status() { return 0U; }

status do_steps(int32_t a, int32_t b, int32_t c) { return status::error; }

status do_velocity(int32_t a, int32_t b, int32_t c) { return status::error; }

} //namespace cube_hw
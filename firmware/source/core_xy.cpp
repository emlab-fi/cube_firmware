#include <numbers>
#include "peripherals.hpp"
#include "main.hpp"
#include "config.hpp"
#include "core_xy.hpp"


namespace cube_hw {

constexpr float PI = std::numbers::pi_v<float>;
constexpr float PI_HALF = PI / 2.0f;

CoreXY::CoreXY(StepperGenerator& motor_x, StepperGenerator& motor_y) : 
    motor_x(motor_x),
    motor_y(motor_y) {}

void CoreXY::move(int32_t x, int32_t y) {
    motor_x.set_direction(x > 0, DIR1_OUT_GPIO_Port, DIR1_OUT_Pin);
    motor_y.set_direction(y > 0, DIR2_OUT_GPIO_Port, DIR2_OUT_Pin);
    
    x = std::abs(x);
    y = std::abs(y);
    float ratio = 1.0f;
    if (x != 0 || y != 0)
        ratio = x > y ? static_cast<float>(y) / x : static_cast<float>(x) / y;

    motor_x.prepare_dma(x, x > y ? 1.0 : ratio);
    motor_y.prepare_dma(y, y > x ? 1.0 : ratio);

    motor_x.start();
    motor_y.start();
}

bool CoreXY::is_idle() const {
    return motor_x.state() == motor_state::IDLE &&  motor_y.state() == motor_state::IDLE;
}

void CoreXY::home() {
    // Home X
    angle = PI; // 180
    motor_x.set_direction(false, DIR1_OUT_GPIO_Port, DIR1_OUT_Pin);
    motor_y.set_direction(false, DIR2_OUT_GPIO_Port, DIR2_OUT_Pin);
    motor_x.do_velocity(HOMING_SPEED);
    motor_y.do_velocity(HOMING_SPEED);
    while(!is_idle());

    // Home Y
    angle = PI + PI_HALF; // 270
    motor_x.set_direction(false, DIR1_OUT_GPIO_Port, DIR1_OUT_Pin);
    motor_y.set_direction(true, DIR2_OUT_GPIO_Port, DIR2_OUT_Pin);
    motor_x.do_velocity(HOMING_SPEED);
    motor_y.do_velocity(HOMING_SPEED);
}

void CoreXY::limit_hit(const uint16_t limit) {
    if (motor_x.state() == motor_state::VELOCITY && motor_y.state() == motor_state::VELOCITY && (
        (limit == LIMIT_X_START && angle > PI_HALF && angle < (PI + PI_HALF)) ||
        (limit == LIMIT_X_STOP && (angle < PI_HALF || angle > (PI + PI_HALF))) ||
        (limit == LIMIT_Y_START && angle > PI) ||
        (limit == LIMIT_Y_STOP && angle < PI && angle != 0))) {
            motor_x.limit_hit();
            motor_y.limit_hit();
    }
}

} // namespace cube_hw

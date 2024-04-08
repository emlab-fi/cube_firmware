#include <numbers>
#include "peripherals.hpp"
#include "main.hpp"
#include "config.hpp"
#include "core_xy.hpp"


namespace cube_hw {

constexpr float PI = std::numbers::pi_v<float>;
constexpr float PI_HALF = PI / 2.0f;

bool no_error(status x, status y) {
    return x == status::no_error && y == status::no_error;
}

CoreXY::CoreXY(StepperGenerator& motor_x, StepperGenerator& motor_y) : 
    motor_x(motor_x),
    motor_y(motor_y) {}

status CoreXY::set_param(uint32_t index, uint32_t value) {
    const auto result = motor_x.set_param(index, value);
    if (result != status::no_error) {
        return result;
    }
    return motor_y.set_param(index, value);
}

status CoreXY::move(int32_t x, int32_t y) {
    const auto x_abs = std::abs(x);
    const auto y_abs = std::abs(y);
    float ratio = 1.0f;
    if (x != 0 || y != 0)
        ratio = x_abs > y_abs ? static_cast<float>(y_abs) / x_abs : static_cast<float>(x_abs) / y_abs;

    auto result_x = motor_x.prepare_dma(x, x_abs > y_abs ? 1.0 : ratio);
    auto result_y = motor_y.prepare_dma(y, y_abs > x_abs ? 1.0 : ratio);
    if (!no_error(result_x, result_y)) {
        return status::no_error;
    }

    result_x = motor_x.start();
    result_y = motor_y.start();
    return no_error(result_x, result_y) ? status::no_error : status::error;
}

bool CoreXY::is_idle() const {
    return motor_x.state() == motor_state::IDLE && motor_y.state() == motor_state::IDLE;
}

status CoreXY::home() {
    status result_x, result_y;

    if (HAL_GPIO_ReadPin(LIMIT_GPIO_BASE, LIMIT_X_START) == GPIO_PinState::GPIO_PIN_SET) {
        // Home X
        angle = PI; // 180
        result_x = motor_x.do_velocity(-CoreConfig.homing_velocity);
        result_y = motor_y.do_velocity(-CoreConfig.homing_velocity);

        if (!no_error(result_x, result_y)) {
            return status::error;
        }

        // wait to X homing
        while(!is_idle());
    }
    
    if (HAL_GPIO_ReadPin(LIMIT_GPIO_BASE, LIMIT_Y_START) == GPIO_PinState::GPIO_PIN_SET) {
        // Home Y
        angle = PI + PI_HALF; // 270
        result_x = motor_x.do_velocity(-CoreConfig.homing_velocity);
        result_y = motor_y.do_velocity(CoreConfig.homing_velocity);
    }

    return no_error(result_x, result_y) ? status::no_error : status::error;
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

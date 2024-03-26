#pragma once
#include "stepper_generator.hpp"

namespace cube_hw {

class CoreXY {
    StepperGenerator& motor_x;
    StepperGenerator& motor_y;
    float angle = 0;

public:
    CoreXY(StepperGenerator& motor_x, StepperGenerator& motor_y);
    void move(const int32_t x, const int32_t y);
    bool is_idle() const;
    void home();
    void limit_hit(const uint16_t limit);
};

} // namespace cube_hw

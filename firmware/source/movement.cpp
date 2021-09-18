#include "hardware.hpp"

namespace cube_hw {

status set_motor_power(bool enabled) { return status::error; }

status do_steps(int32_t a, int32_t b, int32_t c) { return status::error; }

status do_velocity(int32_t a, int32_t b, int32_t c) { return status::error; }

uint8_t limits_status() { return 0U; }

} //namespace cube_hw
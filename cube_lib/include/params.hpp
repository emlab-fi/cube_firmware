#pragma once
#include <cstdint>

namespace cube {

namespace params {

constexpr uint32_t motors = 0x01'00'0000;

namespace motor_cat {

constexpr uint32_t motor_x = 0x00'01'0000;
constexpr uint32_t motor_y = 0x00'02'0000;
constexpr uint32_t motor_z = 0x00'03'0000;
// reserved
// composed
constexpr uint32_t core_xy = 0x00'10'0000;

} //namespace cat
} //namespace error
} //namespace cube
#pragma once
#include <cstdint>

namespace cube {

namespace error {

constexpr uint32_t cube = 0xaa'00'0000;

namespace cat {

constexpr uint32_t message = 0x00'01'0000;
constexpr uint32_t decode = 0x00'02'0000;
constexpr uint32_t planner = 0x00'03'0000;
constexpr uint32_t hw_transfers = 0x00'04'0000;
constexpr uint32_t hw_movement = 0x00'05'0000;
constexpr uint32_t hw_gpio = 0x00'06'0000;
constexpr uint32_t hw_misc = 0x00'07'0000;
constexpr uint32_t misc = 0x00'08'0000;

} //namespace cat
} //namespace error


constexpr uint32_t error_code(uint32_t nmpspace, uint32_t category, uint32_t id) {
    return nmpspace + category + id;
}

} //namespace cube
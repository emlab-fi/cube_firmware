#include "errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"

namespace {

struct scoped_motor_power {

    scoped_motor_power() {
        cube_hw::set_motor_power(1);
    }

    ~scoped_motor_power() {
        cube_hw::set_motor_power(0);
    }
};

} //anonymous namespace

namespace cube {

void controller::instr_move(uint32_t id, point* target) {
    return;
}


void controller::instr_home(uint32_t id) {
    return;
}

} //namespace cube
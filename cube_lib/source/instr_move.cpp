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
    //turn on motors
    scoped_motor_power pwr{};

    cube_hw::log_info("cube_lib::controller: starting move instruction\n");

    planner_result res = motion_planner.do_move(*target);

    switch(res.err) {

    case planner_error::conversion_error:
        send_simple_reply(id, error_code(error::cube, error::cat::planner, 2));
        return;

    case planner_error::out_of_bounds:
        send_simple_reply(id, error_code(error::cube, error::cat::planner, 1));
        return;

    case planner_error::misc:
        send_simple_reply(id, error_code(error::cube, error::cat::planner, 3));
        return;

    default:
        cube_hw::log_info("cube_lib::controller: acquired steps calculation\n");
        break;
    };

    auto status = cube_hw::do_steps(res.steps_a, res.steps_b, res.steps_c);

    if (status != cube_hw::status::no_error) {
        cube_hw::log_info("cube_lib::controller: HW failed during step mode\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_movement, 2));
        return;
    }

    cube_hw::log_info("cube_lib::controller: finished steps\n");
    send_simple_reply(id, 0);

    return;
}


void controller::instr_home(uint32_t id) {

    cube_hw::log_warning("cube_lib::controller: basic home implementation!");

    motion_planner.reset_absolute_pos();
    motion_planner.reset_zero_pos();

    send_simple_reply(id, 0);

    return;
}

} //namespace cube
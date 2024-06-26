#include "errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"

namespace {

struct scoped_motor_power {
    scoped_motor_power() {
        cube_hw::set_motor_power(true);
    }

    ~scoped_motor_power() {
        cube_hw::set_motor_power(false);
    }
};

uint32_t wait_on_limit(uint8_t flag) {
    uint32_t counter = 0;
    while ((cube_hw::limits_status() & flag) == 0) {
        // increment counter so that we do something
        ++counter;
    }
    return counter;
}

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
        cube_hw::log_error("cube_lib::controller: HW failed during step mode\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_movement, 2));
        return;
    }

    cube_hw::log_info("cube_lib::controller: finished steps\n");
    send_simple_reply(id, 0);

    return;
}


void controller::instr_home(uint32_t id) {
    cube_hw::log_info("cube_lib::controller: homing procedure started\n");

    //turn on motors
    scoped_motor_power pwr{};

    cube_hw::log_info("cube_lib::controller: homing Y\n");
    auto status = cube_hw::home();

    if (status != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: HW fail in velocity mode\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_movement, 1));
        return;
    }

    cube_hw::log_info("cube_lib::controller: resetting internal positions\n");    
    motion_planner.reset_absolute_pos();
    motion_planner.reset_zero_pos();
    status = cube_hw::reset_pos();

    if (status != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: HW reset pos fail\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_movement, 4));
        return;
    }

    cube_hw::log_info("cube_lib::controller: homing OK\n");
    send_simple_reply(id, 0);
}

} //namespace cube

#include "errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"
#include "params.hpp"

namespace cube {

void controller::instr_i2c_transfer(uint32_t id, i2c_transfer_payload* data) {

    cube_hw::log_info("cube_lib::controller: starting i2c transfer\n");

    auto [err, result] = cube_hw::i2c_transfer(data);

    if (err != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: i2c transfer failed\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 10));
        return;
    }

    reply_message msg = {
        .id = id,
        .status = {
            .error_id = 0,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_relative_pos()
        },
        .payload = result
    };

    cube_hw::send_message(msg.encode());
    cube_hw::log_info("cube_lib::controller: i2c transfer finished, sent reply\n");
}


void controller::instr_spi_transfer(uint32_t id, spi_transfer_payload* data) {

    cube_hw::log_info("cube_lib::controller: starting spi transfer\n");

    auto [err, result] = cube_hw::spi_transfer(data);

    if (err != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: spi transfer failed\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 11));
        return;
    }

    reply_message msg = {
        .id = id,
        .status = {
            .error_id = 0,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_relative_pos()
        },
        .payload = result
    };

    cube_hw::send_message(msg.encode());
    cube_hw::log_info("cube_lib::controller: spi transfer finished, sent reply\n");
}


void controller::instr_set_gpio_mode(uint32_t id, gpio_config_payload* data) {

    cube_hw::log_info("cube_lib::controller: setting GPIO mode\n");

    auto err = cube_hw::set_gpio_mode(data);

    if (err != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: GPIO set mode error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 1));
        return;
    }

    send_simple_reply(id, 0);
}


void controller::instr_set_gpio(uint32_t id, gpio_config_payload* data) {

    cube_hw::log_info("cube_lib::controller: setting GPIO output\n");

    auto err = cube_hw::set_gpio(data);

    switch(err) {
    case cube_hw::status::gpio_wrong_mode:
        cube_hw::log_error("cube_lib::controller: GPIO wrong mode\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 2));
        break;

    case cube_hw::status::gpio_set_error:
        cube_hw::log_error("cube_lib::controller: GPIO set error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 3));
        break;

    case cube_hw::status::no_error:
        send_simple_reply(id, 0);
        break;

    default:
        cube_hw::log_error("cube_lib::controller: GPIO set misc error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 5));
        break;
    }
}


void controller::instr_get_gpio(uint32_t id, gpio_config_payload* data) {

    cube_hw::log_info("cube_lib::controller: reading GPIO state\n");

    auto [err, result] = cube_hw::read_gpio(data);

    if (err != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: GPIO read error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::hw_io, 4));
        return;
    }

    reply_message msg = {
        .id = id,
        .status = {
            .error_id = 0,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_relative_pos()
        },
        .payload = result
    };

    cube_hw::send_message(msg.encode());
    cube_hw::log_info("cube_lib::controller: GPIO read done, sent reply\n");
}

void controller::instr_set_param(uint32_t id, param_config_payload param) {
    switch(cube_hw::set_param(param.id, param.value)) {
    case cube_hw::status::param_id_invalid:
        cube_hw::log_error("cube_lib::controller: Unknown param id \n");
        send_simple_reply(id, error_code(error::cube, error::cat::param, 1));
        break;

    case cube_hw::status::param_set_error:
        cube_hw::log_error("cube_lib::controller: Invalid param value \n");
        send_simple_reply(id, error_code(error::cube, error::cat::param, 2));
        break;

    case cube_hw::status::no_error:
        send_simple_reply(id, 0);
        break;

    default:
        cube_hw::log_error("cube_lib::controller: Param set misc error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::param, 3));
        break;
    }
}

void controller::instr_get_param(uint32_t id, param_config_payload param) {
    const auto result = cube_hw::get_param(param.id, param.value);

    if (result == cube_hw::status::param_id_invalid) {
        cube_hw::log_error("cube_lib::controller: Unknown param id \n");
        send_simple_reply(id, error_code(error::cube, error::cat::param, 1));
        return;
    }
    else if (result != cube_hw::status::no_error) {
        cube_hw::log_error("cube_lib::controller: Param read misc error\n");
        send_simple_reply(id, error_code(error::cube, error::cat::param, 4));
        return;
    }

    reply_message msg = {
        .id = id,
        .status = {
            .error_id = 0,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_relative_pos()
        },
        .payload = param.value
    };

    cube_hw::send_message(msg.encode());
    cube_hw::log_info("cube_lib::controller: Param read done, sent reply\n");
}

} //namespace cube
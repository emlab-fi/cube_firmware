#include <tuple>
#include "errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

void controller::send_simple_reply(uint32_t id, uint32_t error) {
    reply_message msg = {
        .id = id,
        .status = {
            .error_id = error,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_relative_pos()
        },
        .payload = std::monostate{}
    };
    cube_hw::log_info("cube_lib::controller: Sent reply id: %u error: %u\n", id, error);
    cube_hw::send_message(msg.encode());
}

void controller::send_absolute_pos(uint32_t id) {
    reply_message msg = {
        .id = id,
        .status = {
            .error_id = 0,
            .mode = motion_planner.get_mode(),
            .position = motion_planner.get_absolute_pos()
        },
        .payload = std::monostate{}
    };
    cube_hw::send_message(msg.encode());
}

void controller::process_command(encoded_message& input) {

    if (input.type == message_type::empty) {
        //do nothing
        return;
    }

    if (input.type != message_type::command) {
        cube_hw::log_error("cube_lib::controller: Message is not a command.\n");
        send_simple_reply(0, error_code(error::cube, error::cat::message, 1));
        return;
    }

    auto [dec_err, command] = decode_cmd_message(input);
    cube_hw::log_info("cube_lib::controller: Decoded new command.\n");

    if (dec_err == decode_error::pb_error) {
        cube_hw::log_error("cube_lib::controller: Decoding error.\n");
        send_simple_reply(command.id, error_code(error::cube, error::cat::decode, 1));
        return;
    }

    if (dec_err == decode_error::wrong_payload) {
        cube_hw::log_error("cube_lib::controller: Wrong message payload.\n");
        send_simple_reply(command.id, error_code(error::cube, error::cat::decode, 2));
        return;
    }

    switch (command.instr) {
    case instructions::nop:
        cube_hw::log_warning("cube_lib::controller: Received nop instruction.\n");
        send_simple_reply(0, 0);
        break;

    case instructions::status:
        cube_hw::log_info("cube_lib::controller: Sending status.\n");
        send_simple_reply(command.id, 0);
        break;

    case instructions::move_to:
        instr_move(command.id, std::get_if<point>(&command.payload));
        break;

    case instructions::spi_transfer:
        instr_spi_transfer(command.id, std::get_if<spi_transfer_payload>(&command.payload));
        break;

    case instructions::i2c_transfer:
        instr_i2c_transfer(command.id, std::get_if<i2c_transfer_payload>(&command.payload));
        break;

    case instructions::set_gpio_mode:
        instr_set_gpio_mode(command.id, std::get_if<gpio_config_payload>(&command.payload));
        break;

    case instructions::set_gpio:
        instr_set_gpio(command.id, std::get_if<gpio_config_payload>(&command.payload));
        break;

    case instructions::get_gpio:
        instr_get_gpio(command.id, std::get_if<gpio_config_payload>(&command.payload));
        break;

    case instructions::set_zero_pos:
        motion_planner.set_zero_pos();
        cube_hw::log_info("cube_lib::planner: Set zero position.\n");
        send_simple_reply(command.id, 0);
        break;

    case instructions::reset_zero_pos:
        motion_planner.reset_zero_pos();
        cube_hw::log_info("cube_lib::planner: Reset zero position.\n");
        send_simple_reply(command.id, 0);
        break;

    case instructions::set_coordinate_mode:
        motion_planner.set_mode(*std::get_if<planner_mode>(&command.payload));
        cube_hw::log_info("cube_lib::planner: Set new mode.\n");
        send_simple_reply(command.id, 0);
        break;

    case instructions::get_abs_pos:
        send_absolute_pos(command.id);
        break;

    case instructions::get_rel_pos:
        send_simple_reply(command.id, 0);
        break;

    case instructions::home:
    case instructions::set_parameter:
    case instructions::get_parameter:
        cube_hw::log_warning("cube_lib::controller: Instruction not implemented.\n");
        send_simple_reply(command.id, error_code(error::cube, error::cat::misc, 2));
        return;

    default:
        cube_hw::log_error("cube_lib::controller: Fatal error in instr decode.\n");
        send_simple_reply(0, error_code(error::cube, error::cat::misc, 1));
        return;
    }

}

}
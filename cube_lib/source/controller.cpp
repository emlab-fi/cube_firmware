#include <tuple>
#include "common/errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

void controller::process_command(encoded_message& input) {

    if (input.type == message_type::empty) {
        //do nothing
        return;
    }

    if (input.type != message_type::command) {
        send_status(0, error_code(error::cube, error::cat::message, 1));
        return;
    }

    auto [dec_err, command] = decode_cmd_message(input);
    cube_hw::log_info("Decoded new command\n");

    if (dec_err == decode_error::pb_error) {
        send_status(command.id, error_code(error::cube, error::cat::decode, 1));
        return;
    }

    if (dec_err == decode_error::wrong_payload) {
        send_status(command.id, error_code(error::cube, error::cat::decode, 2));
        return;
    }

    switch (command.instr) {
    case instructions::nop:
        send_status(0, 0);
    break;

    case instructions::status:
        send_status(command.id, 0);
        break;

    case instructions::move_to:
        do_move(command.id, std::get_if<point>(&command.payload));
        break;

    case instructions::spi_transfer:
        do_spi_transfer(command.id, std::get_if<spi_transfer_payload>(&command.payload));
        break;

    case instructions::i2c_transfer:
        do_i2c_transfer(command.id, std::get_if<i2c_transfer_payload>(&command.payload));
        break;

    case instructions::get_abs_pos:
    case instructions::get_rel_pos:
    case instructions::set_zero_pos:
    case instructions::reset_zero_pos:
    case instructions::set_coordinate_mode:
    case instructions::home:
    case instructions::set_gpio_mode:
    case instructions::set_gpio:
    case instructions::get_gpio:
    case instructions::set_parameter:
    case instructions::get_parameter:
        send_status(command.id, error_code(error::cube, error::cat::misc, 2));
        return;

    default:
        cube_hw::log_error("Fatal cube_lib::controller error in instr decode\n");
        send_status(0, error_code(error::cube, error::cat::misc, 1));
        return;
    }

}

}
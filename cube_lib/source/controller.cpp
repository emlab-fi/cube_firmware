#include <tuple>
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

void controller::process_command(encoded_message& input) {

    if (input.type == message_type::empty) {
        //do nothing
        return;
    }

    if (input.type != message_type::command) {
        //return some kind of error about incorrect message
        return;
    }

    decode_error dec_err;
    command_message command;
    std::tie(dec_err, command) = decode_cmd_message(input);

    if (dec_err == decode_error::pb_error) {
        //return decoding error
        return;
    }

    if (dec_err == decode_error::wrong_payload) {
        //return wrong playload error
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
        do_move(command.id, command.payload);
        break;

    case instructions::get_abs_pos:
    case instructions::get_rel_pos:
    case instructions::set_zero_pos:
    case instructions::reset_zero_pos:
    case instructions::set_coordinate_mode:
    case instructions::home:
    case instructions::spi_transfer:
    case instructions::i2c_transfer:
    case instructions::set_gpio_mode:
    case instructions::set_gpio:
    case instructions::get_gpio:
    case instructions::set_parameter:
    case instructions::get_parameter:
        //raise non-implemented error
        return;

    default:
        //fatal error
        return;
    }

}

}
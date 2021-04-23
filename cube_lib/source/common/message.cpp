#include "common/message.hpp"
#include "pb_encode.h"
#include "pb_decode.h"
#include "cube.pb.h"

namespace {

status_msg fill_status_msg(const cube::status_message& input) {

    status_msg msg = status_msg_init_zero;
    msg.error_id = input.error_id;
    msg.has_pos = true;
    msg.pos.a = input.position[0];
    msg.pos.b = input.position[1];
    msg.pos.c = input.position[2];

    switch (input.mode) {
        case cube::planner_mode::cartesian:
        msg.mode = coordinate_mode_cartesian;
        break;
        case cube::planner_mode::cylindrical:
        msg.mode = coordinate_mode_cylindrical;
        break;
        case cube::planner_mode::spherical:
        msg.mode = coordinate_mode_spherical;
        break;
    }
    return msg;
}

}//anonymous namespace

namespace cube {

encoded_message status_message::encode() const {

    auto msg = fill_status_msg(*this);
    encoded_message output;
    output.type = message_type::status;

    pb_ostream_t encoding_stream = pb_ostream_from_buffer(output.data.data(), output.data.max_size());

    if (!pb_encode(&encoding_stream, status_msg_fields, &msg)) {
        return {0, message_type::empty, {}};
    }

    output.length = encoding_stream.bytes_written;

    return output;
}

encoded_message reply_message::encode() const {

    reply_msg msg = reply_msg_init_zero;
    encoded_message output;
    output.type = message_type::reply;

    msg.id = id;
    msg.has_stat = true;
    msg.stat = fill_status_msg(status);

    auto index = payload.index();

    if (index == 0) {
        const data_reply_payload& rpl = std::get<data_reply_payload>(payload);
        msg.which_payload = reply_msg_data_tag;
        msg.payload.data.length = rpl.length;
        memcpy(msg.payload.data.data, rpl.data.data(), rpl.length);
    } else if (index == 1) {
        bool state = std::get<bool>(payload);
        msg.which_payload = reply_msg_gpio_status_tag;
        msg.payload.gpio_status = state;
    } else if (index == 2) {
        uint32_t value = std::get<uint32_t>(payload);
        msg.which_payload = reply_msg_param_value_tag;
        msg.payload.param_value = value;
    }

    pb_ostream_t encoding_stream = pb_ostream_from_buffer(output.data.data(), output.data.max_size());

    if (!pb_encode(&encoding_stream, reply_msg_fields, &msg)) {
        return {0, message_type::empty, {}};
    }

    output.length = encoding_stream.bytes_written;

    return output;
}

//this function is extremely ugly, rethink how we are doing things!
decode_return decode_cmd_message(encoded_message& input) {

    if (input.type != message_type::command) {
        return {decode_error::wrong_type, {}};
    }

    command_msg message = command_msg_init_zero;
    command_message command;
    pb_istream_t input_stream = pb_istream_from_buffer(input.data.data(), input.length);
    if ((!pb_decode(&input_stream, command_msg_fields, &message))) {
        return {decode_error::pb_error, {}};
    }

    command.id = message.id;
    command.payload = std::monostate();

    switch (message.inst) {

    case instruction_nop:
        command.instr = instructions::nop;
        break;

    case instruction_status_i:
        command.instr = instructions::status;
        break;
    
    case instruction_get_abs_pos:
        command.instr = instructions::get_abs_pos;
        break;

    case instruction_get_rel_pos:
        command.instr = instructions::get_rel_pos;
        break;

    case instruction_set_zero_pos:
        command.instr = instructions::set_zero_pos;
        break;

    case instruction_reset_zero_pos:
        command.instr = instructions::reset_zero_pos;
        break;

    case instruction_home:
        command.instr = instructions::home;
        break;

    case instruction_get_gpio:
        command.instr = instructions::get_gpio;
        break;

    case instruction_get_parameter:
        command.instr = instructions::get_parameter;
        break;

    case instruction_move_to:
        command.instr = instructions::move_to;
        if (message.which_payload != command_msg_pos_tag) {
            return {decode_error::wrong_payload, {}};
        }
        command.payload = point(message.payload.pos.a, message.payload.pos.b, message.payload.pos.c);
        break;

    case instruction_set_coordinate_mode:
        command.instr = instructions::set_coordinate_mode;
        if (message.which_payload != command_msg_mode_tag) {
            return {decode_error::wrong_payload, {}};
        }
        switch (message.payload.mode) {
            case coord_mode_cartesian:
                command.payload = planner_mode::cartesian;
                break;
            case coord_mode_cylindrical:
                command.payload = planner_mode::cylindrical;
                break;
            case coord_mode_spherical:
                command.payload = planner_mode::spherical;
        };
        break;

    case instruction_spi_transfer:
        command.instr = instructions::spi_transfer;
        if (message.which_payload != command_msg_spi_tag) {
            return {decode_error::wrong_payload, {}};
        }
        break;

    case instruction_i2c_transfer:
        command.instr = instructions::i2c_transfer;
        if (message.which_payload != command_msg_i2c_tag) {
            return {decode_error::wrong_payload, {}};
        }
        break;

    case instruction_set_gpio_mode:
        command.instr = instructions::set_gpio_mode;
        if (message.which_payload != command_msg_gpio_tag) {
            return {decode_error::wrong_payload, {}};
        }
        break;

    case instruction_set_gpio:
        command.instr = instructions::set_gpio;
        if (message.which_payload != command_msg_gpio_tag) {
            return {decode_error::wrong_payload, {}};
        }
        break;

    case instruction_set_parameter:
        command.instr = instructions::set_parameter;
        if (message.which_payload != command_msg_param_tag) {
            return {decode_error::wrong_payload, {}};
        }
        break;

    }

    return {decode_error::no_error, command};
}

}//namespace cube
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
        msg.mode = coord_mode_cartesian;
        break;
        case cube::planner_mode::cylindrical:
        msg.mode = coord_mode_cylindrical;
        break;
        case cube::planner_mode::spherical:
        msg.mode = coord_mode_spherical;
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

    if (index == 1) {
        auto rpl = std::get_if<data_reply_payload>(&payload);
        msg.which_payload = reply_msg_data_tag;
        msg.payload.data.length = rpl->length;
        memcpy(msg.payload.data.data, rpl->data.data(), rpl->length);
    } else if (index == 2) {
        auto state = std::get_if<bool>(&payload);
        msg.which_payload = reply_msg_gpio_status_tag;
        msg.payload.gpio_status = *state;
    } else if (index == 3) {
        auto value = std::get_if<uint32_t>(&payload);
        msg.which_payload = reply_msg_param_value_tag;
        msg.payload.param_value = *value;
    }

    pb_ostream_t encoding_stream = pb_ostream_from_buffer(output.data.data(), output.data.max_size());

    if (!pb_encode(&encoding_stream, reply_msg_fields, &msg)) {
        return {0, message_type::empty, {}};
    }

    output.length = encoding_stream.bytes_written;

    return output;
}

//this function is extremely ugly, rethink how we are doing things!
std::pair<decode_error, command_message> decode_cmd_message(const encoded_message& input) {

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

    command.instr = static_cast<instructions>(message.inst);

    if (command.instr == instructions::move_to) {
        if (message.which_payload != command_msg_pos_tag) {
            return {decode_error::wrong_payload, {}};
        }
        command.payload = point(message.payload.pos.a, message.payload.pos.b, message.payload.pos.c);
    }

    if (command.instr == instructions::set_coordinate_mode) {
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
    }

    if (command.instr == instructions::spi_transfer) {
        if (message.which_payload != command_msg_spi_tag) {
            return {decode_error::wrong_payload, {}};
        }

        uint32_t cs = message.payload.spi.cs;
        uint32_t length = message.payload.spi.length;
        uint32_t mode = message.payload.spi.mode;

        command.payload = spi_transfer_payload{cs, mode, length, {0}};

        auto dest = std::get<spi_transfer_payload>(command.payload).data.data();
        auto src = message.payload.spi.data;
        memcpy(dest, src, length);
    }

    if (command.instr == instructions::i2c_transfer) {
        if (message.which_payload != command_msg_i2c_tag) {
            return {decode_error::wrong_payload, {}};
        }

        uint32_t addr = message.payload.i2c.address;
        uint32_t rx_len = message.payload.i2c.rx_length;
        uint32_t tx_len = message.payload.i2c.tx_length;

        command.payload = i2c_transfer_payload{rx_len, tx_len, addr, {0}};

        auto dest = std::get<i2c_transfer_payload>(command.payload).data.data();
        auto src = message.payload.i2c.data;
        memcpy(dest, src, tx_len);
    }

    if (command.instr == instructions::set_gpio_mode ||
        command.instr == instructions::set_gpio) {
        if (message.which_payload != command_msg_gpio_tag) {
            return {decode_error::wrong_payload, {}};
        }
        command.payload = gpio_config_payload{message.payload.gpio.index, message.payload.gpio.value};
    }

    if (command.instr == instructions::set_parameter) {
        command.instr = instructions::set_parameter;
        if (message.which_payload != command_msg_param_tag) {
            return {decode_error::wrong_payload, {}};
        }
        command.payload = param_config_payload{message.payload.param.id, message.payload.param.value};
    }

    return {decode_error::no_error, command};
}

}//namespace cube

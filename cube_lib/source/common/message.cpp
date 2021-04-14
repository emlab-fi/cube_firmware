#include <variant>
#include "common/message.hpp"
#include "pb_encode.h"
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

    


    pb_ostream_t encoding_stream = pb_ostream_from_buffer(output.data.data(), output.data.max_size());

    if (!pb_encode(&encoding_stream, reply_msg_fields, &msg)) {
        return {0, message_type::empty, {}};
    }

    output.length = encoding_stream.bytes_written;

    return output;
}

}//namespace cube
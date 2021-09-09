#pragma once
#include "planner.hpp"
#include "common/message.hpp"

namespace cube {

class controller {
    planner motion_planner;

    void send_simple_reply(uint32_t id, uint32_t error);

    void do_move_instr(uint32_t id, point* target);
    void do_i2c_transfer(uint32_t id, i2c_transfer_payload* data);
    void do_spi_transfer(uint32_t id, spi_transfer_payload* data);
    void do_home(uint32_t id);


public:
    void process_command(encoded_message& input);
};

}
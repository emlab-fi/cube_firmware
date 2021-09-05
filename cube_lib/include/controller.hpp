#pragma once
#include "planner.hpp"
#include "common/message.hpp"

namespace cube {

class controller {
    planner motion_planner;

    void send_status(uint32_t id, uint32_t error);

    void do_move(uint32_t id, point* target);
    void do_i2c_transfer(uint32_t id, i2c_transfer_payload* data);
    void do_spi_transfer(uint32_t id, spi_transfer_payload* data);

public:
    void process_command(encoded_message& input);
};

}
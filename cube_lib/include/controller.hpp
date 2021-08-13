#pragma once
#include "planner.hpp"
#include "common/message.hpp"

namespace cube {

class controller {
    planner motion_planner;

    void send_status(uint32_t id, uint32_t error);

public:
    void process_command(encoded_message& input);
};

}
#pragma once
#include "planner.hpp"
#include "common/message.hpp"

namespace cube {

class controller {
    planner motion_planner;

    void send_simple_reply(uint32_t id, uint32_t error);

    void send_absolute_pos(uint32_t id);

    void instr_move(uint32_t id, point* target);
    void instr_home(uint32_t id);

    void instr_i2c_transfer(uint32_t id, i2c_transfer_payload* data);
    void instr_spi_transfer(uint32_t id, spi_transfer_payload* data);
    void instr_set_gpio_mode(uint32_t id, gpio_config_payload* data);
    void instr_set_gpio(uint32_t id, gpio_config_payload* data);
    void instr_get_gpio(uint32_t id, gpio_config_payload* data);


public:

    controller(const planner_config& cfg) : motion_planner(cfg) {}

    void process_command(encoded_message& input);
};

}
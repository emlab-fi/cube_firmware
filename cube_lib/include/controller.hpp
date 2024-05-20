/// @file
/// @brief This file contains cube controller logic 

#pragma once
#include "planner.hpp"
#include "common/message.hpp"

namespace cube {

/// @brief controller class for command processing
///
/// every reply send by controller contains status data (heartbeat)
///
class controller {
    planner motion_planner;

    /// @brief sends simple reply containing status without payload
    /// @param id of previously received command tied to this reply
    /// @param error code (0 = noerror)
    void send_simple_reply(uint32_t id, uint32_t error);

    /// @brief sends status with absolute pos instead of default relative without payload
    /// @param id of previously received command tied to this reply
    void send_absolute_pos(uint32_t id);

    /// @brief move motors to desired target
    /// @param id of previously received command tied to this reply
    /// @param target position where we want to end up
    void instr_move(uint32_t id, point* target);

    /// @brief execute homing sequence
    /// @param id of previously received command tied to this reply
    void instr_home(uint32_t id);

    void instr_i2c_transfer(uint32_t id, i2c_transfer_payload* data);
    void instr_spi_transfer(uint32_t id, spi_transfer_payload* data);
    void instr_set_gpio_mode(uint32_t id, gpio_config_payload* data);
    void instr_set_gpio(uint32_t id, gpio_config_payload* data);
    void instr_get_gpio(uint32_t id, gpio_config_payload* data);
    void instr_set_param(uint32_t id, param_config_payload value);
    void instr_get_param(uint32_t id, param_config_payload value);

public:

    controller(const planner_config& cfg) : motion_planner(cfg) {}

    void process_command(encoded_message& input);
};

} // namespace cube

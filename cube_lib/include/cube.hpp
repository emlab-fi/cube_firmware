/// @file
/// @brief This file contains infinite main loop

#pragma once
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

/// @brief class containing main loop
class cube_main {
    controller ctrl;

public:
    cube_main(const planner_config& cfg) : ctrl(cfg) {}

    /// @brief infinite main loop
    /// sequentially parses and executes received commands
    void main_loop() {
        auto msg = cube_hw::get_message();
        if (msg) {
            ctrl.process_command(*msg);
        }
    }
};

} // namespace cube

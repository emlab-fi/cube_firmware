#pragma once
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

class cube_main {
    controller ctrl;

public:
    cube_main(const planner_config& cfg) : ctrl(cfg) {}

    void main_loop() {
        auto msg = cube_hw::get_message();
        if (msg) {
            ctrl.process_command(*msg);
        }
    }
};

} //namespace cube
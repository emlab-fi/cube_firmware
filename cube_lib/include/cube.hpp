#pragma once
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

class cube_main {
    controller ctrl;

    void main_loop() {
        auto msg = cube_hw::get_message();
        if (msg) {
            ctrl.process_command(*msg);
        }
    }
};

} //namespace cube
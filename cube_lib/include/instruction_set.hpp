#pragma once

namespace cube {

    enum class instructions : int {
        nop = 0,
        status = 1,
        move_to = 2,
        get_abs_pos = 3,
        get_rel_pos = 4,
        set_zero_pos = 5,
        reset_zero_pos = 6,
        set_coordinate_mode = 7,
        home = 8,
        spi_transfer = 9,
        i2c_transfer = 10,
        set_gpio_mode = 11,
        set_gpio = 12,
        get_gpio = 13,
        set_parameter = 14,
        get_parameter = 15
    };

} //namespace cube
namespace cube {

    enum class instructions {
        nop,
        status,
        move_to,
        get_abs_pos,
        get_rel_pos,
        set_zero_pos,
        set_coordinate_mode,
        home,
        spi_transfer,
        i2c_transfer,
        set_gpio_mode,
        set_gpio,
        get_gpio,
        set_parameter,
        get_parameter
    };

} //namespace cube
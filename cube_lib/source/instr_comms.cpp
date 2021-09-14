#include "errors.hpp"
#include "controller.hpp"
#include "hardware.hpp"

namespace cube {

void controller::instr_i2c_transfer(uint32_t id, i2c_transfer_payload* data) {
    return;
}


void controller::instr_spi_transfer(uint32_t id, spi_transfer_payload* data) {
    return;
}


void controller::instr_set_gpio_mode(uint32_t id, gpio_config_payload* data) {
    return;
}


void controller::instr_set_gpio(uint32_t id, gpio_config_payload* data) {
    return;
}


void controller::instr_get_gpio(uint32_t id, gpio_config_payload* data) {
    return;
}

} //namespace cube
#include "hardware.hpp"

namespace cube_hw {

std::optional<cube::encoded_message> get_message() { return {}; }

status send_message(const cube::encoded_message& msg) { return status::error; }

}
#pragma once
#include <array>
#include <variant>
#include <stdint.h>
#include "../planner.hpp"
#include "instruction_set.hpp"
#include "coordinate.hpp"

namespace cube {

enum class message_type {
    command,
    reply,
    status,
    empty
};

struct spi_transfer_payload {
    uint32_t cs;
    uint32_t length;
    std::array<uint8_t, 64> data;
};

struct i2c_transfer_payload {
    uint32_t rx_length;
    uint32_t tx_length;
    uint32_t address;
    std::array<uint8_t, 64> data;
};

struct data_reply_payload {
    uint32_t length;
    std::array<uint8_t, 64> data;
};

struct gpio_config_payload {
    uint32_t index;
    bool value;
};

struct param_config_payload {
    uint32_t id;
    uint32_t value;
};

struct encoded_message {
    std::size_t length;
    message_type type;
    std::array<uint8_t, 256> data;
};

struct status_message {
    uint32_t error_id;
    planner_mode mode;
    point position;

    encoded_message encode() const;
};

struct command_message {
    uint32_t id;
    instructions instr;
    status_message status;
    std::variant<
        planner_mode,
        spi_transfer_payload,
        i2c_transfer_payload,
        gpio_config_payload,
        param_config_payload
        > payload;

    //redundant, as we should never have to encode a command message in cube_hw
    encoded_message encode() const {
        return {0, message_type::command, {}};
    }
};

struct reply_message {
    uint32_t id;
    status_message status;
    std::variant<
        data_reply_payload,
        bool,
        uint32_t
        > payload;

    encoded_message encode() const;
};

command_message decode_cmd_message(encoded_message& input);

}//namespace cube

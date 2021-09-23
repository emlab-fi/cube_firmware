#pragma once
#include <cstdint>
#include <array>
#include "common/message.hpp"

constexpr std::size_t BUFFER_SIZE = 8;

enum class parse_state {
    start, header, type, length, reading
};

class serial_buffer {
    std::array<cube::encoded_message, BUFFER_SIZE> buffer;

    std::size_t read_index;
    std::size_t write_index;

    parse_state state;

    uint32_t bytes_read;

public:
    serial_buffer() : read_index(0U),
                      write_index(0U),
                      state(parse_state::start),
                      bytes_read(0U)
    {}

    void new_byte(const uint8_t data) {
        switch (state) {
        case parse_state::start:
            if (data == 0x55) {
                state = parse_state::header;
                bytes_read = 1;
            }
            break;

        case parse_state::header:
            if (data == 0x55) {
                ++bytes_read;
            } else {
                state = parse_state::start;
            }
            if (bytes_read == 3) {
                state = parse_state::type;
                bytes_read = 0;
            }
            break;

        case parse_state::type:
            if (data == 0x01) {
                buffer[write_index].type = cube::message_type::command;
            }
            else if (data == 0x02) {
                buffer[write_index].type = cube::message_type::reply;
            }
            else {
                buffer[write_index].type = cube::message_type::empty;
            }
            state = parse_state::length;
            break;

        case parse_state::length:
            buffer[write_index].length = data;
            state = parse_state::reading;
            break;

        case parse_state::reading:
            buffer[write_index].data[bytes_read] = data;
            ++bytes_read;
            if (bytes_read == buffer[write_index].length) {
                ++write_index;
                if (write_index == BUFFER_SIZE) {
                    write_index = 0;
                }
                state = parse_state::start;
            }
            break;

        default:
            state = parse_state::start;
            break;
        }
    }

    bool empty() const {
        return read_index == write_index;
    }

    cube::encoded_message read() {
        std::size_t temp = read_index;

        ++read_index;
        if (read_index == BUFFER_SIZE) {
            read_index = 0;
        }

        return buffer[temp];
    }
};
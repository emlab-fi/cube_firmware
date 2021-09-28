#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_gpio.h"

#include "hardware.hpp"
#include "config.hpp"
#include "serial_buffer.hpp"
#include "printf.h"

static serial_buffer receive_buffer{};

extern "C" {

void UART0_IRQHandler(void) {
    //we need to clear the RDRF flag by reading S1 register first
    //the SDK doesnt do it properly, and it is mentioned in single line in 2k pages of documentation
    uint8_t temp = UART0_PERIPHERAL->S1;
    receive_buffer.new_byte(UART_ReadByte(UART0_PERIPHERAL));
}

} // extern "C"

namespace {

void write_message(const char symbol, const char * fmt, va_list args) {
    uint8_t buffer[256] = {0};

    int written = snprintf((char*)buffer, 256, "[%c] ", symbol);

    if (written == -1) {
        return;
    }

    UART_WriteBlocking(UART3_PERIPHERAL, buffer, written);

    written = vsnprintf((char*)buffer, 256, fmt, args);

    if (written == -1) {
        return;
    }

    UART_WriteBlocking(UART3_PERIPHERAL, buffer, written);
}

} //anonymous namespace

namespace cube_hw {

void log_info(const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    write_message('i', fmt, args);
    va_end(args);
}

void log_warning(const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    write_message('w', fmt, args);
    va_end(args);
}

void log_error(const char * fmt, ...) {
    va_list args;

    va_start(args, fmt);
    write_message('e', fmt, args);
    va_end(args);
}

status init_hardware() {
}

std::optional<cube::encoded_message> get_message() {
    if (!receive_buffer.empty()) {
        return receive_buffer.read();
    }
    return {};
}

status send_message(const cube::encoded_message& msg) {
    uint8_t header[5] = {0x55, 0x55, 0x55, 0, static_cast<uint8_t>(msg.length)};

    switch(msg.type) {
    case cube::message_type::command:
        header[3] = 0x01;
        break;

    case cube::message_type::reply:
        header[3] = 0x02;
        break;

    case cube::message_type::status:
        header[3] = 0x03;
        break;

    default:
        return status::msg_send_error;
    }

    UART_WriteBlocking(UART0_PERIPHERAL, header, 5);
    UART_WriteBlocking(UART0_PERIPHERAL, msg.data.data(), msg.length);
    return status::no_error;
}

status set_motor_power(bool enabled) {
    GPIO_PinWrite(CUBE_A_ENABLE_GPIO, CUBE_A_ENABLE_PIN, enabled);
    GPIO_PinWrite(CUBE_B_ENABLE_GPIO, CUBE_B_ENABLE_PIN, enabled);
    GPIO_PinWrite(CUBE_Z1_ENABLE_GPIO, CUBE_Z1_ENABLE_PIN, enabled);
    GPIO_PinWrite(CUBE_Z2_ENABLE_GPIO, CUBE_Z2_ENABLE_PIN, enabled);
    return status::no_error;
}

status do_steps(int32_t a, int32_t b, int32_t c) {
    log_info("cube_hw: steps|a:%d b:%d c:%d\n", a, b, c);
    static int32_t pos_a = 0;
    static int32_t pos_b = 0;
    static int32_t pos_c = 0;

    pos_a += a;
    pos_b += b;
    pos_c += c;

    auto res = steppers_ptr->setMode(TMC429_Driver::RampMode::POSITION);
    if (res != TMC429_Driver::Status::success) {
        log_error("cube_hw: tmc429 mode set fail\n");
        return status::error;
    }

    res = steppers_ptr->doSteps(pos_a, pos_b, pos_c);
    if (res != TMC429_Driver::Status::success) {
        log_error("cube_hw: tmc429 pos set fail\n");
        return status::error;
    }

    auto status = steppers_ptr->getStepperStatus();
    while (!status.stepper1_pos || !status.stepper2_pos || !status.stepper3_pos) {
        status = steppers_ptr->getStepperStatus();
        if (status.status != TMC429_Driver::Status::success) {
            log_error("cube_hw: tmc429 status get fail\n");
            return status::error;
        }
    }
    return status::no_error;
}

status do_velocity(int32_t a, int32_t b, int32_t c) {
    auto res = steppers_ptr->setMode(TMC429_Driver::RampMode::VELOCITY);
    if (res != TMC429_Driver::Status::success) {
        return status::error;
    }

    res = steppers_ptr->setSpeed(a, b, c);
    if (res != TMC429_Driver::Status::success) {
        return status::error;
    }

    return status::no_error;
}


std::pair<status, cube::data_reply_payload> i2c_transfer(cube::i2c_transfer_payload* msg) {
    log_info("cube_hw: i2c transfer - addr: %x tx_len: %u rx_len: %u\n", msg->address, msg->tx_length, msg->rx_length);

    status_t result;
    result = I2C_MasterStart(I2C0_PERIPHERAL, msg->address, kI2C_Write);
    if (result != kStatus_Success) {
        return {status::i2c_transfer_error, {} };
    }
    result = I2C_MasterWriteBlocking(I2C0_PERIPHERAL, msg->data.data(), msg->tx_length, kI2C_TransferNoStopFlag);
    if (result != kStatus_Success) {
        return {status::i2c_transfer_error, {} };
    }
    result = I2C_MasterRepeatedStart(I2C0_PERIPHERAL, msg->address, kI2C_Write);
    if (result != kStatus_Success) {
        return {status::i2c_transfer_error, {} };
    }

    cube::data_reply_payload reply{};
    reply.length = msg->rx_length;

    result = I2C_MasterReadBlocking(I2C0_PERIPHERAL, reply.data.data(), reply.length, kI2C_TransferDefaultFlag);
    if (result != kStatus_Success) {
        return {status::i2c_transfer_error, {} };
    }

    for (auto item : reply.data) {
        log_info("thing_test: %u\n", item);
    }

    return {status::no_error, reply};
}

std::pair<status, cube::data_reply_payload> spi_transfer(cube::spi_transfer_payload*) {
    return {status::error, {}};
}

status set_gpio_mode(cube::gpio_config_payload*) {
    return status::error;
}

status set_gpio(cube::gpio_config_payload*) {
    return status::error;
}

std::pair<status, bool> read_gpio(cube::gpio_config_payload*) {
    return {status::error, {}};
}

uint8_t limits_status() {
    uint8_t flag = 0x00;

    if (GPIO_PinRead(CUBE_X_LIMIT_GPIO, CUBE_X_LIMIT_PIN) == 0) {
        flag |= A_LIMIT_START;
    }
    if (GPIO_PinRead(CUBE_Y_LIMIT_GPIO, CUBE_Y_LIMIT_PIN) == 0) {
        flag |= B_LIMIT_START;
    }
    if (GPIO_PinRead(CUBE_Z_LIMIT_GPIO, CUBE_Z_LIMIT_PIN) == 0) {
        flag |= C_LIMIT_START;
    }

    return flag;
}

} //namespace cube_hw

/*
namespace {
    float calculateRatio(Cube_Common::Steps_t steps) {
        if (steps.a == 0 || steps.b == 0) {
            return 1.f;
        }
        int32_t abs_a = abs(steps.a);
        int32_t abs_b = abs(steps.b);
        if (abs_a > abs_b) {
            return static_cast<float>(abs_b)/abs_a;
        } else {
            return static_cast<float>(abs_a)/abs_b;
        }
    }
}
*/
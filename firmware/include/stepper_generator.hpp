#pragma once
#include <array>
#include <cinttypes>
#include <cmath>
#include <stm32g4xx_hal.h>
#include "hardware.hpp"
#include "config.hpp"


namespace cube_hw
{

enum class motor_state{
    RESET,  // not initialized
    IDLE,   // standby
    READY,  // dma ready
    SKIP,   // dma ready with 0 steps
    BUSY    // running
};

/// @brief generator of PWM for stepper motor
class StepperGenerator {
    TIM_HandleTypeDef& _htim;
    const unsigned _channel;
    bool _flip_direction = false;
    motor_state _state = motor_state::RESET;

    unsigned _dma_idx;
    float _dma_ms;
    std::array<uint16_t, DMA_MEM_SIZE> _dma_buffer;

    void insert_dma(int auto_reload, int repetition_c);
    void insert_dma_padding(int auto_reload);

public:
    StepperGenerator(TIM_HandleTypeDef& htim, unsigned channel);

    /// @brief starts the timer base
    status start_tim_base();

    /// @brief htim tied to this step generator
    const TIM_HandleTypeDef* htim() const;

    /// @brief get current motor state
    motor_state state() const;

    /// @brief get precalculated motor runtime
    float dma_ms() const;
    
    /// @brief negates the result of @ref set_direction
    void flip_direction();

    /// @brief sets the given direction pin
    void set_direction(bool forward, GPIO_TypeDef* PORT, const int PIN);

    /// @brief precalculates acceleration curve
    status prepare_dma(int32_t steps);

    /// @brief stretches the acceleration curve so it fits max_time
    void adjust_timings(float max_time, int32_t steps);

    /// @brief starts dma transfer
    status start();

    /// @brief must be called after the dma finished transfer
    void finished_callback();
};

} // namespace cube_hw

/// @file
/// @brief This file is responsible for device and peripherals initialisation

#pragma once
#include "drivers/tmc2209.hpp"
#include "drivers/mcp23008.hpp"
#include "stepper_generator.hpp"
#include "core_xy.hpp"
#include "main.hpp"


/// @brief Uart handle for communication and data interface
extern UART_HandleTypeDef data_uart;

/// @brief Uart handle for debug info logging
extern UART_HandleTypeDef debug_uart;

/// @brief Trinamic Motion Control - physical debugger?
extern UART_HandleTypeDef tmc_uart;

/// @brief I2C handle for transferring measured data
extern I2C_HandleTypeDef hi2c3;

/// @brief Timer1 used as PWM for stepper motor X - CORE Xy
extern TIM_HandleTypeDef htim1;
/// @brief Timer8 used as PWM for stepper motor Z - vertical
extern TIM_HandleTypeDef htim8;
/// @brief Timer20 used as PWM for stepper motor Y - CORE xY
extern TIM_HandleTypeDef htim20;

/// @brief DMA for Timer1
extern DMA_HandleTypeDef hdma_tim1_up;
/// @brief DMA for Timer8
extern DMA_HandleTypeDef hdma_tim8_up;
/// @brief DMA for Timer20
extern DMA_HandleTypeDef hdma_tim20_up;

/// @brief TMC driver for first CoreXY motor
extern cube_hw::TMC2209 tmc_driver_x;
/// @brief TMC driver for second CoreXY motor
extern cube_hw::TMC2209 tmc_driver_y;
/// @brief TMC driver for first motor of vertical motors pair
extern cube_hw::TMC2209 tmc_driver_z1;
/// @brief TMC driver for second motor of vertical motors pair
extern cube_hw::TMC2209 tmc_driver_z2;

/// @brief step signal generator for first CoreXY motor
extern cube_hw::StepperGenerator stepper_generator_x;
/// @brief step signal generator for second CoreXY motor
extern cube_hw::StepperGenerator stepper_generator_y;
/// @brief step signal generator for z axis motor
//extern cube_hw::StepperGenerator stepper_generator_z;

extern cube_hw::CoreXY core_xy;


constexpr uint16_t LIMIT_X_STOP = IO1_Pin;
constexpr uint16_t LIMIT_X_START = IO2_Pin;
constexpr uint16_t LIMIT_Y_STOP = IO3_Pin;
constexpr uint16_t LIMIT_Y_START = IO4_Pin;
constexpr uint16_t LIMIT_Z_STOP = IO5_Pin;
constexpr uint16_t LIMIT_Z_START = IO6_Pin;

/// @brief System clock initialisation function 
void SystemClock_Config(void);

/// @brief USART1 (data) initialisation function
void MX_USART1_UART_Init(void);

/// @brief USART2 (debug) initialisation function
void MX_USART2_UART_Init(void);

/// @brief USART3 (TMC) initialisation function
void MX_USART3_UART_Init(void);

/// @brief I2C3 (measurement data) initialisation function
void MX_I2C3_Init(void);

/// @brief GPIO initialisation function
void MX_GPIO_Init(void);

/// @brief TIM1 generated initialisation function
void MX_TIM1_Init(void);

/// @brief TIM8 generated initialisation function
void MX_TIM8_Init(void);

/// @brief TIM20 generated initialisation function
void MX_TIM20_Init(void);

/// @brief TIM1 generated post initialisation function
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/// @brief DMA generated initialisation function
void MX_DMA_Init(void);

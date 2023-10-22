/// @file
/// @brief This file is responsible for device and peripherals initialisation

#pragma once
#include "main.hpp"

/// @brief Uart handle for communication and data interface
extern UART_HandleTypeDef data_uart;

/// @brief Uart handle for debug info logging
extern UART_HandleTypeDef debug_uart;

/// @brief Trinamic Motion Control - physical debugger?
extern UART_HandleTypeDef tmc_uart;

/// @ brief I2C handle for transferring measured data
extern I2C_HandleTypeDef hi2c3;
 

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

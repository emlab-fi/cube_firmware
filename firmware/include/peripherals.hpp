#pragma once
#include "main.hpp"

extern UART_HandleTypeDef data_uart;
extern UART_HandleTypeDef debug_uart;
extern UART_HandleTypeDef tmc_uart;

extern I2C_HandleTypeDef hi2c3;

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

void MX_I2C3_Init(void);

void MX_GPIO_Init(void);
#pragma once
#include <stm32g4xx_hal.h>

extern "C" {

#define DEBUG_LED3_Pin GPIO_PIN_2
#define DEBUG_LED3_GPIO_Port GPIOE
#define DEBUG_LED4_Pin GPIO_PIN_3
#define DEBUG_LED4_GPIO_Port GPIOE
#define DEBUG_LED5_Pin GPIO_PIN_4
#define DEBUG_LED5_GPIO_Port GPIOE
#define DEBUG_LED6_Pin GPIO_PIN_5
#define DEBUG_LED6_GPIO_Port GPIOE
#define IO1_Pin GPIO_PIN_6
#define IO1_GPIO_Port GPIOE
#define STEP_OUT1_Pin GPIO_PIN_0
#define STEP_OUT1_GPIO_Port GPIOC
#define STEP_OUT4_Pin GPIO_PIN_2
#define STEP_OUT4_GPIO_Port GPIOC
#define DATA_UART_TX_Pin GPIO_PIN_4
#define DATA_UART_TX_GPIO_Port GPIOC
#define DATA_UART_RX_Pin GPIO_PIN_5
#define DATA_UART_RX_GPIO_Port GPIOC
#define IO2_Pin GPIO_PIN_7
#define IO2_GPIO_Port GPIOE
#define IO3_Pin GPIO_PIN_8
#define IO3_GPIO_Port GPIOE
#define IO4_Pin GPIO_PIN_9
#define IO4_GPIO_Port GPIOE
#define IO5_Pin GPIO_PIN_10
#define IO5_GPIO_Port GPIOE
#define IO6_Pin GPIO_PIN_11
#define IO6_GPIO_Port GPIOE
#define IO7_Pin GPIO_PIN_12
#define IO7_GPIO_Port GPIOE
#define IO8_Pin GPIO_PIN_13
#define IO8_GPIO_Port GPIOE
#define IO9_Pin GPIO_PIN_14
#define IO9_GPIO_Port GPIOE
#define IO10_Pin GPIO_PIN_15
#define IO10_GPIO_Port GPIOE
#define STEP_UART_TX_Pin GPIO_PIN_10
#define STEP_UART_TX_GPIO_Port GPIOB
#define ENABLE_OUT1_Pin GPIO_PIN_8
#define ENABLE_OUT1_GPIO_Port GPIOD
#define ENABLE_OUT2_Pin GPIO_PIN_9
#define ENABLE_OUT2_GPIO_Port GPIOD
#define ENABLE_OUT3_Pin GPIO_PIN_10
#define ENABLE_OUT3_GPIO_Port GPIOD
#define ENABLE_OUT4_Pin GPIO_PIN_11
#define ENABLE_OUT4_GPIO_Port GPIOD
#define DIAG_IN1_Pin GPIO_PIN_12
#define DIAG_IN1_GPIO_Port GPIOD
#define DIAG_IN2_Pin GPIO_PIN_13
#define DIAG_IN2_GPIO_Port GPIOD
#define DIAG_IN3_Pin GPIO_PIN_14
#define DIAG_IN3_GPIO_Port GPIOD
#define DIAG_IN4_Pin GPIO_PIN_15
#define DIAG_IN4_GPIO_Port GPIOD
#define STEP_OUT2_Pin GPIO_PIN_6
#define STEP_OUT2_GPIO_Port GPIOC
#define STEP_OUT3_Pin GPIO_PIN_7
#define STEP_OUT3_GPIO_Port GPIOC
#define I2C_SCL_Pin GPIO_PIN_8
#define I2C_SCL_GPIO_Port GPIOC
#define I2C_SDA_Pin GPIO_PIN_9
#define I2C_SDA_GPIO_Port GPIOC
#define DIR1_OUT_Pin GPIO_PIN_0
#define DIR1_OUT_GPIO_Port GPIOD
#define DIR2_OUT_Pin GPIO_PIN_1
#define DIR2_OUT_GPIO_Port GPIOD
#define DIR3_OUT_Pin GPIO_PIN_2
#define DIR3_OUT_GPIO_Port GPIOD
#define DIR4_OUT_Pin GPIO_PIN_3
#define DIR4_OUT_GPIO_Port GPIOD
#define DEBUG_UART_TX_Pin GPIO_PIN_5
#define DEBUG_UART_TX_GPIO_Port GPIOD
#define DEBUG_UART_RX_Pin GPIO_PIN_6
#define DEBUG_UART_RX_GPIO_Port GPIOD
#define DEBUG_LED1_Pin GPIO_PIN_0
#define DEBUG_LED1_GPIO_Port GPIOE
#define DEBUG_LED2_Pin GPIO_PIN_1
#define DEBUG_LED2_GPIO_Port GPIOE

void Error_Handler(void);

int main(void);

} //extern C
#include "main.hpp"
#include "peripherals.hpp"
#include "stepper_generator.hpp"
#include "config.hpp"
#include "cube.hpp"

void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  cube_hw::log_error("inside error handler...\n");
  __disable_irq();
  while (1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  HAL_TIM_DMABurst_WriteStop(htim, TIM_DMA_UPDATE);

  if (htim == stepper_generator_x.htim()) {
    stepper_generator_x.finished_callback();
  }
  if (htim == stepper_generator_y.htim()) {
    stepper_generator_y.finished_callback();
  }

}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C3_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_USART3_UART_Init();
    MX_TIM1_Init();
    MX_TIM8_Init();
    MX_TIM20_Init();

    if (tmc_driver_x.configure() != cube_hw::status::no_error ||
        tmc_driver_y.configure() != cube_hw::status::no_error ||
        tmc_driver_z1.configure() != cube_hw::status::no_error ||
        tmc_driver_z2.configure() != cube_hw::status::no_error) {
        cube_hw::log_error("Failed to configure TMC drivers\n");
        Error_Handler();
    }

    if (stepper_generator_x.start_tim_base() != cube_hw::status::no_error ||
        stepper_generator_y.start_tim_base() != cube_hw::status::no_error) {
        cube_hw::log_error("Failed to start timers");
        Error_Handler();
    }

    cube::cube_main cube_core{planner_conf};

    cube_hw::log_info("System initialized!\n");

    while(1) {
      HAL_GPIO_TogglePin(DEBUG_LED1_GPIO_Port, DEBUG_LED1_Pin);
      HAL_GPIO_TogglePin(DEBUG_LED2_GPIO_Port, DEBUG_LED2_Pin);
      HAL_GPIO_TogglePin(DEBUG_LED3_GPIO_Port, DEBUG_LED3_Pin);
      HAL_GPIO_TogglePin(DEBUG_LED4_GPIO_Port, DEBUG_LED4_Pin);
      HAL_GPIO_TogglePin(DEBUG_LED5_GPIO_Port, DEBUG_LED5_Pin);
      HAL_GPIO_TogglePin(DEBUG_LED6_GPIO_Port, DEBUG_LED6_Pin);
      cube_core.main_loop();
    }
}

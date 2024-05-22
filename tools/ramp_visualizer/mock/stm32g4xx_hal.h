#ifndef STM32G4xx_HAL_H
#define STM32G4xx_HAL_H

#include <cstdint>
#include <iostream>

// DEFINES
#define TIM_CHANNEL_1 1
#define TIM_CHANNEL_2 2
#define TIM_CHANNEL_3 3
#define TIM_CHANNEL_4 4
#define TIM_CHANNEL_5 5
#define TIM_CHANNEL_6 6

#define TIM_DMABurstLength_3Transfers 3
#define TIM_DMABurstLength_4Transfers 4
#define TIM_DMABurstLength_5Transfers 5
#define TIM_DMABurstLength_6Transfers 6
#define TIM_DMABurstLength_7Transfers 7
#define TIM_DMABurstLength_8Transfers 8

#define TIM_DMABASE_ARR 0
#define TIM_DMA_UPDATE 0

// Types
typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef enum {
  GPIO_PIN_RESET = 0U,
  GPIO_PIN_SET
} GPIO_PinState;

// Structs
struct TIM_HandleTypeDef{
  unsigned ARR = 0;
};
struct GPIO_TypeDef{};

// Macros
#define __HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__) do{(__HANDLE__)->ARR = (__AUTORELOAD__);} while(0)
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__)

// Functions
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef*);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef*, uint32_t);
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef*, uint32_t);
HAL_StatusTypeDef HAL_TIM_DMABurst_MultiWriteStart(TIM_HandleTypeDef*, uint32_t, uint32_t, uint32_t*, uint32_t , uint32_t);
void HAL_GPIO_WritePin(GPIO_TypeDef*, uint16_t, GPIO_PinState);

namespace cube_hw
{
    void log_info(const char*, ...);
    void log_error(const char*, ...);
} // namespace cube_hw


#endif // STM32
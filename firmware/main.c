#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_bus.h>
#include <stm32g4xx_ll_system.h>
#include <stm32g4xx_ll_utils.h>
#include <stm32g4xx_ll_pwr.h>

#define LED_PORT GPIOE
#define LED6_PIN LL_GPIO_PIN_5
#define LED5_PIN LL_GPIO_PIN_4

void SystemClockSetup(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  LL_RCC_HSI_Enable();
   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_2, 16, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_Enable();
   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_Init1msTick(64000000);

  LL_SetSystemCoreClock(64000000);
}

int main() {
    SystemClockSetup();
    SystemCoreClockUpdate();
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
    LL_GPIO_SetPinMode(LED_PORT, LED5_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LED_PORT, LED6_PIN, LL_GPIO_MODE_OUTPUT);

    //SysTick_Config(SystemCoreClock / 1000);

    uint32_t counter = 0U;

    for (;;) {
        ++counter;
        if(counter = 1000000) {
            LL_GPIO_TogglePin(LED_PORT, LED6_PIN | LED5_PIN);
            counter = 0;
        }
    }

    return 0;
}
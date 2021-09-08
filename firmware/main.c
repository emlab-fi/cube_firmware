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

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
    while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
    {
    }
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSI_Enable();
    /* Wait till HSI is ready */
    while(LL_RCC_HSI_IsReady() != 1)
    {
    }

    LL_RCC_HSI_SetCalibTrimming(64);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {
    }

    /* Set AHB prescaler*/
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_Init1msTick(16000000);

    LL_SetSystemCoreClock(16000000);
}

int main() {
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    NVIC_SetPriorityGrouping(((uint32_t)0x00000003));
    LL_PWR_DisableUCPDDeadBattery();

    SystemClock_Config();
    SystemCoreClockUpdate();

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
    LL_GPIO_SetPinMode(LED_PORT, LED5_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(LED_PORT, LED6_PIN, LL_GPIO_MODE_OUTPUT);

    uint32_t counter = 0U;

    for (;;) {
        ++counter;
        if(counter == 8000000) {
            LL_GPIO_TogglePin(LED_PORT, LED6_PIN | LED5_PIN);
            counter = 0;
        }
    }

    return 0;
}
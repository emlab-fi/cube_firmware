#include <stm32g4xx_ll_gpio.h>
#include <stm32g4xx_ll_rcc.h>
#include <stm32g4xx_ll_cortex.h>
#include <stm32g4xx_ll_bus.h>

#define LED_PORT GPIOE
#define LED_PIN LL_GPIO_PIN_0

/*
void SysTick_Handler(void) {
    static int counter = 0;
    counter++;

    if ((counter % 500) == 0) {
        LL_GPIO_TogglePin(LED_PORT, LED_PIN);
    }
}
*/

void init_gpio() {
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);

    LL_GPIO_SetPinMode(LED_PORT, LED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinOutputType(LED_PORT, LED_PIN, LL_GPIO_OUTPUT_PUSHPULL);
}

int main(void) {
    SystemCoreClockUpdate();

    init_gpio();

    LL_GPIO_SetOutputPin(LED_PORT, LED_PIN);

    //SysTick_Config(SystemCoreClock / 1000);

    uint32_t counter = 0U;

    for (;;) {
        ++counter;
    }

    return 0;
}
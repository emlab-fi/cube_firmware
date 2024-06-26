/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v10.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VLL12
mcu_data: ksdk2_0
processor_version: 10.0.0
board: FRDM-K64F
pin_labels:
- {pin_num: '90', pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b, label: 'J1[2]', identifier: TMR_1588_0;Z_LIMIT}
- {pin_num: '91', pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b, label: 'J1[4]', identifier: TMR_1588_1;Y_LIMIT}
- {pin_num: '57', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, label: 'J1[6]', identifier: Z2_ENABLE}
- {pin_num: '69', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, label: 'J1[10]', identifier: Z1_ENABLE}
- {pin_num: '72', pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS, label: 'J1[14]', identifier: X_LIMIT}
- {pin_num: '73', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: 'J1[16]', identifier: CLOCK_20}
- {pin_num: '64', pin_signal: PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA, label: 'J1[1]', identifier: A_ENABLE}
- {pin_num: '65', pin_signal: PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB, label: 'J1[3]', identifier: B_ENABLE}
- {pin_num: '81', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, label: 'J1[9]', identifier: Y_FAULT;B_FAULT}
- {pin_num: '77', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2, label: 'J1[15]', identifier: Z2_FAULT}
- {pin_num: '79', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, label: 'J1[13]', identifier: CMP0_IN1;Z1_FAULT}
- {pin_num: '70', pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1, label: 'J1[11]', identifier: X_FAULT;A_FAULT}
- {pin_num: '76', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT, label: 'J2[4]', identifier: TMC429_INT}
- {pin_num: '93', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b, label: 'J2[6]', identifier: SPI_CS}
- {pin_num: '95', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL, label: 'J2[8]', identifier: UART2_RX;SPI_MOSI}
- {pin_num: '96', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA, label: 'J2[10]', identifier: UART2_TX;SPI_MISO}
- {pin_num: '94', pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b, label: 'J2[12]', identifier: SPI_CLK}
- {pin_num: '32', pin_signal: ADC0_SE18/PTE25/UART4_RX/I2C0_SDA/EWM_IN, label: 'J2[18]/U8[6]/I2C0_SDA', identifier: ACCEL_SDA;I2C_SDA}
- {pin_num: '31', pin_signal: ADC0_SE17/PTE24/UART4_TX/I2C0_SCL/EWM_OUT_b, label: 'J2[20]/U8[4]/I2C0_SCL', identifier: ACCEL_SCL;I2C_SCL}
- {pin_num: '58', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1, label: 'J4[6]', identifier: UART3_RX}
- {pin_num: '59', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2, label: 'J4[8]', identifier: UART3_TX}
- {pin_num: '82', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, label: 'J4[12]', identifier: TOGGLE}
- {pin_num: '85', pin_signal: PTC13/UART4_CTS_b/FB_AD26, label: 'U8[11]/SW2', identifier: ACCEL_INT2}
- {pin_num: '62', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN, label: 'U7[4]/UART0_RX', identifier: DEBUG_UART_RX;UART0_RX}
- {pin_num: '63', pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b, label: 'U10[1]/UART0_TX', identifier: DEBUG_UART_TX;UART0_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    CUBE_Pins();
    CUBE_UART0Pins();
    CUBE_I2CPins();
    CUBE_SPIPins();
    CUBE_UART3Pins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
CUBE_Pins:
- options: {callFromInitBoot: 'true', prefix: CUBE_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: FB, signal: CLKOUT, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '76', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT, direction: INPUT, gpio_interrupt: kPORT_InterruptOrDMADisabled}
  - {pin_num: '72', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS, direction: INPUT}
  - {pin_num: '91', peripheral: GPIOC, signal: 'GPIO, 17', pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b, identifier: Y_LIMIT,
    direction: INPUT}
  - {pin_num: '90', peripheral: GPIOC, signal: 'GPIO, 16', pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b, identifier: Z_LIMIT,
    direction: INPUT}
  - {pin_num: '70', peripheral: GPIOC, signal: 'GPIO, 0', pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1, identifier: A_FAULT, direction: INPUT}
  - {pin_num: '81', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, identifier: B_FAULT, direction: INPUT}
  - {pin_num: '79', peripheral: GPIOC, signal: 'GPIO, 7', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, identifier: Z1_FAULT, direction: INPUT}
  - {pin_num: '77', peripheral: GPIOC, signal: 'GPIO, 5', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2, direction: INPUT}
  - {pin_num: '64', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA, direction: OUTPUT, gpio_init_state: 'true',
    open_drain: disable, pull_select: up}
  - {pin_num: '65', peripheral: GPIOB, signal: 'GPIO, 19', pin_signal: PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB, direction: OUTPUT, gpio_init_state: 'true',
    open_drain: disable, pull_select: up}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, direction: OUTPUT, gpio_init_state: 'true', open_drain: disable,
    pull_select: up}
  - {pin_num: '57', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, direction: OUTPUT, gpio_init_state: 'true', open_drain: disable,
    pull_select: up}
  - {pin_num: '82', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : CUBE_Pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void CUBE_Pins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t Z2_ENABLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB9 (pin 57)  */
    GPIO_PinInit(CUBE_Z2_ENABLE_GPIO, CUBE_Z2_ENABLE_PIN, &Z2_ENABLE_config);

    gpio_pin_config_t A_ENABLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB18 (pin 64)  */
    GPIO_PinInit(CUBE_A_ENABLE_GPIO, CUBE_A_ENABLE_PIN, &A_ENABLE_config);

    gpio_pin_config_t B_ENABLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB19 (pin 65)  */
    GPIO_PinInit(CUBE_B_ENABLE_GPIO, CUBE_B_ENABLE_PIN, &B_ENABLE_config);

    gpio_pin_config_t Z1_ENABLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB23 (pin 69)  */
    GPIO_PinInit(CUBE_Z1_ENABLE_GPIO, CUBE_Z1_ENABLE_PIN, &Z1_ENABLE_config);

    gpio_pin_config_t A_FAULT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC0 (pin 70)  */
    GPIO_PinInit(CUBE_A_FAULT_GPIO, CUBE_A_FAULT_PIN, &A_FAULT_config);

    gpio_pin_config_t X_LIMIT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC2 (pin 72)  */
    GPIO_PinInit(CUBE_X_LIMIT_GPIO, CUBE_X_LIMIT_PIN, &X_LIMIT_config);

    gpio_pin_config_t TMC429_INT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC4 (pin 76)  */
    GPIO_PinInit(CUBE_TMC429_INT_GPIO, CUBE_TMC429_INT_PIN, &TMC429_INT_config);

    gpio_pin_config_t Z2_FAULT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC5 (pin 77)  */
    GPIO_PinInit(CUBE_Z2_FAULT_GPIO, CUBE_Z2_FAULT_PIN, &Z2_FAULT_config);

    gpio_pin_config_t Z1_FAULT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC7 (pin 79)  */
    GPIO_PinInit(CUBE_Z1_FAULT_GPIO, CUBE_Z1_FAULT_PIN, &Z1_FAULT_config);

    gpio_pin_config_t B_FAULT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC9 (pin 81)  */
    GPIO_PinInit(CUBE_B_FAULT_GPIO, CUBE_B_FAULT_PIN, &B_FAULT_config);

    gpio_pin_config_t TOGGLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC10 (pin 82)  */
    GPIO_PinInit(CUBE_TOGGLE_GPIO, CUBE_TOGGLE_PIN, &TOGGLE_config);

    gpio_pin_config_t Z_LIMIT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC16 (pin 90)  */
    GPIO_PinInit(CUBE_Z_LIMIT_GPIO, CUBE_Z_LIMIT_PIN, &Z_LIMIT_config);

    gpio_pin_config_t Y_LIMIT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC17 (pin 91)  */
    GPIO_PinInit(CUBE_Y_LIMIT_GPIO, CUBE_Y_LIMIT_PIN, &Y_LIMIT_config);

    /* PORTB18 (pin 64) is configured as PTB18 */
    PORT_SetPinMux(CUBE_A_ENABLE_PORT, CUBE_A_ENABLE_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[18] = ((PORTB->PCR[18] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                      | PORT_PCR_ODE(kPORT_OpenDrainDisable)

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));

    /* PORTB19 (pin 65) is configured as PTB19 */
    PORT_SetPinMux(CUBE_B_ENABLE_PORT, CUBE_B_ENABLE_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[19] = ((PORTB->PCR[19] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                      | PORT_PCR_ODE(kPORT_OpenDrainDisable)

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));

    /* PORTB23 (pin 69) is configured as PTB23 */
    PORT_SetPinMux(CUBE_Z1_ENABLE_PORT, CUBE_Z1_ENABLE_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[23] = ((PORTB->PCR[23] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                      | PORT_PCR_ODE(kPORT_OpenDrainDisable)

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));

    /* PORTB9 (pin 57) is configured as PTB9 */
    PORT_SetPinMux(CUBE_Z2_ENABLE_PORT, CUBE_Z2_ENABLE_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[9] = ((PORTB->PCR[9] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                     /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
                     | PORT_PCR_ODE(kPORT_OpenDrainDisable)

                     /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullUp));

    /* PORTC0 (pin 70) is configured as PTC0 */
    PORT_SetPinMux(CUBE_A_FAULT_PORT, CUBE_A_FAULT_PIN, kPORT_MuxAsGpio);

    /* PORTC10 (pin 82) is configured as PTC10 */
    PORT_SetPinMux(CUBE_TOGGLE_PORT, CUBE_TOGGLE_PIN, kPORT_MuxAsGpio);

    /* PORTC16 (pin 90) is configured as PTC16 */
    PORT_SetPinMux(CUBE_Z_LIMIT_PORT, CUBE_Z_LIMIT_PIN, kPORT_MuxAsGpio);

    /* PORTC17 (pin 91) is configured as PTC17 */
    PORT_SetPinMux(CUBE_Y_LIMIT_PORT, CUBE_Y_LIMIT_PIN, kPORT_MuxAsGpio);

    /* PORTC2 (pin 72) is configured as PTC2 */
    PORT_SetPinMux(CUBE_X_LIMIT_PORT, CUBE_X_LIMIT_PIN, kPORT_MuxAsGpio);

    /* PORTC3 (pin 73) is configured as CLKOUT */
    PORT_SetPinMux(CUBE_CLOCK_20_PORT, CUBE_CLOCK_20_PIN, kPORT_MuxAlt5);

    /* PORTC4 (pin 76) is configured as PTC4 */
    PORT_SetPinMux(CUBE_TMC429_INT_PORT, CUBE_TMC429_INT_PIN, kPORT_MuxAsGpio);

    /* Interrupt configuration on PORTC4 (pin 76): Interrupt/DMA request is disabled */
    PORT_SetPinInterruptConfig(CUBE_TMC429_INT_PORT, CUBE_TMC429_INT_PIN, kPORT_InterruptOrDMADisabled);

    /* PORTC5 (pin 77) is configured as PTC5 */
    PORT_SetPinMux(CUBE_Z2_FAULT_PORT, CUBE_Z2_FAULT_PIN, kPORT_MuxAsGpio);

    /* PORTC7 (pin 79) is configured as PTC7 */
    PORT_SetPinMux(CUBE_Z1_FAULT_PORT, CUBE_Z1_FAULT_PIN, kPORT_MuxAsGpio);

    /* PORTC9 (pin 81) is configured as PTC9 */
    PORT_SetPinMux(CUBE_B_FAULT_PORT, CUBE_B_FAULT_PIN, kPORT_MuxAsGpio);

    SIM->SOPT2 = ((SIM->SOPT2 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT2_CLKOUTSEL_MASK)))

                  /* CLKOUT select: FlexBus CLKOUT. */
                  | SIM_SOPT2_CLKOUTSEL(SOPT2_CLKOUTSEL_FLEXBUS));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
CUBE_UART0Pins:
- options: {callFromInitBoot: 'true', prefix: CUBE_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b, identifier: UART0_TX, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN, identifier: UART0_RX, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : CUBE_UART0Pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void CUBE_UART0Pins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t UART0_RX = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as UART0_RX */
                                        kPORT_MuxAlt3,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB16 (pin 62) is configured as UART0_RX */
    PORT_SetPinConfig(CUBE_UART0_RX_PORT, CUBE_UART0_RX_PIN, &UART0_RX);

    const port_pin_config_t UART0_TX = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as UART0_TX */
                                        kPORT_MuxAlt3,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB17 (pin 63) is configured as UART0_TX */
    PORT_SetPinConfig(CUBE_UART0_TX_PORT, CUBE_UART0_TX_PIN, &UART0_TX);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
CUBE_I2CPins:
- options: {callFromInitBoot: 'true', prefix: CUBE_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '32', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE18/PTE25/UART4_RX/I2C0_SDA/EWM_IN, identifier: I2C_SDA, slew_rate: fast, open_drain: enable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '31', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE17/PTE24/UART4_TX/I2C0_SCL/EWM_OUT_b, identifier: I2C_SCL, slew_rate: fast, open_drain: enable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : CUBE_I2CPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void CUBE_I2CPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t I2C_SCL = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is enabled */
                                       kPORT_OpenDrainEnable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as I2C0_SCL */
                                       kPORT_MuxAlt5,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE24 (pin 31) is configured as I2C0_SCL */
    PORT_SetPinConfig(CUBE_I2C_SCL_PORT, CUBE_I2C_SCL_PIN, &I2C_SCL);

    const port_pin_config_t I2C_SDA = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is enabled */
                                       kPORT_OpenDrainEnable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as I2C0_SDA */
                                       kPORT_MuxAlt5,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE25 (pin 32) is configured as I2C0_SDA */
    PORT_SetPinConfig(CUBE_I2C_SDA_PORT, CUBE_I2C_SDA_PIN, &I2C_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
CUBE_SPIPins:
- options: {callFromInitBoot: 'true', prefix: CUBE_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: SPI0, signal: PCS0_SS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
  - {pin_num: '94', peripheral: SPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
  - {pin_num: '95', peripheral: SPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL, identifier: SPI_MOSI}
  - {pin_num: '96', peripheral: SPI0, signal: SIN, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA, identifier: SPI_MISO}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : CUBE_SPIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void CUBE_SPIPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD0 (pin 93) is configured as SPI0_PCS0 */
    PORT_SetPinMux(CUBE_SPI_CS_PORT, CUBE_SPI_CS_PIN, kPORT_MuxAlt2);

    /* PORTD1 (pin 94) is configured as SPI0_SCK */
    PORT_SetPinMux(CUBE_SPI_CLK_PORT, CUBE_SPI_CLK_PIN, kPORT_MuxAlt2);

    /* PORTD2 (pin 95) is configured as SPI0_SOUT */
    PORT_SetPinMux(CUBE_SPI_MOSI_PORT, CUBE_SPI_MOSI_PIN, kPORT_MuxAlt2);

    /* PORTD3 (pin 96) is configured as SPI0_SIN */
    PORT_SetPinMux(CUBE_SPI_MISO_PORT, CUBE_SPI_MISO_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
CUBE_UART3Pins:
- options: {callFromInitBoot: 'true', prefix: CUBE_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '58', peripheral: UART3, signal: RX, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: '59', peripheral: UART3, signal: TX, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : CUBE_UART3Pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void CUBE_UART3Pins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB10 (pin 58) is configured as UART3_RX */
    PORT_SetPinMux(CUBE_UART3_RX_PORT, CUBE_UART3_RX_PIN, kPORT_MuxAlt3);

    /* PORTB11 (pin 59) is configured as UART3_TX */
    PORT_SetPinMux(CUBE_UART3_TX_PORT, CUBE_UART3_TX_PIN, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/

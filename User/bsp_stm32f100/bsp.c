/**
 ******************************************************************************
 * @file    bsp.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-05
 * @brief   provide basic hardware driver and configuration.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

/* ll */
#include "bsp.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
/* board support pack */
#include "bsp_DS3231.h"
#include "bsp_HV57708.h"
#include "bsp_i2c_gpio.h"
#include "bsp_iwdg.h"
#include "bsp_key.h"
#include "bsp_neon_bulb.h"
#include "bsp_rtc.h"
#include "bsp_timer.h"
#include "bsp_ws2812b.h"

static void SystemClock_Config(void);

/**
 * @brief  initialize hardwares and configuration structures.
 * @param  None.
 * @retval None.
 */
void bsp_init(void)
{
    SystemClock_Config();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#if USE_JTAG_AS_GPIO
    // Turn off the mapping of JTAG, use PB3, PB4, PA15 as GPIO
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_GPIO_AF_Remap_SWJ_NOJTAG();
#endif
    /* Initialize hardwares */
    key_init();
    timer_100us_init();
    I2C_init();
    HV57708_init();
    ws2812b_init();
    neon_bulbs_init();
    IWDG_init();
    RTC_init();
}

/**
 * @brief  This function will be run when idling.
 * @param  None.
 * @retval None.
 */
void bsp_idle(void)
{
    IWDG_feed();
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 24000000
 *            HCLK(Hz)                       = 24000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1
 *            APB2 Prescaler                 = 1
 *            HSE Frequency(Hz)              = 8000000
 *            HSE PREDIV1                    = 2
 *            PLLMUL                         = 6
 *            Flash Latency(WS)              = 0
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
    /* Enable HSE oscillator */
    LL_RCC_HSE_Enable();
    while (LL_RCC_HSE_IsReady() != 1)
    {};

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_2, LL_RCC_PLL_MUL_6);
    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1)
    {};

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {};

    /* Set APB1 & APB2 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    SystemCoreClockUpdate();
}

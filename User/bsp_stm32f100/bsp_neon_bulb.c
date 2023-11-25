/**
 ******************************************************************************
 * @file    bsp_neon_bulb.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2021-08-05
 * @brief   driver for neon_bulb.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "bsp_neon_bulb.h"
#include "bsp.h"
#include "bsp_define.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

#define NEON_BULB_SET_PIN(PIN)                                                 \
    SET_PIN(NEON_BULB_##PIN##_GPIO_PORT, NEON_BULB_##PIN##_GPIO_PIN)
#define NEON_BULB_RESET_PIN(PIN)                                               \
    RESET_PIN(NEON_BULB_##PIN##_GPIO_PORT, NEON_BULB_##PIN##_GPIO_PIN)

////TIM

// #define NEON_BULB_TIMx                      TIM3

// #define NEON_BULB_TIM_APBxClock_FUN         RCC_APB1PeriphClockCmd
// #define NEON_BULB_TIM_CLK                   RCC_APB1Periph_TIM3
// #define NEON_BULB_TIM_GPIO_APBxClock_FUN    RCC_APB2PeriphClockCmd
// #define NEON_BULB_TIM_GPIO_CLK (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

// #define NEON_BULB_GPIO_REMAP_FUN()
// GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

// #define NEON_BULB_TIM_LED_PORT              GPIOB
// #define NEON_BULB_TIM_LED_PIN               GPIO_Pin_9

// #define NEON_BULB_TIM_OCxInit               TIM_OC2Init
// #define NEON_BULB_TIM_OCxPreloadConfig      TIM_OC2PreloadConfig
// #define NEON_BULB_CCRx                      CCR2
// #define NEON_BULB_TIM_IT                    TIM_IT_CC2

// #define NEON_BULB_TIMx_IRQn                 TIM3_IRQn
// #define NEON_BULB_TIMx_IRQHandler           TIM3_IRQHandler

void neon_bulbs_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_GPIO_CLOCK(NEON_BULB_UP_GPIO_PORT)
                             | LL_GPIO_CLOCK(NEON_BULB_DOWN_GPIO_PORT));

    GPIO_InitStruct.Pin = LL_GPIO_PIN(NEON_BULB_UP_GPIO_PIN);
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(LL_GPIO_PORT(NEON_BULB_UP_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(NEON_BULB_DOWN_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(NEON_BULB_DOWN_GPIO_PORT), &GPIO_InitStruct);
}

void neon_bulbs_on(void)
{
    NEON_BULB_SET_PIN(UP);
    NEON_BULB_SET_PIN(DOWN);
}

void neon_bulbs_off(void)
{
    NEON_BULB_RESET_PIN(UP);
    NEON_BULB_RESET_PIN(DOWN);
}

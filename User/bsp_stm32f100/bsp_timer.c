/**
 ******************************************************************************
 * @file    bsp_timer.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-07
 * @brief   driver for timer.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "bsp_timer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"

void bsp_init_timer(void)
{
    SysTick_Config(SystemCoreClock / 1000);
    bsp_init_100us_Timer();
}

extern void bsp_RunPer1ms(void);
extern void bsp_RunPer10ms(void);
void SysTick_Handler(void)
{
    static uint8_t s_count = 0;
    bsp_RunPer1ms();

    if (++s_count >= 10)
    {
        s_count = 0;
        bsp_RunPer10ms();
    }
}

void bsp_init_100us_Timer(void)
{
    LL_TIM_InitTypeDef TIM_InitStruct;

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    LL_TIM_StructInit(&TIM_InitStruct);
    TIM_InitStruct.Prescaler = SystemCoreClock / 10000;
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 0xFFFF;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    TIM_InitStruct.RepetitionCounter = 0x00000000U;
    (void)LL_TIM_Init(TIM6, &TIM_InitStruct);

    LL_TIM_EnableCounter(TIM6);
}

uint16_t bsp_get_100us_time(void)
{
    return TIM6->CNT;
}

uint16_t bsp_check_100us_time(uint16_t last_time)
{
    return TIM6->CNT - last_time;
}

void bsp_delay_n100us(uint16_t n100us)
{
    uint16_t start_time = bsp_get_100us_time();
    while (bsp_check_100us_time(start_time) < n100us)
        ;
}

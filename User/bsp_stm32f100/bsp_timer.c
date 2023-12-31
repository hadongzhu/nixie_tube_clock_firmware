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
#include "bsp.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_tim.h"
#include "tick.h"
#include "key.h"

volatile uint32_t global_tick = 0U;

void SysTick_Handler(void)
{
    static uint32_t count = 0;
    global_tick++;
    tick_inc(&tick_controller_entity, 1);
    if (++count >= 10)
    {
        count -= 10;
        key_run(&key_controller_entity);
    }
}

void timer_100us_init(void)
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

uint16_t get_100us_time(void)
{
    return TIM6->CNT;
}

uint16_t check_100us_time(uint16_t last_time)
{
    return TIM6->CNT - last_time;
}

void delay_n100us(uint16_t n100us)
{
    uint16_t start_time = get_100us_time();
    while (check_100us_time(start_time) < n100us)
    {
        bsp_idle();
    }
}

/**
 ******************************************************************************
 * @file    bsp_iwdg.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-12-02
 * @brief   Driver of iwdg.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "bsp_iwdg.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"

void IWDG_init(void)
{
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {}

    LL_IWDG_Enable(IWDG);
    LL_IWDG_EnableWriteAccess(IWDG);
    LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_8);
    LL_IWDG_SetReloadCounter(IWDG, 0xFEE);
    while (LL_IWDG_IsReady(IWDG) != 1)
    {}
    LL_IWDG_ReloadCounter(IWDG);
}

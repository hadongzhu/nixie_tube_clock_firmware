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
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_exti.h"

void IWDG_init(void)
{
    /* Enable the peripheral clock of DBG register (uncomment for debug purpose)
     */
    /* -------------------------------------------------------------------------
     */
    /*  LL_DBGMCU_APB1_GRP1_FreezePeriph(LL_DBGMCU_APB1_GRP1_IWDG_STOP); */

    /* Enable the peripheral clock IWDG */
    /* -------------------------------- */
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {}

    /* Configure the IWDG with window option disabled */
    /* ------------------------------------------------------- */
    /* (1) Enable the IWDG by writing 0x0000 CCCC in the IWDG_KR register */
    /* (2) Enable register access by writing 0x0000 5555 in the IWDG_KR register
     */
    /* (3) Write the IWDG prescaler by programming IWDG_PR from 0 to 7 -
     * LL_IWDG_PRESCALER_4 (0) is lowest divider*/
    /* (4) Write the reload register (IWDG_RLR) */
    /* (5) Wait for the registers to be updated (IWDG_SR = 0x0000 0000) */
    /* (6) Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000 AAAA) */
    LL_IWDG_Enable(IWDG);                            /* (1) */
    LL_IWDG_EnableWriteAccess(IWDG);                 /* (2) */
    LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_4); /* (3) */
    LL_IWDG_SetReloadCounter(IWDG, 0xFEE);           /* (4) */
    while (LL_IWDG_IsReady(IWDG) != 1)               /* (5) */
    {}
    LL_IWDG_ReloadCounter(IWDG);                     /* (6) */
}

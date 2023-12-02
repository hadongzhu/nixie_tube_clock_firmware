/**
 ******************************************************************************
 * @file    bsp_iwdg.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-12-02
 * @brief   Driver of internal flash.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_IWDG_H
#define __BSP_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f1xx_ll_iwdg.h"

void IWDG_init(void);
static inline void IWDG_feed(void)
{
    LL_IWDG_ReloadCounter(IWDG);
}

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C_GPIO_H */

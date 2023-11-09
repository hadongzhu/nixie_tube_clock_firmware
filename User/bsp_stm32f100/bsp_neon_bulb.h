/**
 ******************************************************************************
 * @file    bsp_neon_bulb.h
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

#ifndef __BSP_NEON_BULB_H
#define __BSP_NEON_BULB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

void neon_bulbs_init(void);
void neon_bulbs_on(void);
void neon_bulbs_off(void);

#ifdef __cplusplus
}
#endif

#endif /* __NEON_BULB_H */

/**
 ******************************************************************************
 * @file    bsp.h
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

#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Set USE_JTAG_AS_GPIO 1 to use pins of JTAG as GPIO pins */
#define USE_JTAG_AS_GPIO 1

void bsp_init(void);
void bsp_idle(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H */

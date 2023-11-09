/**
 ******************************************************************************
 * @file    bsp_timer.h
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

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void bsp_init_timer(void);
void bsp_init_100us_Timer(void);
uint16_t bsp_get_100us_time(void);
uint16_t bsp_check_100us_time(uint16_t last_time);
void bsp_delay_n100us(uint16_t n100us);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_TIMER_H */

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

static inline uint32_t get_global_tick(void)
{
    extern volatile uint32_t global_tick;
    return global_tick;
}

void timer_100us_init(void);
uint16_t get_100us_time(void);
uint16_t check_100us_time(uint16_t last_time);
void delay_n100us(uint16_t n100us);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_TIMER_H */

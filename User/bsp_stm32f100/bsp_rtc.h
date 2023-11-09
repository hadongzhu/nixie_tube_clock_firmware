/**
 ******************************************************************************
 * @file    bsp_rtc.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2021-08-05
 * @brief   driver for rtc.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_RTC_H
#define __BSP_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// #define RTC_CLOCK_SOURCE_LSE       /* LSE */
#define RTC_CLOCK_SOURCE_LSI /* LSI */

#if !defined(RTC_CLOCK_SOURCE_LSI) && !defined(RTC_CLOCK_SOURCE_LSE)
#error "Please select RTC clock source!"
#endif

void RTC_init(void);
void RTC_set_unix_timestamp(uint32_t unix_timestamp);
uint32_t RTC_get_unix_timestamp(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_RTC_H */

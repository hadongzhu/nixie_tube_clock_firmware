/**
 ******************************************************************************
 * @file    bsp_rtc.c
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

#include "bsp_rtc.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_rtc.h"
#include "time_custom.h"

void RTC_init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_BKP | LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
        ;
    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI
        || LL_RTC_TIME_Get(RTC) < 946656000)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
        LL_RCC_EnableRTC();
        (void)LL_RTC_DeInit(RTC);
        LL_RTC_InitTypeDef RTC_InitStruct;
        LL_RTC_StructInit(&RTC_InitStruct);
        RTC_InitStruct.AsynchPrescaler = 40000 - 1;
        RTC_InitStruct.OutPutSource = LL_RTC_CALIB_OUTPUT_NONE;
        (void)LL_RTC_Init(RTC, &RTC_InitStruct);
    }
}

void RTC_set_unix_timestamp(uint32_t unix_timestamp)
{
    LL_RTC_DisableWriteProtection(RTC);
    LL_RTC_TIME_Set(RTC, unix_timestamp);
    LL_RTC_EnableWriteProtection(RTC);
}

uint32_t RTC_get_unix_timestamp(void)
{
    return LL_RTC_TIME_Get(RTC);
}

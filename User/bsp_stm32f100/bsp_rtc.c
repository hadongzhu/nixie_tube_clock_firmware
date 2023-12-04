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
#include "stm32f1xx_ll_cortex.h"
#include "time_custom.h"
#include <stdbool.h>

static bool RTC_LSE_init(void);
static bool RTC_LSI_init(void);
static bool RTC_HSE_DIV128_init(void);

void RTC_init(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();

    if (LL_RCC_IsEnabledRTC() == 0)
    {
        LL_RTC_InitTypeDef RTC_InitStruct;
        LL_RTC_StructInit(&RTC_InitStruct);
        RTC_InitStruct.AsynchPrescaler = LSE_VALUE;
        RTC_InitStruct.OutPutSource = LL_RTC_CALIB_OUTPUT_NONE;
        if (RTC_LSE_init() == false)
        {
            RTC_InitStruct.AsynchPrescaler = HSE_VALUE / 128 - 1;
            if (RTC_HSE_DIV128_init() == false)
            {
                RTC_InitStruct.AsynchPrescaler = 40e3 - 1;
                RTC_LSI_init();
            }
        }
        LL_RCC_EnableRTC();
        (void)LL_RTC_DeInit(RTC);
        (void)LL_RTC_Init(RTC, &RTC_InitStruct);
    }
}

static bool RTC_LSE_init(void)
{
    uint32_t timeout = 0U;
    if (LL_RCC_LSE_IsReady() == 0)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_LSE_Enable();
        timeout = LSE_TIMEOUT_VALUE;
        while (LL_RCC_LSE_IsReady() != 1)
        {
            if (LL_SYSTICK_IsActiveCounterFlag())
            {
                timeout--;
            }
            if (timeout == 0)
            {
                return false;
            }
        }
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);
    }
    return true;
}

static bool RTC_LSI_init(void)
{
    LL_RCC_LSI_Enable();
    uint32_t timeout = LSI_TIMEOUT_VALUE;
    while (LL_RCC_LSI_IsReady() != 1)
    {
        if (LL_SYSTICK_IsActiveCounterFlag())
        {
            timeout--;
        }
        if (timeout == 0)
        {
            return false;
        }
    }
    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_LSI)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    }
    return true;
}

static bool RTC_HSE_DIV128_init(void)
{
    uint32_t timeout = LSI_TIMEOUT_VALUE;
    if (LL_RCC_HSE_IsReady() == 0)
    {
        LL_RCC_HSE_Enable();
        timeout = HSE_TIMEOUT_VALUE;
        while (LL_RCC_HSE_IsReady() != 1)
        {
            if (LL_SYSTICK_IsActiveCounterFlag())
            {
                timeout--;
            }
            if (timeout == 0)
            {
                return false;
            }
        }
    }
    if (LL_RCC_GetRTCClockSource() != LL_RCC_RTC_CLKSOURCE_HSE_DIV128)
    {
        LL_RCC_ForceBackupDomainReset();
        LL_RCC_ReleaseBackupDomainReset();
        LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_HSE_DIV128);
    }
    return true;
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

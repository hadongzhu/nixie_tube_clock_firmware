/**
 ******************************************************************************
 * @file    bsp_DS3231.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2022-03-07
 * @brief   provide driver for DS3231.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_DS3231_H
#define __BSP_DS3231_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

#define DS3231_DEV_ADDRESS             0xD0
// BCD,00-59
#define DS3231_ADDRESS_SECONDS         0x00
// BCD,00-59
#define DS3231_ADDRESS_MINUTES         0x01
// BCD,00-12/24
#define DS3231_ADDRESS_HOURS           0x02
// 0-7
#define DS3231_ADDRESS_DAY             0x03
// BCD,00-31
#define DS3231_ADDRESS_DATE            0x04
// BCD,00-12
#define DS3231_ADDRESS_MONTH_CENTURY   0x05
// BCD,00-99
#define DS3231_ADDRESS_YEAR            0x06
// BCD,00-59
#define DS3231_ADDRESS_ALARM1_SECONDS  0x07
#define DS3231_DATA_A1M1               0x80
// BCD,00-59
#define DS3231_ADDRESS_ALARM1_MINUTES  0x08
#define DS3231_DATA_A1M2               0x80
// BCD,00-12/24
#define DS3231_ADDRESS_ALARM1_HOURS    0x09
#define DS3231_DATA_A1M3               0x80
// BCD,00-31
#define DS3231_ADDRESS_ALARM1_DAY_DATE 0x0A
#define DS3231_DATA_A1M4               0x80
#define DS3231_DATA_A1DY               0x40
#define DS3231_DATA_A1DT               0x00
// BCD,00-59
#define DS3231_ADDRESS_ALARM2_MINUTES  0x0B
#define DS3231_DATA_A2M2               0x80
// BCD,00-12/24
#define DS3231_ADDRESS_ALARM2_HOURS    0x0C
#define DS3231_DATA_A2M3               0x80
// BCD,00-31
#define DS3231_ADDRESS_ALARM2_DAY_DATE 0x0D
#define DS3231_DATA_A2M4               0x80
#define DS3231_DATA_A2DY               0x40
#define DS3231_DATA_A2DT               0x40
#define DS3231_ADDRESS_CONTROL         0x0E
#define DS3231_DATA_EOSC               0x80
#define DS3231_DATA_EOSC_ENABLE        0x00 // Enable Oscillator
#define DS3231_DATA_EOSC_DISABLE       0x80 // Disable Oscillator
#define DS3231_DATA_BBSQW              0x40
#define DS3231_DATA_BBSQW_ENABLE       0x40 // Battery-Backed Square-Wave Enable
#define DS3231_DATA_BBSQW_DISABLE      0x00 // Battery-Backed Square-Wave Disable
#define DS3231_DATA_CONV               0x20 // Convert Temperature
#define DS3231_DATA_RATE_1HZ           0x00
#define DS3231_DATA_RATE_1204HZ        0x08
#define DS3231_DATA_RATE_4096HZ        0x10
#define DS3231_DATA_RATE_8192HZ        0x18
#define DS3231_DATA_INTCN              0x04
#define DS3231_DATA_INTCN_SQUARE_WAVE  0x00
#define DS3231_DATA_INTCN_INT          0x04
#define DS3231_DATA_A2IE               0x02
#define DS3231_DATA_A2IE_ENABLE        0x02 // Alarm 2 Interrupt Enable
#define DS3231_DATA_A2IE_DISABLE       0x00 // Alarm 2 Interrupt Disable
#define DS3231_DATA_A1IE               0x01
#define DS3231_DATA_A1IE_ENABLE        0x01 // Alarm 2 Interrupt Enable
#define DS3231_DATA_A1IE_DISABLE       0x00 // Alarm 2 Interrupt Disable
#define DS3231_ADDRESS_CONTROL_STATUS  0x0F
#define DS3231_DATA_OSF                0x80
#define DS3231_DATA_EN32KHZ            0x08
#define DS3231_DATA_BSY                0x04
#define DS3231_DATA_A2F                0x02
#define DS3231_DATA_A1F                0x01
#define DS3231_ADDRESS_AGING_OFFSET    0x10
#define DS3231_ADDRESS_MSB_OF_TEMP     0x11
#define DS3231_ADDRESS_LSB_OF_TEMP     0x12

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t day;  // Range 1-7
    uint8_t date; // Range 1-31
    uint8_t month;
    uint8_t year; // Range 0-99
} ds3231_date_time_type;

void ds3231_set_date_time(ds3231_date_time_type *DS3231_Date);
void ds3231_get_date_time(ds3231_date_time_type *DS3231_Date);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DS3231_H */

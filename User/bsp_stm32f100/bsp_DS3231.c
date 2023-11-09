/**
 ******************************************************************************
 * @file    bsp_DS3231.c
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

#include "bsp_DS3231.h"
#include "bsp_i2c_gpio.h"

static inline uint8_t dec2bcd(uint8_t dec)
{
    return dec + (dec / 10) * 6;
}

static inline uint8_t bcd2dec(uint8_t bcd)
{
    return bcd - (bcd >> 4) * 6;
}

void ds3231_set_date_time(ds3231_date_time_type *ds3231_date)
{
    ds3231_date_time_type ds3231_date_temp;
    ds3231_date_temp.seconds = dec2bcd(ds3231_date->seconds);
    ds3231_date_temp.minutes = dec2bcd(ds3231_date->minutes);
    ds3231_date_temp.hour = dec2bcd(ds3231_date->hour);
    ds3231_date_temp.day = dec2bcd(ds3231_date->day);
    ds3231_date_temp.date = dec2bcd(ds3231_date->date);
    ds3231_date_temp.month = dec2bcd(ds3231_date->month);
    ds3231_date_temp.year = dec2bcd(ds3231_date->year);
    I2C_write_multiple_bytes(DS3231_DEV_ADDRESS, DS3231_ADDRESS_SECONDS, 7,
                             (uint8_t *)&ds3231_date_temp);
}

void ds3231_get_date_time(ds3231_date_time_type *ds3231_date)
{
    I2C_read_multiple_bytes(DS3231_DEV_ADDRESS, DS3231_ADDRESS_SECONDS, 7,
                            (uint8_t *)ds3231_date);
    ds3231_date->seconds = bcd2dec(ds3231_date->seconds);
    ds3231_date->minutes = bcd2dec(ds3231_date->minutes);
    ds3231_date->hour = bcd2dec(ds3231_date->hour);
    ds3231_date->day = bcd2dec(ds3231_date->day);
    ds3231_date->date = bcd2dec(ds3231_date->date);
    ds3231_date->month = bcd2dec(ds3231_date->month);
    ds3231_date->year = bcd2dec(ds3231_date->year);
}



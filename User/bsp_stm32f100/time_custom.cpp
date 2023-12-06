/**
 ******************************************************************************
 * @file    time_custom.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide clock driver.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "time_custom.h"

time_custom::unix_timestamp time_custom::readabel_convert_to_unix_timestamp(
    const time_custom::readable &readable)
{
    // 946656000 is the Unix timestamp of 2000-01-01 00:00:00
    time_custom::unix_timestamp unix_timestamp{
        unix_timestamp_of_2000_01_01_00_00, readable.millisecond};
    uint16_t day = 0;
    // add the seconds, minutes and hours to the UnixTimestamp
    unix_timestamp.second
        += readable.hour * 60 * 60 + readable.minutes * 60 + readable.seconds;
    // calculate the number of days of the current year
    for (uint8_t Month = 0; Month < 12 && Month < readable.month - 1; Month++)
    {
        day += days_of_each_month[Month];
    }
    // calculate the number of days of the previous years, notice 2000 is a leap
    // year
    day += readable.year * 365 + readable.year / 4 - readable.year / 100
           + readable.year / 400 + 1 + readable.month_day - 1
           - (((readable.month <= 2)
               && (readable.year % 400 == 0
                   || (readable.year % 4 == 0 && readable.year % 100 != 0)))
                  ? 1
                  : 0);
    unix_timestamp.second += day * 60 * 60 * 24;
    return unix_timestamp;
}

time_custom::readable time_custom::unix_timestamp_convert_to_readable(
    const time_custom::unix_timestamp &unix_timestamp)
{
    // 946656000 is the Unix timestamp of 2000-01-01 00:00:00
    uint32_t unix_timestamp_second = unix_timestamp.second - 946656000;
    time_custom::readable readable{
        unix_timestamp.millisecond, 0, 0, 0, 0, 0, 1, 0};
    uint32_t day = 0;
    uint32_t days_this_year = 0;
    uint8_t days_this_month = 0;
    // calculate the number of days of the current year
    while (true)
    {
        days_this_year
            = 365
              + ((readable.year % 400 == 0
                  || (readable.year % 4 == 0 && readable.year % 100 != 0))
                     ? 1
                     : 0);
        if (day + days_this_year > unix_timestamp_second / (60 * 60 * 24))
        {
            break;
        }
        else
        {
            day += days_this_year;
            readable.year++;
        }
    }
    // calculate the number of days of the current month
    while (true)
    {
        days_this_month
            = days_of_each_month[readable.month - 1]
              + ((readable.month == 2
                  && ((readable.year % 4 == 0 && readable.year % 100 != 0)
                      || readable.year % 400 == 0))
                     ? 1
                     : 0);
        if (day + days_this_month > unix_timestamp_second / (60 * 60 * 24))
        {
            break;
        }
        else
        {
            day += days_this_month;
            readable.month++;
        }
    }
    readable.month_day = unix_timestamp_second / (60 * 60 * 24) - day + 1;
    day += readable.month_day - 1;
    // calculate the number of seconds of the current day
    unix_timestamp_second -= day * 60 * 60 * 24;
    readable.hour = unix_timestamp_second / (60 * 60);
    unix_timestamp_second -= readable.hour * 60 * 60;
    readable.minutes = unix_timestamp_second / 60;
    unix_timestamp_second -= readable.minutes * 60;
    readable.seconds = unix_timestamp_second;
    // 2000-01-01 is saturday
    readable.week_day = (day + 5) % 7 + 1;
    return readable;
}

bool time_custom::is_readable_valid(const time_custom::readable &readable)
{
    if (readable.month < 1 || readable.month > 12)
    {
        return false;
    }
    if (readable.month_day < 1
        || readable.month_day > days_of_each_month[readable.month - 1]
                                    + ((readable.month == 2
                                        && ((readable.year % 4 == 0
                                             && readable.year % 100 != 0)
                                            || readable.year % 400 == 0))
                                           ? 1
                                           : 0))
    {
        return false;
    }
    if (readable.week_day < 1 || readable.week_day > 7)
    {
        return false;
    }
    if (readable.hour > 23)
    {
        return false;
    }
    if (readable.minutes > 59)
    {
        return false;
    }
    if (readable.seconds > 59)
    {
        return false;
    }
    return true;
}

void time_custom::make_readable_valid(time_custom::readable &readable)
{
    if (readable.month < 1)
    {
        readable.month = 1;
    }
    else if (readable.month > 12)
    {
        readable.month = 12;
    }
    if (readable.month_day < 1)
    {
        readable.month_day = 1;
    }
    else if (readable.month_day
             > days_of_each_month[readable.month - 1]
                   + ((readable.month == 2
                       && ((readable.year % 4 == 0 && readable.year % 100 != 0)
                           || readable.year % 400 == 0))
                          ? 1
                          : 0))
    {
        readable.month_day
            = days_of_each_month[readable.month - 1]
              + ((readable.month == 2
                  && ((readable.year % 4 == 0 && readable.year % 100 != 0)
                      || readable.year % 400 == 0))
                     ? 1
                     : 0);
    }
    if (readable.week_day < 1)
    {
        readable.week_day = 1;
    }
    else if (readable.week_day > 7)
    {
        readable.week_day = 7;
    }
    if (readable.hour > 23)
    {
        readable.hour = 23;
    }
    if (readable.minutes > 59)
    {
        readable.minutes = 59;
    }
    if (readable.seconds > 59)
    {
        readable.seconds = 59;
    }
}

uint8_t time_custom::calculate_week_day(
    const time_custom::unix_timestamp &unix_timestamp)
{
    // 2000-01-01 is saturday
    return (unix_timestamp.second / (60 * 60 * 24) + 5) % 7 + 1;
}

void time_unix_timestamp_add_millisecond(void *time, int32_t millisecond)
{
    static_cast<time_custom::time *>(time)->unix_timestamp_add_millisecond(
        millisecond);
}

time_custom_readable time_get_readable(void *time)
{
    return static_cast<time_custom::time *>(time)->readable;
}

time_custom_unix_timestamp time_get_unix_timestamp(void *time)
{
    return static_cast<time_custom::time *>(time)->unix_timestamp;
}

void time_set_readable(void *time, time_custom_readable readable)
{
    *static_cast<time_custom::time *>(time) = readable;
}

void time_set_unix_timestamp(void *time,
                             time_custom_unix_timestamp unix_timestamp)
{
    *static_cast<time_custom::time *>(time) = unix_timestamp;
}

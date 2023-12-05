/**
 ******************************************************************************
 * @file    tick.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide tick tool.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "tick.h"
#include "bsp_DS3231.h"
#include "bsp_rtc.h"

tick::controller tick_controller_entity{
    std::array<tick::time_source, tick::controller::time_source_amount>{
        tick::time_source{
            .type = tick_time_source_type::readable,
            .function
            = {.readable = {
                   .get =
                       []() {
                           ds3231_date_time_type ds3231_date;
                           ds3231_get_date_time(&ds3231_date);
                           return time_custom_readable{
                               .millisecond = 0,
                               .seconds = ds3231_date.seconds,
                               .minutes = ds3231_date.minutes,
                               .hour = ds3231_date.hour,
                               .month_day = ds3231_date.date,
                               .month = ds3231_date.month,
                               .year = ds3231_date.year,
                           };
                       },
                   .set =
                       [](time_custom::readable readable) {
                           ds3231_set_date_time(
                               reinterpret_cast<ds3231_date_time_type *>(
                                   &(readable.seconds)));
                       },

               }},
            .is_valid = []() -> bool { return true; },
            .is_resumable = []() -> bool {
                ds3231_date_time_type ds3231_date;
                ds3231_get_date_time(&ds3231_date);
                return !(ds3231_date.year == 0);
            },
        },
        tick::time_source{
            .type = tick_time_source_type::unix_timestamp,
            .function
            = {.unix_timestamp = {
                   .get =
                       []() {
                           return time_custom_unix_timestamp{
                               .second = RTC_get_unix_timestamp(),
                               .millisecond = 0,
                           };
                       },
                   .set =
                       [](time_custom::unix_timestamp unix_timestamp) {
                           RTC_set_unix_timestamp(unix_timestamp.second);
                       },
               }},
            .is_valid = []() -> bool { return true; },
            .is_resumable
            = []() -> bool { return RTC_get_unix_timestamp() >= 946656000; },
        },
    }};

void tick::controller::run(void)
{
    disable_irq();
    uint32_t milliseconds_from_last_updata_temp = milliseconds_from_last_updata;
    milliseconds_from_last_updata = 0;
    enable_irq();
    time_now.unix_timestamp_add_millisecond(milliseconds_from_last_updata_temp);
}

uint32_t tick::controller::resume(void)
{
    for (uint32_t i = 0; i < time_source_amount; i++)
    {
        if (time_source[i].is_resumable() && time_source[i].is_valid() == true)
        {
            if (time_source[i].type == time_source_type::readable)
            {
                time_now.set_readable(time_source[i].function.readable.get());
            }
            else if (time_source[i].type == time_source_type::unix_timestamp)
            {
                time_now.set_unix_timestamp(
                    time_source[i].function.unix_timestamp.get());
            }
            return i;
        }
    }
    time_now.set_readable(default_time);
    return 0xFFFFFFFFU;
}

void tick::controller::sync(uint32_t exclude_index)
{
    for (uint32_t i = 0; i < time_source_amount; i++)
    {
        if (time_source[i].is_valid() == true && i != exclude_index)
        {
            if (time_source[i].type == time_source_type::readable)
            {
                time_source[i].function.readable.set(time_now.get_readable());
            }
            else if (time_source[i].type == time_source_type::unix_timestamp)
            {
                time_source[i].function.unix_timestamp.set(
                    time_now.get_unix_timestamp());
            }
        }
    }
}

void tick_inc(void *controller, uint32_t milliseconds)
{
    static_cast<tick::controller *>(controller)->inc(milliseconds);
}

void tick_set_unix_timestamp(void *controller, time_custom_unix_timestamp tick)
{
    static_cast<tick::controller *>(controller)->get_time_now().set_unix_timestamp(tick);
}
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

#ifndef __TIME_COSTOM_H
#define __TIME_COSTOM_H

#ifdef __cplusplus
    #include <array>
    #include <cstddef>
    #include <cstdint>
#else
    #include <stddef.h>
    #include <stdint.h>
#endif

#include "utils.h"
#include "compiler.h"

#ifdef __cplusplus
namespace time_custom {
namespace impl {
#endif

struct time_custom_readable {
    // From 0 to 999
    uint16_t millisecond;
    // From 0 to 59
    uint8_t seconds;
    // From 0 to 59
    uint8_t minutes;
    // From 0 to 23
    uint8_t hour;
    // From 0 to 6
    uint8_t week_day;
    // From 1 to 31
    uint8_t month_day;
    // From 1 to 12
    uint8_t month;
    // From 2000 to now (0 - 99)
    uint8_t year;
};

struct time_custom_unix_timestamp {
    uint32_t second;
    uint16_t millisecond;
};

#ifdef __cplusplus
} // namespace impl
} // namespace time_custom

namespace time_custom {

using readable = impl::time_custom_readable;
using unix_timestamp = impl::time_custom_unix_timestamp;
static constexpr std::array<uint8_t, 12> days_of_each_month
    = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static constexpr uint32_t unix_timestamp_of_2000_01_01_00_00 = 946656000;

time_custom::unix_timestamp
readabel_convert_to_unix_timestamp(const time_custom::readable &readable);
time_custom::readable unix_timestamp_convert_to_readable(
    const time_custom::unix_timestamp &unix_timestamp);
bool is_readable_valid(const time_custom::readable &readable);
void make_readable_valid(time_custom::readable &readable);
uint8_t calculate_week_day(const time_custom::unix_timestamp &unix_timestamp);

struct time {
  public:
    time_custom::readable readable;
    time_custom::unix_timestamp unix_timestamp;

    time(void) = default;
    time(time_custom::readable readable)
        : readable(readable),
          unix_timestamp(readabel_convert_to_unix_timestamp(readable))
    {
        readable.week_day = calculate_week_day(unix_timestamp);
    }
    time(time_custom::unix_timestamp unix_timestamp)
        : unix_timestamp(unix_timestamp),
          readable(unix_timestamp_convert_to_readable(unix_timestamp))
    {
    }
    time(time_custom::readable readable,
         time_custom::unix_timestamp unix_timestamp)
        : readable(readable), unix_timestamp(unix_timestamp)
    {
    }
    void readable_sync_unix_timestamp(void)
    {
        unix_timestamp = readabel_convert_to_unix_timestamp(readable);
    }
    void unix_timestamp_sync_readable(void)
    {
        readable = unix_timestamp_convert_to_readable(unix_timestamp);
    }
    void unix_timestamp_add_second(int32_t second)
    {
        unix_timestamp.second += second;
        unix_timestamp_sync_readable();
    }
    void unix_timestamp_add_millisecond(int32_t millisecond)
    {
        unix_timestamp.second += (millisecond + unix_timestamp.millisecond) / 1000;
        unix_timestamp.millisecond = (millisecond + unix_timestamp.millisecond) % 1000;
        unix_timestamp_sync_readable();
    }
    void readable_change_millisecond(int32_t volume)
    {
        readable.millisecond = utils::number_change_with_limitation(
            readable.millisecond, volume, 0, 999);
        readable_sync_unix_timestamp();
    }
    void readable_change_seconds(int32_t volume)
    {
        readable.seconds = utils::number_change_with_limitation(
            readable.seconds, volume, 0, 59);
        readable_sync_unix_timestamp();
    }
    void readable_change_minutes(int32_t volume)
    {
        readable.minutes = utils::number_change_with_limitation(
            readable.minutes, volume, 0, 59);
        readable_sync_unix_timestamp();
    }
    void readable_change_hour(int32_t volume)
    {
        readable.hour = utils::number_change_with_limitation(readable.hour,
                                                             volume, 0, 23);
        readable_sync_unix_timestamp();
    }
    void readable_change_month_day(int32_t volume)
    {
        readable.month_day = utils::number_change_with_limitation(
            readable.month_day, volume, 1,
            days_of_each_month[readable.month - 1]
                + ((readable.month == 2
                    && ((readable.year % 4 == 0 && readable.year % 100 != 0)
                        || readable.year % 400 == 0))
                       ? 1
                       : 0));
        readable_sync_unix_timestamp();
        readable.week_day = calculate_week_day(unix_timestamp);
    }
    void readable_change_month(int32_t volume)
    {
        readable.month = utils::number_change_with_limitation(readable.month,
                                                              volume, 1, 12);
        if (is_readable_valid(readable) == false)
        {
            make_readable_valid(readable);
        }
        readable_sync_unix_timestamp();
        readable.week_day = calculate_week_day(unix_timestamp);
    }
    void readable_change_year(int32_t volume)
    {
        readable.year = utils::number_change_with_limitation(readable.year,
                                                             volume, 0, 99);
        if (is_readable_valid(readable) == false)
        {
            make_readable_valid(readable);
        }
        readable_sync_unix_timestamp();
        readable.week_day = calculate_week_day(unix_timestamp);
    }
    void set_readable(const time_custom::readable readable)
    {
        this->readable = readable;
        readable_sync_unix_timestamp();
        this->readable.week_day = time_custom::calculate_week_day(unix_timestamp);
    }
    void set_unix_timestamp(const time_custom::unix_timestamp unix_timestamp)
    {
        this->unix_timestamp = unix_timestamp;
        unix_timestamp_sync_readable();
    }
    time_custom::readable get_readable(void) const
    {
        return readable;
    }
    time_custom::unix_timestamp get_unix_timestamp(void) const
    {
        return unix_timestamp;
    }
};

}; // namespace time_custom

#endif

#ifdef __cplusplus
extern time_custom::time time_now;
using time_custom_readable = time_custom::readable;
using time_custom_unix_timestamp = time_custom::unix_timestamp;
#else
extern void time_now;
typedef struct time_custom_readable time_custom_readable;
typedef struct time_custom_unix_timestamp time_custom_unix_timestamp;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void time_unix_timestamp_add_millisecond(void *time, int32_t millisecond);
time_custom_readable time_get_readable(void *time);
time_custom_unix_timestamp time_get_unix_timestamp(void *time);
void time_set_readable(void *time, time_custom_readable readable);
void time_set_unix_timestamp(void *time, time_custom_unix_timestamp unix_timestamp);

#ifdef __cplusplus
}
#endif

#endif /* __TIME_COSTOM_H */

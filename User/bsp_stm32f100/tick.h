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

#ifndef __TICK_H
#define __TICK_H

#ifdef __cplusplus
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "time_custom.h"

#ifdef __cplusplus
namespace tick {
namespace time_source_type_impl {
#endif

enum tick_time_source_type
{
    TIME_SOURCE_TYPE_UNIX_TIMESTAMP,
    TIME_SOURCE_TYPE_READABLE,
};

#ifdef __cplusplus
} // namespace time_source_type_impl
} // namespace tick

namespace tick {
enum class time_source_type : uint8_t
{
    unix_timestamp = time_source_type_impl::TIME_SOURCE_TYPE_UNIX_TIMESTAMP,
    readable = time_source_type_impl::TIME_SOURCE_TYPE_READABLE,
};
} // namespace tick
#endif

#ifdef __cplusplus
using tick_time_source_type = tick::time_source_type;
#else
typedef uint8_t tick_time_source_type;
#endif

#ifdef __cplusplus
namespace tick {
namespace impl {
#endif

struct tick_time_source_unix_timestamp
{
    time_custom_unix_timestamp (*get)(void);
    void (*set)(time_custom_unix_timestamp second);
};

struct tick_time_source_readable
{
    time_custom_readable (*get)(void);
    void (*set)(const time_custom_readable tick);
};

union tick_time_source_function
{
    struct tick_time_source_unix_timestamp unix_timestamp;
    struct tick_time_source_readable readable;
};

struct tick_time_source
{
    tick_time_source_type type;
    union tick_time_source_function function;
    bool (*is_valid)(void);
    bool (*is_resumable)(void);
};

#ifdef __cplusplus
} // namespace impl
} // namespace tick

namespace tick {

using time_source = impl::tick_time_source;

struct controller
{
    constexpr static uint32_t time_source_amount = 1;
    constexpr static auto default_time = time_custom::readable{
        .millisecond = 0,
        .seconds = 0,
        .minutes = 0,
        .hour = 0,
        .month_day = 27,
        .month = 9,
        .year = 23,
    };
    std::array<tick::time_source, time_source_amount> time_source;
    time_custom::time time_now;
    volatile uint32_t milliseconds_from_last_updata = 0;

    controller(void) = delete;
    controller(std::array<tick::time_source, time_source_amount> time_source)
        : time_source(time_source){};
    time_custom::time &get_time_now(void)
    {
        return time_now;
    };
    void inc(uint32_t milliseconds)
    {
        this->milliseconds_from_last_updata += milliseconds;
    };
    void resume(void);
    void sync(void);
    void run(void);
};

} // namespace tick

#endif

#ifdef __cplusplus
extern tick::controller _tick_controller;
#else
extern void _tick_controller;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void tick_inc(void *controller, uint32_t milliseconds);

#ifdef __cplusplus
}
#endif

#endif /* __TICK_H */

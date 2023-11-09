/**
 ******************************************************************************
 * @file    display.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide display config.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __DISPLAY_H
#define __DISPLAY_H

#ifdef __cplusplus
    #include <cstddef>
    #include <cstdint>
#else
    #include <stddef.h>
    #include <stdint.h>
#endif

#include "time_custom.h"

#ifdef __cplusplus
namespace display {
namespace impl {
#endif

enum display_mode {
    DISPLAY_MODE_CLOCK,
    DISPLAY_MODE_CLOCK_SETTING,
    DISPLAY_MODE_NONE,
    DISPLAY_MODE_AMOUNT,
};

enum display_content {
    DISPLAY_CONTENT_YEAR,
    DISPLAY_CONTENT_MONTH_DATE,
    DISPLAY_CONTENT_HOUR_MINUTES,
    DISPLAY_CONTENT_AMOUNT,
};

#ifdef __cplusplus
} // namespace impl
} // namespace display

namespace display {
enum class mode {
    clock = impl::DISPLAY_MODE_CLOCK,
    clock_setting = impl::DISPLAY_MODE_CLOCK_SETTING,
    none = impl::DISPLAY_MODE_NONE,
    amount = impl::DISPLAY_MODE_AMOUNT,
};

enum class content {
    year = impl::DISPLAY_CONTENT_YEAR,
    month_date = impl::DISPLAY_CONTENT_MONTH_DATE,
    hour_minutes = impl::DISPLAY_CONTENT_HOUR_MINUTES,
    amount = impl::DISPLAY_CONTENT_AMOUNT,
};

struct controller {
    display::mode mode = display::mode::clock;
    display::content content = display::content::hour_minutes;
    time_custom::readable *time_display_source = nullptr;

    controller(void) = delete;
    controller(time_custom::readable *time_display_source)
        : time_display_source(time_display_source){};
    controller(display::mode mode, display::content content,
               time_custom::readable *time_display_source)
        : mode(mode), content(content),
          time_display_source(time_display_source){};
    void run(void);
    void display_clock(void);
};
}; // namespace display

#endif

#ifdef __cplusplus
using display_mode = display::mode;
using display_content = display::content;
using display_controller = display::controller;
extern display::controller _display_controller;
#else
typedef enum display_mode display_mode;
typedef enum display_content display_content;
typedef struct display_controller display_controller;
extern display_controller _display_controller;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __DISPLAY_H */

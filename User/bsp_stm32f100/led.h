/**
 ******************************************************************************
 * @file    led.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-03
 * @brief   controller for led.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __LED_H
#define __LED_H

#include "color.h"
#include "utils.h"

#ifdef __cplusplus
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "utils.h"

#ifdef __cplusplus
namespace led {
namespace impl {
#endif

#ifdef __cplusplus
using led_rgb = color::rgb;
using led_hsv = color::hsv;
#else
typedef color_rgb led_rgb;
typedef color_hsv led_hsv;
#endif

struct led_always_on_config
{
    led_hsv color;
};

struct led_twinkle_config
{
    led_hsv color_1;
    uint16_t color_1_time;
    led_hsv color_2;
    uint16_t color_2_time;
    uint16_t tick;
    utils_twinkle_state state;
};

struct led_breath_direction
{
    utils_breath_direction h;
    utils_breath_direction s;
    utils_breath_direction v;
};

struct led_breath_config
{
    led_hsv color_init;
    struct led_breath_direction direction;
    led_hsv color_1;
    led_hsv color_2;
    led_hsv step;
};

union led_config
{
    struct led_always_on_config always_on;
    struct led_twinkle_config twinkle;
    struct led_breath_config breath;
};

enum led_style
{
    LED_STYLE_DISABLE,
    LED_STYLE_ALWAYS_ON,
    LED_STYLE_TWINKLE,
    LED_STYLE_BREATH,
};

#ifdef __cplusplus
} // namespace impl
} // namespace led

namespace led {

enum class type
{
    disable = impl::LED_STYLE_DISABLE,
    always_on = impl::LED_STYLE_ALWAYS_ON,
    twinkle = impl::LED_STYLE_TWINKLE,
    breath = impl::LED_STYLE_BREATH,
};

using hsv = impl::led_hsv;
using rgb = impl::led_rgb;
using config = impl::led_config;
using always_on_config = impl::led_always_on_config;
using twinkle_config = impl::led_twinkle_config;
using breath_config = impl::led_breath_config;

struct style
{
    type type;
    config config;
};

struct driver
{
    friend struct controller;

  public:
    static constexpr uint16_t amount = 4U;

  private:
    std::array<hsv, led::driver::amount> color;
    std::array<rgb, led::driver::amount> color_rgb;
    void (*control)(rgb *rgb, uint16_t amount) = nullptr;
    bool is_update = false;

  public:
    driver(void (*control)(rgb *rgb, uint16_t amount)) : control(control) {}
    driver(std::array<hsv, led::driver::amount> color,
           void (*control)(rgb *rgb, uint16_t amount))
        : color(color), control(control)
    {}
    void check_is_update(std::array<hsv, led::driver::amount> color);
    void run(uint32_t tick_now);
    void set_color(std::array<hsv, led::driver::amount> color)
    {
        check_is_update(color);
        if (this->is_update)
        {
            this->color = color;
        }
    }
};

struct controller
{
  public:
    static constexpr led::style default_style{
        .type = led::type::disable,
        .config = {},
    };

  private:
    led::driver driver;
    uint32_t cycle_start_tick = 0;
    std::array<led::style, led::driver::amount> style;
    std::array<hsv, led::driver::amount> color;
    uint16_t update_period = 100U;
    void isUpdateNeeded(void);
    void init(void)
    {
        this->color.fill(hsv{
            .h = 0,
            .s = 0,
            .v = 0,
        });
    }
    void twinkle_run(led::style &style, hsv &color);
    void breath_run(led::style &style, hsv &color);

  public:
    controller(void) = delete;
    controller(led::driver driver, uint16_t update_period = 100U)
        : driver(driver)
    {
        init();
        set_style(default_style, update_period);
    };
    controller(led::driver driver,
               std::array<led::style, led::driver::amount> style,
               uint16_t update_period = 100U)
        : driver(driver), style(style), update_period(update_period)
    {
        init();
    };
    controller(led::driver driver, led::style style,
               uint16_t update_period = 100U)
        : driver(driver)
    {
        init();
        set_style(style, update_period);
    };
    void run(uint32_t tick_now);
    void set_style(const std::array<led::style, led::driver::amount> &style,
                   uint16_t update_period = 100U)
    {
        this->style = style;
        this->update_period = update_period;
        this->cycle_start_tick = 0;
    }
    void set_style(const led::style &style = default_style,
                   uint16_t update_period = 100U)
    {
        this->style.fill(style);
        this->update_period = update_period;
        this->cycle_start_tick = 0;
    };
    std::array<led::style, led::driver::amount> get_style(void) const
    {
        return style;
    }
};

namespace preset {
static constexpr style breath{.type = led::type::breath,
                .config = {.breath = {.color_init = {.h = 15, .s = 255, .v = 0},
                                      .direction = {.h = utils::breath_direction::up,
                                                    .s = utils::breath_direction::up,
                                                    .v = utils::breath_direction::up,},
                                      .color_1 = {.h = 15, .s = 255, .v = 0},
                                      .color_2 = {.h = 15, .s = 255, .v = 200},
                                      .step = {.h = 0, .s = 0, .v = 1}}}};

static constexpr style breath_purple{.type = led::type::breath,
                .config = {.breath = {.color_init = {.h = 278, .s = 255, .v = 0},
                                      .direction = {.h = utils::breath_direction::up,
                                                    .s = utils::breath_direction::up,
                                                    .v = utils::breath_direction::up,},
                                      .color_1 = {.h = 278, .s = 255, .v = 0},
                                      .color_2 = {.h = 278, .s = 255, .v = 200},
                                      .step = {.h = 0, .s = 0, .v = 1}}}};

static constexpr style twinkle{
    .type = led::type::twinkle,
    .config = {.twinkle = {.color_1 = {15, 255, 150},
                           .color_1_time = 50,
                           .color_2 = {0, 0, 0},
                           .color_2_time = 50,
                           .tick = 0,
                           .state = utils::twinkle_state::off}}};

static constexpr style always_on{.type = led::type::always_on,
                                 .config
                                 = {.always_on = {.color = {15, 255, 150}}}};

static constexpr style off{.type = led::type::always_on,
                           .config = {.always_on = {.color = {0, 0, 0}}}};

} // namespace preset

} // namespace led

#endif

#ifdef __cplusplus
using led_controller = led::controller;
using led_style = led::style;
using led_config = led::config;
using led_always_on_config = led::always_on_config;
using led_twinkle_config = led::twinkle_config;
using led_breath_config = led::breath_config;
using led_hsv = led::hsv;
using led_rgb = led::rgb;
extern led::controller led_controller_entity;
#else
typedef enum led_control_state led_control_state;
typedef enum led_style led_style;
typedef union led_config led_config;
typedef struct led_always_on_config led_always_on_config;
typedef struct led_twinkle_config led_twinkle_config;
typedef struct led_breath_config led_breath_config;
typedef struct led_driver led_driver;
typedef struct led_controller led_controller;
extern void led_controller_entity;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */

/**
 ******************************************************************************
 * @file    colon.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-08-30
 * @brief   controller for colon.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __COLON_H
#define __COLON_H

#include "utils.h"

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace colon {
namespace impl {
#endif

struct colon_always_on_config
{
    uint8_t brightness;
};

struct colon_twinkle_config
{
    uint8_t on_brightness;
    uint16_t on_time;
    uint8_t off_brightness;
    uint16_t off_time;
    uint16_t tick;
    utils_twinkle_state state;
};

struct colon_breath_config
{
    uint8_t brightness_init;
    utils_breath_direction direction;
    uint8_t max_brightness;
    uint8_t min_brightness;
    uint8_t step;
};

union colon_config
{
    struct colon_always_on_config always_on;
    struct colon_twinkle_config twinkle;
    struct colon_breath_config breath;
};

enum colon_control_state
{
    COLON_CONTROL_STATE_ON,
    COLON_CONTROL_STATE_OFF,
};

enum colon_type
{
    COLON_STYLE_DISABLE = 0x00U,
    COLON_STYLE_ALWAYS_ON,
    COLON_STYLE_TWINKLE,
    COLON_STYLE_BREATH,
};

#ifdef __cplusplus
} // namespace impl
} // namespace colon

namespace colon {
enum class type : uint8_t
{
    disable = impl::COLON_STYLE_DISABLE,
    always_on = impl::COLON_STYLE_ALWAYS_ON,
    twinkle = impl::COLON_STYLE_TWINKLE,
    breath = impl::COLON_STYLE_BREATH,
};

enum class control_state
{
    on = impl::COLON_CONTROL_STATE_ON,
    off = impl::COLON_CONTROL_STATE_OFF,
};

using config = impl::colon_config;
using always_on_config = impl::colon_always_on_config;
using twinkle_config = impl::colon_twinkle_config;
using breath_config = impl::colon_breath_config;

struct style
{
    type type;
    config config;
};

struct driver
{
    friend struct controller;

  public:
    static constexpr uint16_t pwm_cycle = 100U;
    static constexpr uint16_t max_brightness = pwm_cycle;

  private:
    uint16_t brightness = max_brightness;
    void (*control)(colon::control_state state) = nullptr;

  public:
    driver(void) = delete;
    driver(void (*control)(colon::control_state state)) : control(control){};
    driver(uint16_t brightness, void (*control)(colon::control_state state))
        : brightness(brightness), control(control){};
    void run(uint32_t tick_now);
    void set_brightness(uint16_t brightness)
    {
        this->brightness = brightness;
    };
};

struct controller
{
  private:
    colon::style style
        = {.type = colon::type::disable, .config = colon::config{}};
    uint16_t brightness = colon::driver::max_brightness;
    uint16_t update_period = 100U;

  public:
    colon::driver driver;
    controller(void) = delete;
    controller(colon::driver driver) : driver(driver){};
    controller(colon::driver driver, colon::style style, uint16_t update_period = 100U)
        : driver(driver), style(style), update_period(update_period){};
    void run(uint32_t tick_now);
    void twinkle_run(void);
    void always_on_run(void);
    void breath_run(void);
    void set_style(colon::style style, uint16_t update_period = 100U);
};

namespace preset {
static constexpr style breath {
    .type = type::breath,
    .config = config{.breath = {.direction = utils::breath_direction::up,
                                .max_brightness = driver::max_brightness,
                                .min_brightness = 0,
                                .step = 1,}},
};

static constexpr style twinkle{
    .type = type::twinkle,
    .config = config{.twinkle = {.on_brightness = driver::max_brightness,
                                 .on_time = 100,
                                 .off_brightness = 0,
                                 .off_time = 100,
                                 .tick = 0,
                                 .state = utils::twinkle_state::off}},
};

static constexpr style always_on{
    .type = type::always_on,
    .config = config{.always_on = {.brightness = driver::max_brightness}},
};

static constexpr style off{
    .type = type::always_on,
    .config = config{.always_on = {.brightness = 0}},
};

static constexpr style disable{
    .type = type::disable,
    .config = config{},
};
} // namespace preset
} // namespace colon

#endif

#ifdef __cplusplus
using colon_control_state = colon::control_state;
using colon_controller = colon::controller;
using colon_style = colon::style;
using colon_type = colon::type;
using colon_config = colon::config;
using colon_always_on_config = colon::always_on_config;
using colon_twinkle_config = colon::twinkle_config;
using colon_breath_config = colon::breath_config;
using colon_driver = colon::driver;
using colon_controller = colon::controller;
extern colon::controller _colon_controler;
#else
typedef enum colon_control_state colon_control_state;
typedef uint8_t colon_type;
typedef union colon_config colon_config;
typedef struct colon_always_on_config colon_always_on_config;
typedef struct colon_twinkle_config colon_twinkle_config;
typedef struct colon_breath_config colon_breath_config;
typedef struct colon_driver colon_driver;
typedef struct colon_controller colon_controller;
extern colon_controller _colon_controler;
#endif

#ifdef __cplusplus
extern "C" {
#endif

colon_controller *
conlon_controler_create(void (*control)(colon_control_state state),
                        colon_type type, colon_config config);
void conlon_controler_destroy(colon_controller *controller);
void colon_controler_run(colon_controller *controller, uint32_t tick_now);

#ifdef __cplusplus
}
#endif

#endif /* __COLON_H */

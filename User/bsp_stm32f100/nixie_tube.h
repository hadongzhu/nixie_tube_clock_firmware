/**
 ******************************************************************************
 * @file    nixie_tube.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-13
 * @brief   controller for nixie tube.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __NIXIE_TUBE_H
#define __NIXIE_TUBE_H

#include "utils.h"

#ifdef __cplusplus
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
namespace nixie_tube {
namespace impl {
#endif

#define NIXIE_TUBE_AMOUNT 4U

enum nixie_tube_display_style_type
{
    NIXIE_TUBE_DISPLAY_STYLE_DISABLE,
    NIXIE_TUBE_DISPLAY_STYLE_ALWAYS_ON,
    NIXIE_TUBE_DISPLAY_STYLE_TWINKLE,
    NIXIE_TUBE_DISPLAY_STYLE_BREATH,
    NIXIE_TUBE_DISPLAY_STYLE_GLITCH,
};

enum nixie_tube_change_style_type
{
    NIXIE_TUBE_CHANGE_STYLE_DISABLE,
    NIXIE_TUBE_CHANGE_STYLE_BREATH,
    NIXIE_TUBE_CHANGE_STYLE_BREATH_MEANTIME,
    NIXIE_TUBE_CHANGE_STYLE_JUMP,
};

struct nixie_tube_display_style_always_on_config
{
    uint8_t brightness;
};

struct nixie_tube_display_style_twinkle_config
{
    uint8_t on_brightness;
    uint16_t on_time;
    uint8_t off_brightness;
    uint16_t off_time;
    uint16_t tick;
    utils_twinkle_state state;
};

struct nixie_tube_display_style_breath_config
{
    uint8_t brightness_init;
    utils_breath_direction direction;
    uint8_t max_brightness;
    uint8_t min_brightness;
    uint8_t step;
};

union nixie_tube_display_style_config
{
    struct nixie_tube_display_style_always_on_config always_on;
    struct nixie_tube_display_style_twinkle_config twinkle;
    struct nixie_tube_display_style_breath_config breath;
};

struct nixie_tube_change_style_breath_config
{
    uint8_t step;
};

struct nixie_tube_change_style_breath_meantime_config
{
    uint8_t step;
};

struct nixie_tube_change_style_jump_config
{
    uint16_t time;
    uint16_t speed;
};

union nixie_tube_change_style_config
{
    struct nixie_tube_change_style_breath_config breath;
    struct nixie_tube_change_style_breath_meantime_config breath_meantime;
    struct nixie_tube_change_style_jump_config jump;
};

#ifdef __cplusplus
} // namespace impl
} // namespace nixie_tube

namespace nixie_tube {
namespace display {

enum class type : uint8_t
{
    disable = impl::NIXIE_TUBE_DISPLAY_STYLE_DISABLE,
    always_on = impl::NIXIE_TUBE_DISPLAY_STYLE_ALWAYS_ON,
    twinkle = impl::NIXIE_TUBE_DISPLAY_STYLE_TWINKLE,
    breath = impl::NIXIE_TUBE_CHANGE_STYLE_BREATH,
    glitch = impl::NIXIE_TUBE_DISPLAY_STYLE_GLITCH,
};

using config = impl::nixie_tube_display_style_config;
using always_on_config = impl::nixie_tube_display_style_always_on_config;
using twinkle_config = impl::nixie_tube_display_style_twinkle_config;
using breath_config = impl::nixie_tube_display_style_breath_config;

struct style
{
    type type;
    config config;
};

} // namespace display

namespace change {

enum class type
{
    disable = impl::NIXIE_TUBE_CHANGE_STYLE_DISABLE,
    breath = impl::NIXIE_TUBE_CHANGE_STYLE_BREATH,
    breath_meantime = impl::NIXIE_TUBE_CHANGE_STYLE_BREATH_MEANTIME,
    jump = impl::NIXIE_TUBE_CHANGE_STYLE_JUMP,
};

using config = impl::nixie_tube_change_style_config;
using breath_config = impl::nixie_tube_change_style_breath_config;
using breath_meantime_config
    = impl::nixie_tube_change_style_breath_meantime_config;
using jump_config = impl::nixie_tube_change_style_jump_config;

struct style
{
    type type;
    config config;
};

} // namespace change

struct driver
{
    friend struct controller;

  public:
    static constexpr uint16_t amount = NIXIE_TUBE_AMOUNT;
    static constexpr uint16_t pwm_cycle = 100U;
    static constexpr uint16_t max_brightness = pwm_cycle;

    static constexpr uint8_t number_comma = 10;
    static constexpr uint8_t number_none = 11;

    struct config
    {
        uint8_t number_primary = number_none;
        uint8_t brightness = nixie_tube::driver::max_brightness;
        uint8_t number_secondary = number_none;
        uint8_t secondary_brightness = 0;
    };

  private:
    uint32_t cycle_start_tick = 0;
    std::array<uint8_t, nixie_tube::driver::amount> old_number
        = {number_none, number_none, number_none, number_none};
    std::array<uint8_t, nixie_tube::driver::amount> new_number
        = {number_none, number_none, number_none, number_none};
    std::array<config, nixie_tube::driver::amount> _config;
    void (*control)(uint8_t *number, uint16_t amount);

  public:
    driver(void) = delete;
    driver(void (*control)(uint8_t *number, uint16_t amount))
        : control(control){};
    driver(void (*control)(uint8_t *number, uint16_t amount),
           std::array<nixie_tube::driver::config, nixie_tube::driver::amount>
               config)
        : control(control), _config(config){};
    void run(uint32_t tick_now);
    void set_config(
        std::array<nixie_tube::driver::config, nixie_tube::driver::amount>
            config)
    {
        this->_config = config;
    };
};

struct number
{
    uint8_t last_one;
    uint8_t new_one;
};

struct controller
{
  public:
    enum class status
    {
        display,
        change,
    };

    static constexpr display::style default_display_style
        = display::style{.type = display::type::disable, .config = {}};
    static constexpr change::style default_change_style
        = change::style{.type = change::type::disable, .config = {}};

  private:
    uint32_t cycle_start_tick = 0;
    std::array<display::style, nixie_tube::driver::amount> display_style;
    std::array<change::style, nixie_tube::driver::amount> change_style;
    std::array<nixie_tube::number, nixie_tube::driver::amount> number;
    std::array<driver::config, nixie_tube::driver::amount> &driver_config
        = driver._config;
    uint16_t update_period = 100U;
    std::array<controller::status, nixie_tube::driver::amount> _status;

    void init(void)
    {
        this->number.fill(nixie_tube::number{.last_one = driver::number_none,
                                             .new_one = driver::number_none});
        this->_status.fill(controller::status::display);
    }

  public:
    nixie_tube::driver driver;
    controller(void) = delete;
    controller(nixie_tube::driver driver, uint16_t update_period = 100U)
        : driver(driver)
    {
        init();
        set_style(default_display_style, default_change_style, update_period);
    };
    controller(
        nixie_tube::driver driver,
        std::array<display::style, nixie_tube::driver::amount> display_style,
        std::array<change::style, nixie_tube::driver::amount> change_style,
        uint16_t update_period = 100U)
        : driver(driver), display_style(display_style),
          change_style(change_style), update_period(update_period)
    {
        init();
    };
    controller(nixie_tube::driver driver, display::style display_style,
               change::style change_style, uint16_t update_period = 100U)
        : driver(driver)
    {
        init();
        set_style(display_style, change_style, update_period);
    };
    void run(uint32_t tick_now);
    void set_style(const std::array<display::style, nixie_tube::driver::amount>
                       &display_style,
                   const std::array<change::style, nixie_tube::driver::amount>
                       &change_style,
                   uint16_t update_period = 100U)
    {
        this->display_style = display_style;
        this->change_style = change_style;
        this->update_period = update_period;
    };
    void set_style(const display::style &display_style = default_display_style,
                   const change::style &change_style = default_change_style,
                   uint16_t update_period = 100U)
    {
        this->display_style.fill(display_style);
        this->change_style.fill(change_style);
        this->update_period = update_period;
    };
    void set_number(
        const std::array<uint8_t, nixie_tube::driver::amount> &new_number);
    nixie_tube::driver::config
    display_twinkle(nixie_tube::display::style &display,
                    const nixie_tube::number &number);
    nixie_tube::controller::status
    change_breath(const nixie_tube::change::style &change,
                  const nixie_tube::display::style &display,
                  const nixie_tube::number &number,
                  driver::config &driver_config);
    std::array<display::style, nixie_tube::driver::amount> &
    get_display_style(void)
    {
        return this->display_style;
    };
    std::array<change::style, nixie_tube::driver::amount> &
    get_change_style(void)
    {
        return this->change_style;
    };
};

namespace preset {
namespace display {
static constexpr nixie_tube::display::style always_on{
    .type = nixie_tube::display::type::always_on,
    .config
    = {.always_on = {.brightness = nixie_tube::driver::max_brightness}}};
} // namespace display

namespace change {
static constexpr nixie_tube::change::style breath{.type = nixie_tube::change::type::breath,
                                 .config = {.breath = {.step = 4}}};
} // namespace change

} // namespace preset

} // namespace nixie_tube

#endif

#ifdef __cplusplus
using nixie_tube_display_style_type = nixie_tube::display::type;
using nixie_tube_display_style_config = nixie_tube::display::config;
using nixie_tube_change_style_type = nixie_tube::change::type;
using nixie_tube_change_style_config = nixie_tube::change::config;
using nixie_tube_driver = nixie_tube::driver;
using nixie_tube_controller = nixie_tube::controller;
extern nixie_tube::controller _nixie_tube_controller;
#else
typedef uint8_t nixie_tube_display_style_type;
typedef union nixie_tube_display_style_config nixie_tube_display_style_config;
typedef uint8_t nixie_tube_change_style_type;
typedef union nixie_tube_change_style_config nixie_tube_change_style_config;
typedef struct nixie_tube_driver nixie_tube_driver;
typedef struct nixie_tube_controller nixie_tube_controller;
extern nixie_tube_controller _nixie_tube_controller;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void nixie_tube_controller_set_number(nixie_tube_controller *controller,
                                      uint8_t *number, uint16_t amount);
void nixie_tube_controller_set_display_style(
    nixie_tube_controller *controller, uint16_t index,
    nixie_tube_display_style_type type, nixie_tube_display_style_config config);

#ifdef __cplusplus
}
#endif

#endif /* __NIXIE_TUBE_H */

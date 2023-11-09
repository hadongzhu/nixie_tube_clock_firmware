/**
 ******************************************************************************
 * @file    colon.cpp
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

#include "colon.h"
#include "bsp_neon_bulb.h"
#include "theme_pack.h"

colon::controller _colon_controler{
    colon::driver{100, [](colon_control_state state) {
                      state == colon::control_state::on ? neon_bulbs_on()
                                                        : neon_bulbs_off();
                  }},
    theme::pack_1.colon,
};

void colon::driver::run(uint32_t tick_now)
{
    static uint32_t cycle_start_tick;
    uint32_t cycle_tick = tick_now - cycle_start_tick;
    cycle_start_tick = cycle_tick >= pwm_cycle ? tick_now : cycle_start_tick;
    (*control)(cycle_tick < brightness ? control_state::on
                                       : control_state::off);
}

void colon::controller::set_style(colon::style style, uint16_t update_period)
{
    this->style = style;
    this->update_period = update_period;
}

void colon::controller::run(uint32_t tick_now)
{
    static uint32_t cycle_start_tick;
    uint32_t cycle_tick = tick_now - cycle_start_tick;
    cycle_start_tick
        = cycle_tick >= update_period ? tick_now : cycle_start_tick;
    if (cycle_tick >= update_period)
    {
        switch (this->style.type)
        {
        case colon::type::always_on:
            this->brightness = style.config.always_on.brightness;
            break;
        case colon::type::twinkle:
            twinkle_run();
            break;
        case colon::type::breath:
            breath_run();
            break;
        case colon::type::disable:
        default:
            break;
        }
    }
    driver.set_brightness(brightness);
    driver.run(cycle_tick);
}
#include <array>
std::array<int, 5> sadf;
void colon::controller::twinkle_run(void)
{
    if (this->style.config.twinkle.tick
        < (this->style.config.twinkle.state == utils::twinkle_state::off
               ? this->style.config.twinkle.off_time
               : this->style.config.twinkle.on_time))
    {
        this->style.config.twinkle.tick++;
    }
    else
    {
        this->style.config.twinkle.state
            = this->style.config.twinkle.state == utils::twinkle_state::off
                  ? utils::twinkle_state::on
                  : utils::twinkle_state::off;
        this->style.config.twinkle.tick = 0;
    }
    this->brightness
        = this->style.config.twinkle.state == utils::twinkle_state::off
              ? this->style.config.twinkle.off_brightness
              : this->style.config.twinkle.on_brightness;
}

void colon::controller::breath_run(void)
{
    this->brightness = utils::number_breath_calculate(
        this->brightness, this->style.config.breath.min_brightness,
        this->style.config.breath.max_brightness,
        this->style.config.breath.step, this->style.config.breath.direction);
}

/* extren C function */

colon_controller *
conlon_controler_create(void (*control)(colon_control_state state),
                        colon_type type, colon_config config)
{
    return new colon::controller{colon::driver{100, control},
                                 colon::style{.type = type, .config = config}};
}

void conlon_controler_destroy(colon_controller *controller)
{
    delete controller;
}

void colon_controler_run(colon_controller *controller, uint32_t tick_now)
{
    controller->run(tick_now);
}

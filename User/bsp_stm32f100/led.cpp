/**
 ******************************************************************************
 * @file    led.cpp
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

#include "led.h"
#include "bsp_ws2812b.h"
#include "theme_pack.h"
#include <algorithm>
#include <vector>

led::controller _led_controller{
    led::driver{ws2812b_send_rgb},
    theme::default_pack.led,
};

void led::driver::check_is_update(std::array<hsv, led::driver::amount> color)
{
    if (!std::equal(this->color.begin(), this->color.end(), color.begin(),
                    [](hsv &color_1, hsv &color_2) {
                        return (color_1.h == color_2.h && color_1.s == color_2.s
                                && color_1.v == color_2.v);
                    }))
    {
        this->is_update = true;
    }
}

void led::driver::run(uint32_t tick_now)
{
    if (this->is_update)
    {
        std::transform(this->color.begin(), this->color.end(),
                       this->color_rgb.begin(), color::hsv_to_rgb);
        this->control(this->color_rgb.data(), this->amount);
        this->is_update = false;
    }
}

void led::controller::run(uint32_t tick_now)
{
    uint32_t cycle_tick = tick_now - cycle_start_tick;
    cycle_start_tick
        = cycle_tick >= update_period ? tick_now : cycle_start_tick;
    if (cycle_tick >= update_period)
    {
        auto style = this->style.begin();
        auto color = this->color.begin();
        for (; style != this->style.end(); style++, color++)
        {
            switch (style->type)
            {
            case led::type::disable:
                break;
            case led::type::always_on:
                *color = style->config.always_on.color;
                break;
            case led::type::twinkle:
                twinkle_run(*style, *color);
                break;
            case led::type::breath:
                breath_run(*style, *color);
                break;
            default:
                break;
            }
        }
        driver.set_color(this->color);
        driver.run(tick_now);
    }
}

void led::controller::twinkle_run(led::style &style, hsv &color)
{
    if (style.config.twinkle.tick
        < (style.config.twinkle.state == utils::twinkle_state::off
               ? style.config.twinkle.color_1_time
               : style.config.twinkle.color_2_time))
    {
        style.config.twinkle.tick++;
    }
    else
    {
        style.config.twinkle.state
            = style.config.twinkle.state == utils::twinkle_state::off
                  ? utils::twinkle_state::on
                  : utils::twinkle_state::off;
        style.config.twinkle.tick = 0;
    }
    color = style.config.twinkle.state == utils::twinkle_state::off
                ? style.config.twinkle.color_1
                : style.config.twinkle.color_2;
}

void led::controller::breath_run(led::style &style, hsv &color)
{
    color.h = utils::number_breath_calculate(
        color.h, style.config.breath.color_1.h, style.config.breath.color_2.h,
        style.config.breath.step.h, style.config.breath.direction.h);
    color.s = utils::number_breath_calculate(
        color.s, style.config.breath.color_1.s, style.config.breath.color_2.s,
        style.config.breath.step.s, style.config.breath.direction.s);
    color.v = utils::number_breath_calculate(
        color.v, style.config.breath.color_1.v, style.config.breath.color_2.v,
        style.config.breath.step.v, style.config.breath.direction.v);
}

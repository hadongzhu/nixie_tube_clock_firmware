/**
 ******************************************************************************
 * @file    control_key.cpp
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-20
 * @brief   provide driver for key control.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "control_key.h"
#include "bsp_DS3231.h"
#include "bsp_key.h"
#include "bsp_rtc.h"
#include "display.h"
#include "factory_test.h"
#include "theme_pack.h"
#include "tick.h"
#include <type_traits>

control_key::controller control_key_controller_entity{};
control_key::clock_setting _clock_setting{};

static constexpr uint8_t item_number_position[] = {
    control_key::number_position::_1 | control_key::number_position::_2
        | control_key::number_position::_3 | control_key::number_position::_4,
    control_key::number_position::_1 | control_key::number_position::_2,
    control_key::number_position::_3 | control_key::number_position::_4,
    control_key::number_position::_1 | control_key::number_position::_2,
    control_key::number_position::_3 | control_key::number_position::_4,
};

void nixie_tube_apply_setting_style(void)
{
    std::array<nixie_tube::display::style, nixie_tube::driver::amount>
        display_style;
    for (std::size_t i = 0; i < nixie_tube::driver::amount; i++)
    {
        if (item_number_position
                [static_cast<std::underlying_type_t<control_key::setting_item>>(
                    _clock_setting._item)]
            & control_key::number_position::_1 << i)
        {
            display_style[i] = nixie_tube::preset::display::twinkle;
        }
        else
        {
            display_style[i] = nixie_tube::display::style{
                .type = nixie_tube::display::type::always_on,
                .config = nixie_tube::display::config{.always_on = {
                                                          .brightness = 100,
                                                      }}};
        }
    }
    nixie_tube_controller_entity.set_style(
        display_style,
        std::array<nixie_tube::change::style, nixie_tube::driver::amount>{
            nixie_tube::change::style{.type = nixie_tube::change::type::disable,
                                      .config = {}},
            nixie_tube::change::style{.type = nixie_tube::change::type::disable,
                                      .config = {}},
            nixie_tube::change::style{.type = nixie_tube::change::type::disable,
                                      .config = {}},
            nixie_tube::change::style{.type = nixie_tube::change::type::disable,
                                      .config = {}},
        });
}

std::array<nixie_tube::display::style, nixie_tube::driver::amount>
    origin_display_style{};
std::array<nixie_tube::change::style, nixie_tube::driver::amount>
    origin_change_style;

static void key_function_enter_clock_setting(void)
{
    display_controller_entity.mode = display::mode::clock_setting;
    display_controller_entity.content = display::content::year;
    display_controller_entity.time_display_source = &_clock_setting.time.readable;
    _clock_setting._item = control_key::setting_item::year;
    _clock_setting.time = tick_controller_entity.get_time_now();
    origin_display_style = nixie_tube_controller_entity.get_display_style();
    origin_change_style = nixie_tube_controller_entity.get_change_style();
    nixie_tube_apply_setting_style();
}

static void key_function_quit_clock_setting_without_save(void)
{
    display_controller_entity.mode = display::mode::clock;
    display_controller_entity.content = display::content::hour_minutes;
    display_controller_entity.time_display_source
        = &tick_controller_entity.get_time_now().readable;
    nixie_tube_controller_entity.set_style(origin_display_style, origin_change_style);
}

static void key_function_quit_clock_setting(void)
{
    display_controller_entity.mode = display::mode::clock;
    display_controller_entity.content = display::content::hour_minutes;
    display_controller_entity.time_display_source
        = &tick_controller_entity.get_time_now().readable;
    _clock_setting.time.readable.millisecond = 0;
    _clock_setting.time.readable.seconds = 0;
    tick_controller_entity.get_time_now().set_readable(_clock_setting.time.readable);
    nixie_tube_controller_entity.set_style(origin_display_style, origin_change_style);
    RTC_set_unix_timestamp(tick_controller_entity.get_time_now().readable.seconds);
    ds3231_set_date_time((ds3231_date_time_type *)(&(
        tick_controller_entity.get_time_now().readable.seconds)));
}

static void key_function_setting_item_switch(void)
{
    if (_clock_setting._item == control_key::setting_item::minute)
    {
        key_function_quit_clock_setting();
        return;
    }
    else
    {
        _clock_setting._item++;
    }
    switch (_clock_setting._item)
    {
    case control_key::setting_item::year:
        display_controller_entity.content = display::content::year;
        break;
    case control_key::setting_item::month:
    case control_key::setting_item::month_day:
        display_controller_entity.content = display::content::month_date;
        break;
    case control_key::setting_item::hour:
    case control_key::setting_item::minute:
        display_controller_entity.content = display::content::hour_minutes;
        break;
    default:
        break;
    }
    nixie_tube_apply_setting_style();
}

static void content_change_with_volume(int32_t volume)
{
    switch (_clock_setting._item)
    {
    case control_key::setting_item::year:
        _clock_setting.time.readable_change_year(volume);
        break;
    case control_key::setting_item::month:
        _clock_setting.time.readable_change_month(volume);
        break;
    case control_key::setting_item::month_day:
        _clock_setting.time.readable_change_month_day(volume);
        break;
    case control_key::setting_item::hour:
        _clock_setting.time.readable_change_hour(volume);
        break;
    case control_key::setting_item::minute:
        _clock_setting.time.readable_change_minutes(volume);
        break;
    default:
        break;
    }
}

static void key_function_setting_content_inc(void)
{
    content_change_with_volume(+1);
}

static void key_function_setting_content_dec(void)
{
    content_change_with_volume(-1);
}

void control_key::controller::run(void)
{
    key_event key_event = {0};
    if (_key_controller.get_event(key_event) == true)
    {
        void (*function)(void)
            = this->function[static_cast<std::size_t>(display_controller_entity.mode)]
                            [key_event.index]
                            [static_cast<std::size_t>(key_event.action)];
        if (function != nullptr)
        {
            function();
        }
    }
}

static void KeyFunction_DisplayMonthAndMonthDay(void)
{
    display_controller_entity.content = display_content::month_date;
}

static void KeyFunction_DisplayHourAndMinute(void)
{
    display_controller_entity.content = display_content::hour_minutes;
}

static void key_function_enter_nixie_tube_protect_mode(void)
{
    display_controller_entity.mode = display::mode::none;
    colon_controller_entity.set_style(colon::preset::off);
    led_controller_entity.set_style(led::preset::off);
}

static void key_function_quit_nixie_tube_protect_mode(void)
{
    display_controller_entity.mode = display::mode::clock;
    colon_controller_entity.set_style(theme::default_pack.colon);
    led_controller_entity.set_style(theme::default_pack.led);
}

void control_key_init(void)
{
    control_key_controller_entity.set_function(display::mode::clock, 0,
                                         key::action::push,
                                         KeyFunction_DisplayMonthAndMonthDay);
    control_key_controller_entity.set_function(display::mode::clock, 0,
                                         key::action::pop,
                                         KeyFunction_DisplayHourAndMinute);
    control_key_controller_entity.set_function(display::mode::clock, 0,
                                         key::action::pop_after_long_press,
                                         KeyFunction_DisplayHourAndMinute);
    control_key_controller_entity.set_function(display::mode::clock, 3,
                                         key::action::long_press,
                                         key_function_enter_clock_setting);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 3, key::action::push,
        key_function_quit_clock_setting_without_save);
    control_key_controller_entity.set_function(display::mode::clock_setting, 2,
                                         key::action::push,
                                         key_function_setting_item_switch);
    control_key_controller_entity.set_function(display::mode::clock_setting, 1,
                                         key::action::push,
                                         key_function_setting_content_inc);
    control_key_controller_entity.set_function(display::mode::clock_setting, 1,
                                         key::action::long_press_repeat,
                                         key_function_setting_content_inc);
    control_key_controller_entity.set_function(display::mode::clock_setting, 0,
                                         key::action::push,
                                         key_function_setting_content_dec);
    control_key_controller_entity.set_function(display::mode::clock_setting, 0,
                                         key::action::long_press_repeat,
                                         key_function_setting_content_dec);
    control_key_controller_entity.set_function(display::mode::clock, 1,
                                         key::action::long_press, factory_test);
    control_key_controller_entity.set_function(
        display::mode::clock, 1, key::action::pop, &theme::pack_ID::switch_ID);
    control_key_controller_entity.set_function(
        display::mode::clock, 2, key::action::push,
        key_function_enter_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 0, key::action::push,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 1, key::action::push,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 2, key::action::push,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 3, key::action::push,
        key_function_quit_nixie_tube_protect_mode);
}

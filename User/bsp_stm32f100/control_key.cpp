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
#include "cron.h"
#include "display.h"
#include "factory_test.h"
#include "storage.h"
#include "theme_pack.h"
#include "tick.h"
#include <type_traits>

control_key::controller control_key_controller_entity{};
control_key::clock_setting clock_setting_entity{};

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
                    clock_setting_entity._item)]
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

static void key_function_enter_clock_setting(void)
{
    display_controller_entity.mode = display::mode::clock_setting;
    display_controller_entity.content = display::content::year;
    display_controller_entity.time_display_source
        = &clock_setting_entity.time.readable;
    clock_setting_entity._item = control_key::setting_item::year;
    clock_setting_entity.time = tick_controller_entity.get_time_now();
    nixie_tube_apply_setting_style();
    cron_controller_entity.disable_auto_protect();
}

static void key_function_quit_clock_setting_without_save(void)
{
    display_controller_entity.mode = display::mode::clock;
    display_controller_entity.content = display::content::hour_minutes;
    display_controller_entity.time_display_source
        = &tick_controller_entity.get_time_now().readable;
    stroage_controller_entity.apply();
}

static void key_function_quit_clock_setting(void)
{
    display_controller_entity.mode = display::mode::clock;
    display_controller_entity.content = display::content::hour_minutes;
    display_controller_entity.time_display_source
        = &tick_controller_entity.get_time_now().readable;
    clock_setting_entity.time.readable.millisecond = 0;
    clock_setting_entity.time.readable.seconds = 0;
    tick_controller_entity.get_time_now() = clock_setting_entity.time.readable;
    stroage_controller_entity.apply();
    RTC_set_unix_timestamp(
        tick_controller_entity.get_time_now().readable.seconds);
    ds3231_set_date_time((ds3231_date_time_type *)(&(
        tick_controller_entity.get_time_now().readable.seconds)));
}

static void key_function_setting_item_switch(void)
{
    if (clock_setting_entity._item == control_key::setting_item::minute)
    {
        key_function_quit_clock_setting();
        return;
    }
    else
    {
        clock_setting_entity._item++;
    }
    switch (clock_setting_entity._item)
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
    switch (clock_setting_entity._item)
    {
    case control_key::setting_item::year:
        clock_setting_entity.time.readable_change_year(volume);
        break;
    case control_key::setting_item::month:
        clock_setting_entity.time.readable_change_month(volume);
        break;
    case control_key::setting_item::month_day:
        clock_setting_entity.time.readable_change_month_day(volume);
        break;
    case control_key::setting_item::hour:
        clock_setting_entity.time.readable_change_hour(volume);
        break;
    case control_key::setting_item::minute:
        clock_setting_entity.time.readable_change_minutes(volume);
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
    if (key_controller_entity.get_event(key_event) == true)
    {
        void (*function)(void)
            = this->function[static_cast<std::size_t>(
                display_controller_entity.mode)][key_event.index]
                            [static_cast<std::size_t>(key_event.action)];
        if (function != nullptr)
        {
            cron_controller_entity.update_auto_protect_task();
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

void key_function_enter_nixie_tube_protect_mode(void)
{
    display_controller_entity.mode = display::mode::none;
    colon_controller_entity.set_style(colon::preset::off);
    led_controller_entity.set_style(led::preset::off);
}

void key_function_quit_nixie_tube_protect_mode(void)
{
    display_controller_entity.mode = display::mode::clock;
    stroage_controller_entity.apply();
}

void enable_auto_protect(void)
{
    key_function_enter_nixie_tube_protect_mode();
    cron_controller_entity.enable_auto_protect();
    stroage_controller_entity.save();
}

void quit_auto_protect(void)
{
    key_function_quit_nixie_tube_protect_mode();
    cron_controller_entity.disable_auto_protect();
    stroage_controller_entity.save();
}

void control_key_init(void)
{
    control_key_controller_entity.set_function(
        display::mode::clock, 0, key::action::push,
        KeyFunction_DisplayMonthAndMonthDay);
    control_key_controller_entity.set_function(
        display::mode::clock, 0, key::action::pop,
        KeyFunction_DisplayHourAndMinute);
    control_key_controller_entity.set_function(
        display::mode::clock, 0, key::action::pop_after_long_press,
        KeyFunction_DisplayHourAndMinute);
    control_key_controller_entity.set_function(
        display::mode::clock, 3, key::action::long_press,
        key_function_enter_clock_setting);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 3, key::action::push,
        key_function_quit_clock_setting_without_save);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 2, key::action::push,
        key_function_setting_item_switch);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 1, key::action::push,
        key_function_setting_content_inc);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 1, key::action::long_press_repeat,
        key_function_setting_content_inc);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 0, key::action::push,
        key_function_setting_content_dec);
    control_key_controller_entity.set_function(
        display::mode::clock_setting, 0, key::action::long_press_repeat,
        key_function_setting_content_dec);
    control_key_controller_entity.set_function(
        display::mode::clock, 1, key::action::long_press, factory_test);
    control_key_controller_entity.set_function(
        display::mode::clock, 1, key::action::pop, &theme::pack_ID::switch_ID);
    control_key_controller_entity.set_function(
        display::mode::clock, 2, key::action::pop,
        key_function_enter_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::clock, 2, key::action::long_press, enable_auto_protect);
    control_key_controller_entity.set_function(
        display::mode::none, 0, key::action::long_press, quit_auto_protect);
    control_key_controller_entity.set_function(
        display::mode::none, 1, key::action::long_press, quit_auto_protect);
    control_key_controller_entity.set_function(
        display::mode::none, 2, key::action::long_press, quit_auto_protect);
    control_key_controller_entity.set_function(
        display::mode::none, 3, key::action::long_press, quit_auto_protect);
    control_key_controller_entity.set_function(
        display::mode::none, 0, key::action::pop,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 1, key::action::pop,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 2, key::action::pop,
        key_function_quit_nixie_tube_protect_mode);
    control_key_controller_entity.set_function(
        display::mode::none, 3, key::action::pop,
        key_function_quit_nixie_tube_protect_mode);
}

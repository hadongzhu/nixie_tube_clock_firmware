/**
 ******************************************************************************
 * @file    display.cpp
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

#include "display.h"
#include "bsp_timer.h"
#include "nixie_tube.h"
#include "tick.h"

display::controller _display_controller{display::mode::clock,
                                        display::content::hour_minutes,
                                        &_tick_controller.time_now.readable};

void display::controller::display_clock(void)
{
    switch (this->content)
    {
    case display::content::year:
        _nixie_tube_controller.set_number(
            {2, 0, static_cast<uint8_t>(this->time_display_source->year / 10),
             static_cast<uint8_t>(this->time_display_source->year % 10)});
        break;
    case display::content::month_date:
        _nixie_tube_controller.set_number(
            {static_cast<uint8_t>(this->time_display_source->month / 10),
             static_cast<uint8_t>(this->time_display_source->month % 10),
             static_cast<uint8_t>(this->time_display_source->month_day / 10),
             static_cast<uint8_t>(this->time_display_source->month_day % 10)});
        break;
    case display::content::hour_minutes:
        _nixie_tube_controller.set_number(
            {static_cast<uint8_t>(this->time_display_source->hour / 10),
             static_cast<uint8_t>(this->time_display_source->hour % 10),
             static_cast<uint8_t>(this->time_display_source->minutes / 10),
             static_cast<uint8_t>(this->time_display_source->minutes % 10)});
        break;
    default:
        break;
    }
}

void display::controller::run(void)
{
    switch (this->mode)
    {
    case display::mode::clock:
    case display::mode::clock_setting:
        display_clock();
        break;
    case display::mode::none:
        _nixie_tube_controller.set_number(
            {nixie_tube::driver::number_none, nixie_tube::driver::number_none,
             nixie_tube::driver::number_none, nixie_tube::driver::number_none});
    default:
        break;
    }
}

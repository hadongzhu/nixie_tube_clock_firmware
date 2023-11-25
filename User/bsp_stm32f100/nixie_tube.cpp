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

#include "nixie_tube.h"
#include "bsp_HV57708.h"
#include <algorithm>
#include "theme_pack.h"

nixie_tube::controller _nixie_tube_controller{
    nixie_tube::driver{HV57705_send_number},
    theme::default_pack.nixie_tube.display,
    theme::default_pack.nixie_tube.change,
};

void nixie_tube::driver::run(uint32_t tick_now)
{
    uint32_t cycle_tick = tick_now - cycle_start_tick;
    cycle_start_tick = cycle_tick >= pwm_cycle ? tick_now : cycle_start_tick;
    auto config = this->_config.begin();
    auto new_number = this->new_number.begin();
    for (; config != this->_config.end(); config++, new_number++)
    {
        if (cycle_tick < config->brightness - config->secondary_brightness)
        {
            *new_number = config->number_primary;
        }
        else if (cycle_tick < config->brightness)
        {
            *new_number = config->number_secondary;
        }
        else
        {
            *new_number = number_none;
        }
    }
    if (!std::equal(this->new_number.begin(), this->new_number.end(),
                    this->old_number.begin()))
    {
        std::copy(this->new_number.begin(), this->new_number.end(),
                  this->old_number.begin());
        (*control)(this->new_number.data(), amount);
    }
}

void nixie_tube::controller::run(uint32_t tick_now)
{
    uint32_t cycle_tick = tick_now - cycle_start_tick;
    cycle_start_tick
        = cycle_tick >= update_period ? tick_now : cycle_start_tick;
    if (cycle_tick >= update_period)
    {
        auto display = this->display_style.begin();
        auto change = this->change_style.begin();
        auto number = this->number.begin();
        auto driver_config = this->driver_config.begin();
        auto status = this->_status.begin();
        for (; number != this->number.end();
             display++, change++, number++, driver_config++, status++)
        {
            if (*status == status::display)
            {
                switch (display->type)
                {
                case display::type::always_on:
                    *driver_config = {number->new_one,
                                      display->config.always_on.brightness,
                                      nixie_tube::driver::number_none, 0};
                    break;
                case display::type::twinkle:
                    *driver_config = display_twinkle(*display, *number);
                    break;
                case display::type::breath:
                    *driver_config = display_breath(*display, *number);
                    break;
                case display::type::disable:
                default:
                    *driver_config = {number->new_one, 0,
                                      nixie_tube::driver::number_none, 0};
                }
            }
            else if (*status == status::change)
            {
                switch (change->type)
                {
                case change::type::breath:
                    *status = change_breath(*change, *display, *number,
                                            *driver_config);
                    break;
                case change::type::breath_meantime:
                    *status = change_breath_meantime(*change, *display, *number,
                                                     *driver_config);
                    break;
                case change::type::jump:
                    *status = change_jump(*change, *number, *driver_config);
                    break;
                case change::type::disable:
                default:
                    *status = status::display;
                    *driver_config = {number->new_one, 0,
                                      nixie_tube::driver::number_none, 0};
                }
            }
        }
    }
    driver.run(tick_now);
}

nixie_tube::driver::config
nixie_tube::controller::display_twinkle(nixie_tube::display::style &display,
                                        const nixie_tube::number &number)
{
    if (display.config.twinkle.tick
        < (display.config.twinkle.state == utils::twinkle_state::off
               ? display.config.twinkle.off_time
               : display.config.twinkle.on_time))
    {
        display.config.twinkle.tick++;
    }
    else
    {
        display.config.twinkle.state
            = display.config.twinkle.state == utils::twinkle_state::off
                  ? utils::twinkle_state::on
                  : utils::twinkle_state::off;
        display.config.twinkle.tick = 0;
    }
    uint8_t brightness
        = display.config.twinkle.state == utils::twinkle_state::off
              ? display.config.twinkle.off_brightness
              : display.config.twinkle.on_brightness;
    return {number.new_one, brightness, nixie_tube::driver::number_none, 0};
}

nixie_tube::driver::config
nixie_tube::controller::display_breath(nixie_tube::display::style &display,
                                       const nixie_tube::number &number)
{
    display.config.breath.brightness_init = utils::number_breath_calculate(
        display.config.breath.brightness_init,
        display.config.breath.min_brightness,
        display.config.breath.max_brightness, display.config.breath.step,
        display.config.breath.direction);
    return {number.new_one, display.config.breath.brightness_init,
            nixie_tube::driver::number_none, 0};
}

void nixie_tube::controller::set_number(
    const std::array<uint8_t, nixie_tube::driver::amount> &new_number)
{
    auto new_number_ = new_number.begin();
    auto number = this->number.begin();
    auto status = this->_status.begin();
    auto change_style = this->change_style.begin();
    for (; new_number_ != new_number.end(); new_number_++, number++, status++)
    {
        if (*new_number_ != number->new_one)
        {
            *number = nixie_tube::number{number->new_one, *new_number_};
            if (change_style->type != nixie_tube::change::type::disable)
            {
                *status = controller::status::change;
            }
        }
    }
};

nixie_tube::controller::status
nixie_tube::controller::change_breath(const nixie_tube::change::style &change,
                                      const nixie_tube::display::style &display,
                                      const nixie_tube::number &number,
                                      driver::config &driver_config)
{
    if (driver_config.number_primary == number.last_one)
    {
        if (driver_config.brightness > change.config.breath.step
            && number.last_one != nixie_tube::driver::number_none)
        {
            driver_config.brightness -= change.config.breath.step;
        }
        else
        {
            driver_config.brightness = 0;
            driver_config.number_primary = number.new_one;
        }
    }
    else if (driver_config.number_primary == number.new_one)
    {
        if (driver_config.brightness
            < display.config.always_on.brightness - change.config.breath.step)
        {
            driver_config.brightness += change.config.breath.step;
        }
        else
        {
            driver_config.brightness = display.config.always_on.brightness;
            return status::display;
        }
    }
    return status::change;
}

nixie_tube::controller::status nixie_tube::controller::change_breath_meantime(
    const nixie_tube::change::style &change,
    const nixie_tube::display::style &display, const nixie_tube::number &number,
    driver::config &driver_config)
{
    if (driver_config.number_primary == number.last_one)
    {
        if (driver_config.secondary_brightness
            < display.config.always_on.brightness
                  - change.config.breath_meantime.step)
        {
            driver_config.number_primary = number.last_one;
            driver_config.number_secondary = number.new_one;
            driver_config.secondary_brightness
                += change.config.breath_meantime.step;
        }
        else
        {
            driver_config.number_primary = number.new_one;
            driver_config.brightness = display.config.always_on.brightness;
            driver_config.number_secondary = nixie_tube::driver::number_none;
            driver_config.secondary_brightness = 0;
            return status::display;
        }
    }
    return status::change;
}

nixie_tube::controller::status
nixie_tube::controller::change_jump(nixie_tube::change::style &change,
                                    const nixie_tube::number &number,
                                    driver::config &driver_config)
{
    if (change.config.jump.tick < change.config.jump.time)
    {
        change.config.jump.tick++;
        if (change.config.jump.tick % change.config.jump.speed == 0)
        {
            driver_config.number_primary = this->get_random_0_9();
        }
    }
    else
    {
        change.config.jump.tick = 0;
        driver_config.number_primary = number.new_one;
        return status::display;
    }
    return status::change;
}

void nixie_tube_controller_set_number(nixie_tube_controller *controller,
                                      uint8_t *number, uint16_t amount)
{
    std::array<uint8_t, nixie_tube::driver::amount> number_array;
    std::copy(number, number + amount, number_array.begin());
    static_cast<nixie_tube::controller *>(controller)->set_number(number_array);
}

void nixie_tube_controller_set_display_style(
    nixie_tube_controller *controller, uint16_t index,
    nixie_tube_display_style_type type, nixie_tube_display_style_config config)
{
    static_cast<nixie_tube::controller *>(controller)
        ->get_display_style()[index]
        = nixie_tube::display::style{.type = type, .config = config};
}

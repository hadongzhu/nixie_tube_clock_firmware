/**
 ******************************************************************************
 * @file    key.cpp
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-09
 * @brief   Driver of keys.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "key.h"
#include "bsp_key.h"

key::controller _key_controller{
    std::array<bool (*)(void), key::controller::key_number>{
        &is_key_1_active,
        &is_key_2_active,
        &is_key_3_active,
        &is_key_4_active,
    },
    fifo_add_key_event,
    [](key::event &event) { return fifo_get_key_event(&event); }};

void key::controller::run()
{
    for (uint8_t index = 0; index < key::controller::key_number; ++index)
    {
        auto &key = this->keys[index];
        if (key.is_key_active())
        {
            if (key.state == key::state::free)
            {
                if (key.filter_count >= key::controller::filter_time)
                {
                    fifo_add_event(key::event{index, key::action::push});
                    key.state = key::state::wait_for_long_press;
                    key.press_count = 0;
                }
                else
                {
                    key.filter_count++;
                }
            }
            else
            {
                key.filter_count = key::controller::filter_time;
                key.press_count++;
                if (key.state == key::state::wait_for_long_press
                    && key.press_count == key.long_press_time)
                {
                    fifo_add_event(key::event{index, key::action::long_press});
                    key.state = key.repeat_cool_down_time != 0
                                    ? key::state::wait_for_repeat_cool_down
                                    : key::state::wait_for_repeat;
                    key.press_count = 0;
                }
                else if (key.repeat_interval_time != 0)
                {
                    if (key.state == key::state::wait_for_repeat_cool_down
                        && key.press_count == key.repeat_cool_down_time)
                    {
                        fifo_add_event(
                            key::event{index, key::action::long_press_repeat});
                        key.state = key::state::wait_for_repeat;
                        key.press_count = 0;
                    }
                    else if (key.state == key::state::wait_for_repeat
                             && key.press_count == key.repeat_interval_time)
                    {
                        fifo_add_event(
                            key::event{index, key::action::long_press_repeat});
                        key.press_count = 0;
                    }
                }
            }
        }
        else
        {
            if (key.state != key::state::free)
            {
                if (key.filter_count == 0)
                {
                    fifo_add_event(key::event{
                        index, key.state == key::state::wait_for_long_press
                                   ? key::action::pop
                                   : key::action::pop_after_long_press});
                    key.state = key::state::free;
                }
                else
                {
                    key.filter_count--;
                }
            }
            else
            {
                key.filter_count = 0;
            }
        }
    }
}

void key_run(key_controller *controller)
{
    controller->run();
}
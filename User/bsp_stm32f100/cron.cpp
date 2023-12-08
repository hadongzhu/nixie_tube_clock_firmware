/**
 ******************************************************************************
 * @file    cron.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    20230xFFU2-08
 * @brief   provide cron.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "cron.h"
#include "control_key.h"
#include "factory_test.h"
#include "tick.h"

cron::controller cron_controller_entity{
    tick_controller_entity.get_time_now().readable,
    std::vector<cron::task>{
        // cron::task{
        //     .time = {
        //         .seconds = 0U,
        //         .minutes = cron::exec_every_time,
        //         .hour = cron::exec_every_time,
        //         .week_day = cron::exec_every_time,
        //         .month_day = cron::exec_every_time,
        //         .month = cron::exec_every_time,
        //         .year = cron::exec_every_time,
        //     },
        //     .callback = &factory_test,
        // },
        cron::task{
            .time = {
                .seconds = 0U,
                .minutes = 0,
                .hour = 0,
                .week_day = cron::exec_every_time,
                .month_day = cron::exec_every_time,
                .month = cron::exec_every_time,
                .year = cron::exec_every_time,
            },
            .repeat_time = cron::always_repeat,
            .callback = []() {
                tick_controller_entity.sync(tick_controller_entity.resume());
            },
        },
    },
};

void cron::controller::run(void)
{
    if (last_run.seconds != source.seconds || last_run.minutes != source.minutes
        || last_run.hour != source.hour || last_run.week_day != source.week_day
        || last_run.month_day != source.month_day
        || last_run.month != source.month || last_run.year != source.year)
    {
        last_run = source;
        exec_tasks();
    }
}

void cron::controller::exec_tasks(void)
{
    for (auto &task : tasks)
    {
        if ((task.time.seconds == source.seconds || task.time.seconds == 0xFFU)
            && (task.time.minutes == source.minutes
                || task.time.minutes == 0xFFU)
            && (task.time.hour == source.hour || task.time.hour == 0xFFU)
            && (task.time.week_day == source.week_day
                || task.time.week_day == 0xFFU)
            && (task.time.month_day == source.month_day
                || task.time.month_day == 0xFFU)
            && (task.time.month == source.month || task.time.month == 0xFFU)
            && (task.time.year == source.year || task.time.year == 0xFFU))
        {
            if (task.repeat_time > 0U)
            {
                if (task.repeat_time != cron::always_repeat)
                {
                    task.repeat_time--;
                }
                task.callback();
            }
        }
    }
}

void cron::controller::update_auto_protect_task(uint32_t delay_seconds)
{
    for (auto &task : tasks)
    {
        if (task.callback == &key_function_enter_nixie_tube_protect_mode)
        {
            if (this->is_auto_protect == false)
            {
                task.repeat_time = 0U;
                return;
            }
            task.repeat_time = 1U;
            task.time = time_custom::time(source)
                            .unix_timestamp_add_second(delay_seconds)
                            .get_readable();
            return;
        }
    }
    if (is_auto_protect == true)
    {
        add_task(cron::task{
            .time = time_custom::time(source)
                        .unix_timestamp_add_second(delay_seconds)
                        .get_readable(),
            .repeat_time = 1U,
            .callback = &key_function_enter_nixie_tube_protect_mode,
        });
    }
}

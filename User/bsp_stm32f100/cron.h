/**
 ******************************************************************************
 * @file    cron.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-12-08
 * @brief   provide cron.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __CRON_H
#define __CRON_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#include <vector>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "time_custom.h"

#ifdef __cplusplus
namespace cron {
namespace impl {
#endif

struct task
{
    time_custom::readable time;
    uint32_t repeat_time;
    void (*callback)(void);
};

#ifdef __cplusplus
} // namespace impl
} // namespace cron

namespace cron {
using task = impl::task;
constexpr uint8_t exec_every_time = 0xFFU;
constexpr uint32_t always_repeat = 0xFFFFFFFFU;
class controller
{
  public:
    std::vector<task> tasks = {};
    bool is_auto_protect = false;
    time_custom::readable &source;
    time_custom::readable last_run;

    controller(void) = delete;
    controller(time_custom::readable &source) : source(source){};
    controller(time_custom::readable &source, std::vector<task> tasks)
        : source(source), tasks(tasks){};
    void add_task(task task)
    {
        tasks.push_back(task);
    };
    void run(void);
    void exec_tasks(void);

    void update_auto_protect_task(uint32_t delay_seconds = 10U);
    void enable_auto_protect(void)
    {
        is_auto_protect = true;
        update_auto_protect_task();
    }
    void disable_auto_protect(void)
    {
        is_auto_protect = false;
        update_auto_protect_task();
    }
    bool get_auto_protect(void)
    {
        return is_auto_protect;
    }
};

} // namespace cron

#endif

#ifdef __cplusplus
extern cron::controller cron_controller_entity;
#else
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */

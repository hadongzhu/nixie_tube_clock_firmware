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
    void (*callback)(void);
};

#ifdef __cplusplus
} // namespace impl
} // namespace cron

namespace cron {
using task = impl::task;
constexpr uint8_t exec_every_time = 0xFFU;
class controller
{
  public:
    std::vector<task> tasks = {};
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

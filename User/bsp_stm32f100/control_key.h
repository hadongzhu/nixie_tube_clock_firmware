/**
 ******************************************************************************
 * @file    control_key.h
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

#ifndef __CONTROL_KEY_H
#define __CONTROL_KEY_H

#ifdef __cplusplus
#include <array>
#include <cstddef>
#include <cstdint>
#else
#include <stddef.h>
#include <stdint.h>
#endif

#include "bsp_key.h"
#include "display.h"
#include "key.h"
#include "nixie_tube.h"
#include "time_custom.h"

#ifdef __cplusplus
namespace control_key {
namespace impl {
#endif

#ifdef __cplusplus
} // namespace impl
} // namespace control_key

namespace control_key {

namespace number_position {
constexpr uint8_t amount = nixie_tube::driver::amount;
constexpr uint8_t _1 = 0x01 << 0;
constexpr uint8_t _2 = 0x01 << 1;
constexpr uint8_t _3 = 0x01 << 2;
constexpr uint8_t _4 = 0x01 << 3;
}; // namespace number_position

namespace setting_item_private {
enum class setting_item
{
    year,
    month,
    month_day,
    hour,
    minute,
};

constexpr setting_item &operator++(setting_item &i, int)
{
    return i = (i == setting_item::minute)
                   ? setting_item::year
                   : static_cast<setting_item>(static_cast<int>(i) + 1);
}

} // namespace setting_item_private

using setting_item = setting_item_private::setting_item;

struct clock_setting
{

    control_key::setting_item _item = control_key::setting_item::year;
    time_custom::time time;
    clock_setting(void) = default;
};

struct controller
{
    std::array<std::array<std::array<void (*)(void), key::action_number>,
                          key::controller::key_number>,
               static_cast<std::size_t>(display::mode::amount)>
        function = {nullptr};

    controller(void) = default;
    controller(
        std::array<std::array<std::array<void (*)(void), key::action_number>,
                              key::controller::key_number>,
                   static_cast<std::size_t>(display::mode::amount)>
            function)
        : function(function){};
    void run(void);
    void set_function(display::mode mode, uint16_t key_id,
                      key_action key_action, void (*function)(void))
    {
        this->function[static_cast<std::size_t>(mode)][key_id]
                      [static_cast<std::size_t>(key_action)]
            = function;
    };
};
}; // namespace control_key

#endif

#ifdef __cplusplus
using control_key_controller = control_key::controller;
extern control_key::controller _control_key_controller;
#else
typedef struct control_key_controller control_key_controller;
extern control_key_controller _control_key_controller;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void control_key_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __CONTROL_KEY_H */

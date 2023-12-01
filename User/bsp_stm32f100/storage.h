/**
 ******************************************************************************
 * @file    storage.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-11-28
 * @brief   storage tools.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __THEME_PACK_H
#define __THEME_PACK_H

#include "bsp_define.h"
#include "bsp_internal_flash.h"
#include "colon.h"
#include "led.h"
#include "nixie_tube.h"
#include "stm32f1xx_hal_flash_ex.h"
#include <cstddef>

namespace stroage {

constexpr uint32_t default_key = 0xF87AD88DU;

struct alignas(alignof(uint32_t)) setting
{
    uint32_t key = default_key;
    uint32_t version = software_version;
    size_t size = sizeof(setting);
    struct
    {
        std::remove_reference<
            decltype(_nixie_tube_controller.get_display_style())>::type
            display_style;
        std::remove_reference<
            decltype(_nixie_tube_controller.get_change_style())>::type
            change_style;
    } nixie_tube;
    struct
    {
        std::remove_reference<decltype(_colon_controler.get_style())>::type
            style;
    } colon;
    struct
    {
        std::remove_reference<decltype(_led_controller.get_style())>::type
            style;
    } led;
};

constexpr uint32_t flash_address = 0x8000000U;
constexpr uint32_t flash_page_size = FLASH_PAGE_SIZE;
constexpr uint32_t flash_size = 0x10000U;
constexpr uint32_t setting_address
    = flash_address + flash_size - flash_page_size;

bool is_setting_valid(void);
void build_setting(stroage::setting &setting);
bool read_setting(stroage::setting &setting);
void write_setting(void);
void apply_setting(const setting &setting);
void restore_setting(void);

} // namespace stroage

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __THEME_PACK_H */

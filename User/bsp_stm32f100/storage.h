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

#ifndef __STORAGE_H
#define __STORAGE_H

#include "bsp_define.h"
#include "bsp_internal_flash.h"
#include "colon.h"
#include "led.h"
#include "nixie_tube.h"
#include "theme_pack.h"
#include "stm32f1xx_hal_flash_ex.h"

namespace stroage {

constexpr uint32_t default_key = 0xF87AD88DU;

struct alignas(alignof(uint32_t)) setting
{
    uint32_t key = default_key;
    uint32_t haedware_version = HARDWARE_VERSION;
    uint32_t firmware_version = FIRMWARE_VERSION;
    decltype(theme::pack_ID::get_ID()) theme_pack_ID;
    size_t size = sizeof(setting);
    struct
    {
        std::remove_reference<
            decltype(nixie_tube_controller_entity.get_display_style())>::type
            display_style;
        std::remove_reference<
            decltype(nixie_tube_controller_entity.get_change_style())>::type
            change_style;
    } nixie_tube;
    struct
    {
        std::remove_reference<decltype(colon_controller_entity.get_style())>::type
            style;
    } colon;
    struct
    {
        std::remove_reference<decltype(led_controller_entity.get_style())>::type
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

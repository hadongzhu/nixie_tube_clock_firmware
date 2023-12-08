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
#include "stm32f1xx_hal_flash_ex.h"
#include "theme_pack.h"

namespace stroage {

constexpr uint32_t default_key = 0xF87AD88DU;

struct alignas(alignof(uint32_t)) setting
{
    uint32_t key = default_key;
    uint32_t haedware_version = HARDWARE_VERSION;
    uint32_t firmware_version = FIRMWARE_VERSION;
    decltype(theme::pack_ID::get_ID()) theme_pack_ID;
    bool is_auto_protect;
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
        std::remove_reference<
            decltype(colon_controller_entity.get_style())>::type style;
    } colon;
    struct
    {
        std::remove_reference<decltype(led_controller_entity.get_style())>::type
            style;
    } led;
};

class controller
{
  public:
    static constexpr uint32_t flash_address = 0x8000000U;
    static constexpr uint32_t flash_page_size = FLASH_PAGE_SIZE;
    static constexpr uint32_t flash_size = 0x10000U;
    static constexpr uint32_t page_number = 1U;
    static constexpr uint32_t size = flash_page_size * page_number;
    static constexpr uint32_t address = flash_address + flash_size - size;

  private:
    uint32_t setting_address = address;
    bool is_valid = false;

  public:
    void restore(void);
    stroage::setting *get(void)
    {
        return reinterpret_cast<stroage::setting *>(setting_address);
    };
    bool check(void)
    {
        return this->get()->key == default_key
               && this->get()->haedware_version == HARDWARE_VERSION
               && this->get()->firmware_version == FIRMWARE_VERSION;
    };
    void init(void)
    {
        setting_address = reinterpret_cast<uint32_t>(
            std::find_if(reinterpret_cast<uint8_t *>(address),
                         reinterpret_cast<uint8_t *>(address + size),
                         [](uint8_t data) { return data != 0x00; }));

        if (setting_address
                <= address + size - offsetof(setting, nixie_tube.display_style)
            && this->check())
        {
            this->is_valid = true;
        }
        else
        {
            this->is_valid = false;
        }
    };
    void apply(void);
    void build(setting &setting);
    void save(void);
};

bool is_setting_valid(void);
void build_setting(stroage::setting &setting);
bool read_setting(stroage::setting &setting);
void write_setting(void);
void apply_setting(const setting &setting);
void restore_setting(void);

} // namespace stroage

extern stroage::controller stroage_controller_entity;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __THEME_PACK_H */

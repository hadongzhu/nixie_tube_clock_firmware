/**
 ******************************************************************************
 * @file    storage.cpp
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

#include "storage.h"
#include "colon.h"
#include "led.h"
#include "nixie_tube.h"
#include "cron.h"
#include <cstddef>

stroage::controller stroage_controller_entity;

void stroage::controller::save(void)
{
    stroage::setting setting;
    this->build(setting);
    bool is_need_to_earese = false;
    if ((this->is_valid == false)
        || (this->setting_address + this->get()->size + setting.size
            > this->address + this->size)
        || (is_internal_flash_writeable(
                this->setting_address + this->get()->size, setting.size)
            == false))
    {
        is_need_to_earese = true;
    }
    enter_internal_flash_write_erase_mode();
    if (is_need_to_earese)
    {
        uint32_t new_setting_address = this->address;
        erase_internal_flash(this->address, this->size);

        write_internal_flash(new_setting_address,
                             reinterpret_cast<uint8_t *>(&setting),
                             setting.size);
        this->setting_address = new_setting_address;
        this->is_valid = true;
    }
    else
    {
        uint32_t new_setting_address
            = this->setting_address + this->get()->size;
        reset_internal_flash(this->setting_address, this->get()->size);
        write_internal_flash(new_setting_address,
                             reinterpret_cast<uint8_t *>(&setting),
                             setting.size);
        this->setting_address = new_setting_address;
        this->is_valid = true;
    }
    exit_internal_flash_write_erase_mode();
}

void stroage::controller::build(setting &setting_struct)
{
    setting_struct.key = default_key;
    setting_struct.haedware_version = HARDWARE_VERSION;
    setting_struct.firmware_version = FIRMWARE_VERSION;
    setting_struct.theme_pack_ID = theme::pack_ID::get_ID();
    setting_struct.is_auto_protect = cron_controller_entity.get_auto_protect();
    if (setting_struct.theme_pack_ID
        == static_cast<std::underlying_type_t<theme::pack_ID::ID>>(
            theme::pack_ID::ID::costom))
    {
        setting_struct.size = sizeof(setting_struct);
        setting_struct.nixie_tube.display_style
            = nixie_tube_controller_entity.get_display_style();
        setting_struct.nixie_tube.change_style
            = nixie_tube_controller_entity.get_change_style();
        setting_struct.colon.style = colon_controller_entity.get_style();
        setting_struct.led.style = led_controller_entity.get_style();
    }
    else
    {
        setting_struct.size = offsetof(setting, nixie_tube);
    }
}

void stroage::controller::apply(void)
{
    this->get()->is_auto_protect
        ? cron_controller_entity.enable_auto_protect()
        : cron_controller_entity.disable_auto_protect();
    if (this->get()->theme_pack_ID
        == static_cast<std::underlying_type_t<theme::pack_ID::ID>>(
            theme::pack_ID::ID::costom))
    {
        nixie_tube_controller_entity.set_style(
            this->get()->nixie_tube.display_style,
            this->get()->nixie_tube.change_style);
        colon_controller_entity.set_style(this->get()->colon.style);
        led_controller_entity.set_style(this->get()->led.style);
    }
    else
    {
        nixie_tube_controller_entity.set_style(
            theme::packs[this->get()->theme_pack_ID].nixie_tube.display,
            theme::packs[this->get()->theme_pack_ID].nixie_tube.change);
        colon_controller_entity.set_style(
            theme::packs[this->get()->theme_pack_ID].colon);
        led_controller_entity.set_style(
            theme::packs[this->get()->theme_pack_ID].led);
    }
    theme::pack_ID::set_ID(this->get()->theme_pack_ID);
}

void stroage::controller::restore(void)
{
    this->init();
    this->is_valid ? this->apply() : this->save();
}

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

bool stroage::read_setting(stroage::setting &setting)
{
    setting = *(stroage::setting *)setting_address;
    return true;
}

bool stroage::is_setting_valid(void)
{
    return reinterpret_cast<stroage::setting *>(stroage::setting_address)->key
               == default_key
           && reinterpret_cast<stroage::setting *>(stroage::setting_address)
                      ->version
                  == software_version;
}

void stroage::write_setting(void)
{
    stroage::setting setting;
    build_setting(setting);
    program_internal_flash(setting_address, (uint8_t *)&setting,
                           sizeof(setting));
}

void stroage::build_setting(setting &setting)
{
    setting.version = software_version;
    setting.size = sizeof(setting);
    setting.nixie_tube.display_style
        = _nixie_tube_controller.get_display_style();
    setting.nixie_tube.change_style = _nixie_tube_controller.get_change_style();
    setting.colon.style = _colon_controler.get_style();
    setting.led.style = _led_controller.get_style();
}

void stroage::apply_setting(const setting &setting)
{
    _nixie_tube_controller.set_style(setting.nixie_tube.display_style,
                                     setting.nixie_tube.change_style);
    _colon_controler.set_style(setting.colon.style);
    _led_controller.set_style(setting.led.style);
}

void stroage::restore_setting(void)
{
    if (stroage::is_setting_valid())
    {
        stroage::apply_setting(
            *reinterpret_cast<stroage::setting *>(stroage::setting_address));
    }
    else
    {
        stroage::write_setting();
    }
}

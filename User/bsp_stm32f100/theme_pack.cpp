/**
 ******************************************************************************
 * @file    theme_pack.cpp
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-26
 * @brief   preset theme pack.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "theme_pack.h"
#include "storage.h"

void theme::apply(theme::pack pack)
{
    _colon_controler.set_style(pack.colon);
    _led_controller.set_style(pack.led);
    _nixie_tube_controller.set_style(pack.nixie_tube.display,
                                     pack.nixie_tube.change);
}

theme::pack_ID::ID theme::pack_ID::id = theme::pack_ID::ID::default_pack;

void theme::pack_ID::switch_ID(void)
{
    if (id != ID::costom)
    {
        id = static_cast<ID>(
            (static_cast<std::underlying_type_t<ID>>(id) + 1)
            % (static_cast<std::underlying_type_t<ID>>(ID::amount)));
    }
    else
    {
        id = ID::default_pack;
    }
    apply(packs[get_ID()]);
    stroage::write_setting();
};

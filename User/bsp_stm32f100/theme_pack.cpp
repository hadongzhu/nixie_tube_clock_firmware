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
    colon_controller_entity.set_style(packs[get_ID()].colon);
    led_controller_entity.set_style(packs[get_ID()].led);
    nixie_tube_controller_entity.set_style(packs[get_ID()].nixie_tube.display,
                                           packs[get_ID()].nixie_tube.change);
    stroage_controller_entity.save();
};

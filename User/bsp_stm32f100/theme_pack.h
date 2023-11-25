/**
 ******************************************************************************
 * @file    theme_pack.h
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

#ifndef __THEME_PACK_H
#define __THEME_PACK_H

#include "colon.h"
#include "led.h"
#include "nixie_tube.h"

namespace theme {

struct pack
{
    colon::style colon;
    led::style led;
    struct
    {
        nixie_tube::display::style display;
        nixie_tube::change::style change;
    } nixie_tube;
};

static constexpr pack pack_1{
    .colon = colon::preset::breath,
    .led = led::preset::breath,
    .nixie_tube = {
        .display = nixie_tube::preset::display::always_on,
        .change = nixie_tube::preset::change::breath_meantime,
    },
};

static constexpr pack pack_2{
    .colon = colon::preset::twinkle,
    .led = led::preset::off,
    .nixie_tube = {
        .display = nixie_tube::preset::display::always_on,
        .change = nixie_tube::preset::change::disable,
    },
};

static constexpr pack default_pack = pack_1;

} // namespace theme

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __THEME_PACK_H */


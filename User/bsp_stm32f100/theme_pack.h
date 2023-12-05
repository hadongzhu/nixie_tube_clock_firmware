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
#include <array>

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

static constexpr pack default_pack{
    .colon = colon::preset::breath,
    .led = led::preset::breath,
    .nixie_tube = {
        .display = nixie_tube::preset::display::always_on,
        .change = nixie_tube::preset::change::breath_meantime,
    },
};

static constexpr std::array<pack, 5> packs = {
    default_pack,
    pack{
        .colon = colon::preset::breath,
        .led = led::preset::breath_purple,
        .nixie_tube = {
            .display = nixie_tube::preset::display::always_on,
            .change = nixie_tube::preset::change::breath_meantime,
        },
    },
    pack{
        .colon = colon::preset::breath,
        .led = led::preset::breath,
        .nixie_tube = {
            .display = nixie_tube::preset::display::always_on,
            .change = nixie_tube::preset::change::breath,
        },
    },
    pack{
        .colon = colon::preset::breath,
        .led = led::preset::breath,
        .nixie_tube = {
            .display = nixie_tube::preset::display::always_on,
            .change = nixie_tube::preset::change::jump,
        },
    },
    pack{
        .colon = colon::preset::twinkle,
        .led = led::preset::off,
        .nixie_tube = {
            .display = nixie_tube::preset::display::always_on,
            .change = nixie_tube::preset::change::disable,
        },
    },
};

void apply(theme::pack pack);

class pack_ID
{
  private:
    enum class ID : int32_t
    {
        costom = -1,
        default_pack = 0,
        pack_1 = 0,
        pack_2,
        pack_3,
        pack_4,
        pack_5,
        amount,
    };
    static ID id;
    pack_ID() = delete;

  public:
    static std::underlying_type_t<ID> get_ID(void)
    {
        return static_cast<std::underlying_type_t<ID>>(id);
    };
    static void set_ID(std::underlying_type_t<ID> id)
    {
        pack_ID::id = static_cast<ID>(id);
    };
    static void switch_ID(void);
};

} // namespace theme

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* __THEME_PACK_H */

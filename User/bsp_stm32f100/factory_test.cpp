/**
 ******************************************************************************
 * @file    factory_test.cpp
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-25
 * @brief   factory test.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "factory_test.h"
#include "bsp_HV57708.h"
#include "bsp_neon_bulb.h"
#include "bsp_timer.h"
#include "bsp_ws2812b.h"
#include "color.h"
#include <array>

void factory_test(void)
{
    std::array<uint8_t, 4> display_number;
    std::array<color::rgb, 4> color;
    /* reset */
    display_number.fill(11);
    HV57705_send_number(display_number.data(), 4);
    color.fill(color::rgb{0, 0, 0});
    ws2812b_send_rgb(color.data(), 4);
    neon_bulbs_off();
    /* nixie tube test */
    display_number.fill(0);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(1);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(2);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(3);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(4);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(5);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(6);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(7);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(8);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(9);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(10);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    display_number.fill(11);
    HV57705_send_number(display_number.data(), 4);
    delay_n100us(10000);
    /* ws2812b test */
    color.fill(color::rgb{255, 0, 0});
    ws2812b_send_rgb(color.data(), 4);
    delay_n100us(10000);
    color.fill(color::rgb{0, 255, 0});
    ws2812b_send_rgb(color.data(), 4);
    delay_n100us(10000);
    color.fill(color::rgb{0, 0, 255});
    ws2812b_send_rgb(color.data(), 4);
    delay_n100us(10000);
    color.fill(color::rgb{0, 0, 0});
    ws2812b_send_rgb(color.data(), 4);
    delay_n100us(10000);
    /* colon test */
    neon_bulbs_on();
    delay_n100us(10000);
    neon_bulbs_off();
    delay_n100us(10000);
}

/*****END OF FILE****/

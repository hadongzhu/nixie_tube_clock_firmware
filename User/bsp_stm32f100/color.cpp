/**
 ******************************************************************************
 * @file    color.cpp
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide tools for color convertion.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "color.h"

color::rgb color::hsv_to_rgb(color::hsv const &hsv)
{
    color::rgb rgb;
    if (hsv.v == 0)
    {
        rgb.r = rgb.g = rgb.b = 0;
    }
    else if (hsv.s == 0)
    {
        rgb.r = rgb.g = rgb.b = hsv.v;
    }
    else
    {
        uint16_t hue = hsv.h % 360 + (hsv.h % 360 < 0 ? 360 : 0);
        int16_t sector = hue / 60;
        int16_t angle = sector & 1 ? 60 - hue % 60 : hue % 60;

        int16_t high = hsv.v;
        int16_t low = (255 - hsv.s) * high / 255;
        int16_t middle = low + (high - low) * angle / 60;

        switch (sector)
        {
        case 0: // red . yellow
            rgb.r = high;
            rgb.g = middle;
            rgb.b = low;
            break;
        case 1: // yellow . green
            rgb.r = middle;
            rgb.g = high;
            rgb.b = low;
            break;
        case 2: // green . cyan
            rgb.r = low;
            rgb.g = high;
            rgb.b = middle;
            break;
        case 3: // cyan . blue
            rgb.r = low;
            rgb.g = middle;
            rgb.b = high;
            break;
        case 4: // blue . magenta
            rgb.r = middle;
            rgb.g = low;
            rgb.b = high;
            break;
        case 5: // magenta . red
            rgb.r = high;
            rgb.g = low;
            rgb.b = middle;
        }
    }
    return rgb;
}

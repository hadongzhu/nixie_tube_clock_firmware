/**
 ******************************************************************************
 * @file    color.h
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

#ifndef __COLOR_H
#define __COLOR_H

#ifdef __cplusplus
    #include <cstddef>
    #include <cstdint>
#else
    #include <stddef.h>
    #include <stdint.h>
#endif

#ifdef __cplusplus
namespace color {
namespace impl {
#endif

struct color_rgb {
    uint8_t r, g, b;
};

struct color_hsv {
    int16_t h;
    uint8_t s, v;
};
#ifdef __cplusplus
} // namespace impl
} // namespace color

namespace color {
using rgb = impl::color_rgb;
using hsv = impl::color_hsv;
color::rgb hsv_to_rgb(color::hsv const &hsv);
} // namespace color

#endif

#ifdef __cplusplus
using color_rgb = color::rgb;
using color_hsv = color::hsv;
#else
typedef struct color_rgb color_rgb;
typedef struct color_hsv color_hsv;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif/* __COLOR_H */


/**
 ******************************************************************************
 * @file    bsp_ws2812b.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-07
 * @brief   driver of ws2812b.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_WS2812B_H
#define __BSP_WS2812B_H

#include <stdint.h>

// #define BSP_WS2812B_USE_GPIO
#define BSP_WS2812B_USE_DMA_TIM

#if defined(BSP_WS2812B_USE_GPIO) && defined(BSP_WS2812B_USE_DMA_TIM)
    #warning "please choose only one way to drive WS2812B! default use GPIO."
    #undef BSP_WS2812B_USE_DMA_TIM
#elif !defined(BSP_WS2812B_USE_GPIO) && !defined(BSP_WS2812B_USE_DMA_TIM)
    #warning "please choose one way to drive WS2812B! default use GPIO."
    #define BSP_WS2812B_USE_GPIO
#endif

#include "led.h"

#ifdef __cplusplus
 extern "C" {
#endif 

void ws2812b_init(void);
void ws2812b_send_rgb(led_rgb *rgb, uint16_t amount);
     
#ifdef __cplusplus
}
#endif

#endif /* __BSP_WS2812B_H */

/**
 ******************************************************************************
 * @file    bsp_define.h
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

#ifndef __BSP_DEFINE_H
#define __BSP_DEFINE_H

#define hardware_version 1

#define software_version 1

#if hardware_version == 1
/* HV57708 */
#define HV57708_CLK_GPIO_PORT    B
#define HV57708_CLK_GPIO_PIN     4
#define HV57708_LE_GPIO_PORT     B
#define HV57708_LE_GPIO_PIN      3
#define HV57708_BL_GPIO_PORT     B
#define HV57708_BL_GPIO_PIN      1
#define HV57708_POL_GPIO_PORT    B
#define HV57708_POL_GPIO_PIN     0
#define HV57708_DIN1_GPIO_PORT   B
#define HV57708_DIN1_GPIO_PIN    12
#define HV57708_DIN2_GPIO_PORT   B
#define HV57708_DIN2_GPIO_PIN    13
#define HV57708_DIN3_GPIO_PORT   B
#define HV57708_DIN3_GPIO_PIN    14
#define HV57708_DIN4_GPIO_PORT   B
#define HV57708_DIN4_GPIO_PIN    15
/* key */
#define KEY1_GPIO_PORT           A
#define KEY1_GPIO_PIN            0
#define KEY2_GPIO_PORT           A
#define KEY2_GPIO_PIN            1
#define KEY3_GPIO_PORT           A
#define KEY3_GPIO_PIN            2
#define KEY4_GPIO_PORT           A
#define KEY4_GPIO_PIN            3
/* i2c */
#define I2C_SCL_GPIO_PORT        B
#define I2C_SCL_GPIO_PIN         6
#define I2C_SDA_GPIO_PORT        B
#define I2C_SDA_GPIO_PIN         7
/* neon_bulb */
#define NEON_BULB_UP_GPIO_PORT   A
#define NEON_BULB_UP_GPIO_PIN    4
#define NEON_BULB_DOWN_GPIO_PORT B
#define NEON_BULB_DOWN_GPIO_PIN  9
/* ws2812b */
/* gpio setting */
#define WS2812B_DIN_GPIO_PORT    A
#define WS2812B_DIN_GPIO_PIN     9
/* DMA setting */
#define WS2812B_DMA_IRQ_PRIORITY 0
#define WS2812B_DMA              1
#define WS2812B_DMA_CHANNEL      3
/* TIM setting */
#define WS2812B_TIM              1
#define WS2812B_TIM_CHANNEL      2
#else
#error "this hardware version is not supported!"
#endif

#endif // __BSP_DEFINE_H

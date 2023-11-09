/**
 ******************************************************************************
 * @file    bsp_i2c_gpio.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-06-07
 * @brief   Driver of i2c(gpio).
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_I2C_GPIO_H
#define __BSP_I2C_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define USE_SOFT_I2C
// #define USE_HARD_I2C

#define I2C_WR 0x00
#define I2C_RD 0x01

void bsp_I2C_init(void);
uint8_t I2C_check_device(uint8_t deviceAddress);
uint8_t I2C_read_multiple_bytes(uint8_t deviceAddress, uint8_t regAddress,
                                uint8_t length, uint8_t *buffer);
uint8_t I2C_write_multiple_bytes(uint8_t deviceAddress, uint8_t regAddress,
                                 uint8_t length, uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C_GPIO_H */

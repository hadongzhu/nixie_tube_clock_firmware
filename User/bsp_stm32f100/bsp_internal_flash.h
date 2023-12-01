/**
 ******************************************************************************
 * @file    bsp_internal_flash.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-11-26
 * @brief   Driver of internal flash.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_INTERNEL_FLASH_H
#define __BSP_INTERNEL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void program_internal_flash(uint32_t address, uint8_t *data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C_GPIO_H */

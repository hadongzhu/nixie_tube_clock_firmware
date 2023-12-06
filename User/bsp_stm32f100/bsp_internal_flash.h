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

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

void enter_internal_flash_write_erase_mode(void);
void exit_internal_flash_write_erase_mode(void);
bool is_internal_flash_writeable(uint32_t address, size_t size);
bool is_internal_flash_reset(uint32_t address, size_t size);
bool memcmp_val(void *address, size_t size, uint8_t val);
bool erase_internal_flash(uint32_t address, uint32_t length);
bool write_internal_flash(uint32_t address, uint8_t *data, uint32_t length);
bool reset_internal_flash(uint32_t address, uint32_t length);
void program_internal_flash(uint32_t address, uint8_t *data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_I2C_GPIO_H */

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

#include "bsp_internal_flash.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <string.h>

static inline uint32_t ceil(uint32_t dividend, uint32_t divisor)
{
    return (dividend - 1) / divisor + 1;
}

bool verify_internal_flash(uint32_t address, uint8_t *data, uint32_t length)
{
    return memcpy((void *)data, (void *)address, length) == 0;
}

void enter_internal_flash_write_erase_mode(void)
{
    __HAL_RCC_HSI_ENABLE();
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
    {}

    HAL_FLASH_Unlock();
}

void exit_internal_flash_write_erase_mode(void)
{
    HAL_FLASH_Lock();

    __HAL_RCC_HSI_DISABLE();
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) != RESET)
    {}
}

bool erase_internal_flash(uint32_t address, uint32_t length)
{
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = address;
    EraseInitStruct.NbPages = ceil(length, FLASH_PAGE_SIZE);
    uint32_t PageError = 0;
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
        while (1)
            ;
    }
    return true;
}

bool write_internal_flash(uint32_t address, uint8_t *data, uint32_t length)
{
    for (uint32_t index = 0; index < length; index += 2)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + index,
                              *(uint16_t *)(data + index))
            != HAL_OK)
        {
            while (1)
                ;
        }
    }
    if (length % 2 == 1)
    {
        uint16_t raw = 0xFFFF & ((uint16_t) * (data + length - 1));
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address + length - 1,
                              raw)
            != HAL_OK)
        {
            while (1)
                ;
        }
    }
    return true;
}

void program_internal_flash(uint32_t address, uint8_t *data, uint32_t length)
{
    enter_internal_flash_write_erase_mode();
    erase_internal_flash(address, length);
    write_internal_flash(address, data, length);
    exit_internal_flash_write_erase_mode();
}

/**
 ******************************************************************************
 * @file    bsp_key.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-06-05
 * @brief   Driver of keys.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "key.h"
#include <stdbool.h>
#include <stdint.h>

#define KEY_FIFO_SIZE 10U

void key_init(void);

bool is_key_1_active(void);
bool is_key_2_active(void);
bool is_key_3_active(void);
bool is_key_4_active(void);

void fifo_add_key_event(key_event event);
bool fifo_get_key_event(key_event *KeyEvent);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_KEY_H */

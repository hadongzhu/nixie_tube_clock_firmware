/**
 ******************************************************************************
 * @file    bsp_key.c
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

#include "bsp_key.h"
#include "fifo.h"
#include "gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

#define KEY1_GPIO_PORT A
#define KEY1_GPIO_PIN  0
#define KEY2_GPIO_PORT A
#define KEY2_GPIO_PIN  1
#define KEY3_GPIO_PORT A
#define KEY3_GPIO_PIN  2
#define KEY4_GPIO_PORT A
#define KEY4_GPIO_PIN  3

// #define KEY_READ_PIN(PIN)       KEY##PIN##_READ()
#define KEY_READ_PIN(PIN) READ_PIN(KEY##PIN##_GPIO_PORT, KEY##PIN##_GPIO_PIN)

static key_event key_event_fifo_buf[KEY_FIFO_SIZE];
static struct fifo_descriptor key_event_fifo;

bool is_key_1_active(void)
{
    return !KEY_READ_PIN(1);
}

bool is_key_2_active(void)
{
    return !KEY_READ_PIN(2);
}

bool is_key_3_active(void)
{
    return !KEY_READ_PIN(3);
}

bool is_key_4_active(void)
{
    return !KEY_READ_PIN(4);
}

/**
 * @brief  Initialize GPIO pins and initialize structures.
 * @param  None.
 * @retval None.
 */
void key_init(void)
{
    /* Initialize key harware*/
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(
        LL_GPIO_CLOCK(KEY1_GPIO_PORT) | LL_GPIO_CLOCK(KEY2_GPIO_PORT)
        | LL_GPIO_CLOCK(KEY3_GPIO_PORT) | LL_GPIO_CLOCK(KEY4_GPIO_PORT));

    GPIO_InitStruct.Pin = LL_GPIO_PIN(KEY1_GPIO_PIN);
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(LL_GPIO_PORT(KEY1_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(KEY2_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(KEY2_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(KEY3_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(KEY3_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(KEY4_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(KEY4_GPIO_PORT), &GPIO_InitStruct);

    (void)fifo_create_static(&key_event_fifo, key_event_fifo_buf, KEY_FIFO_SIZE,
                             sizeof(key_event));
}

/**
 * @brief  Add key event to fifo.
 * @param  KeyEvent: Key event to add.
 * @retval true if success, false if fifo is full.
 */
void fifo_add_key_event(key_event event)
{
    key_event event_temp;
    while (fifo_add(&key_event_fifo, &event) == false)
    {
        fifo_get(&key_event_fifo, &event_temp);
    }
}

/**
 * @brief  Get key event from fifo.
 * @param  KeyEvent: Pointer to key event structure to get.
 * @retval true if success, false if fifo is empty.
 */
bool fifo_get_key_event(key_event *event)
{
    return fifo_get(&key_event_fifo, event);
}

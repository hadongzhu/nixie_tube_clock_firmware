/**
 ******************************************************************************
 * @file    gpio.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   gpio driver.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __GPIO_H
#define __GPIO_H

#ifndef __cplusplus
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include <stdbool.h>
#endif

#define PIN_STATE_SET   true
#define PIN_STATE_RESET false

#define GPIO_PORT(PORT)  GPIO_PORT_(PORT)
#define GPIO_PORT_(PORT) GPIO##PORT

#define HAL_GPIO_PORT(PORT) GPIO_PORT(PORT)
#define LL_GPIO_PORT(PORT)  GPIO_PORT(PORT)

#define HAL_GPIO_PIN(PIN)  HAL_GPIO_PIN_(PIN)
#define HAL_GPIO_PIN_(PIN) GPIO_PIN_##PIN

#define LL_GPIO_PIN(PIN)  LL_GPIO_PIN_(PIN)
#define LL_GPIO_PIN_(PIN) LL_GPIO_PIN_##PIN

#define HAL_GPIO_CLOCK_ENABLE(PORT)  HAL_GPIO_CLOCK_ENABLE_(PORT)
#define HAL_GPIO_CLOCK_ENABLE_(PORT) __HAL_RCC_GPIO##PORT##_CLK_ENABLE()

#define LL_GPIO_CLOCK(PORT)  LL_GPIO_CLOCK_(PORT)
#define LL_GPIO_CLOCK_(PORT) LL_APB2_GRP1_PERIPH_GPIO##PORT

#define LL_DMA_CHANNEL(CHANNEL)  _LL_DMA_CHANNEL(CHANNEL)
#define _LL_DMA_CHANNEL(CHANNEL) LL_DMA_CHANNEL_##CHANNEL

#define DMAx_Channelx_IRQn(INDEX, CHANNEL)  _DMAx_Channelx_IRQn(INDEX, CHANNEL)
#define _DMAx_Channelx_IRQn(INDEX, CHANNEL) DMA##INDEX##_Channel##CHANNEL##_IRQn

#define DMA(INDEX)  _DMA(INDEX)
#define _DMA(INDEX) DMA##INDEX

#define TIM(INDEX)  _TIM(INDEX)
#define _TIM(INDEX) TIM##INDEX

#define TIM_CCR(INDEX, CHANNEL)  _TIM_CCR(INDEX, CHANNEL)
#define _TIM_CCR(INDEX, CHANNEL) TIM(INDEX)->CCR##CHANNEL

#define LL_TIM_CHANNEL(CHANNEL)  _LL_TIM_CHANNEL(CHANNEL)
#define _LL_TIM_CHANNEL(CHANNEL) LL_TIM_CHANNEL_CH##CHANNEL

#define __LL_DMA_EnableClock(INDEX) _LL_DMA_EnableClock(INDEX)
#define _LL_DMA_EnableClock(INDEX)                                             \
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA##INDEX)

#define __LL_TIM_OC_SetCompareCH(INDEX, CHANNEL, VALUE)                          \
    _LL_TIM_OC_SetCompareCH(INDEX, CHANNEL, VALUE)
#define _LL_TIM_OC_SetCompareCH(INDEX, CHANNEL, VALUE)                         \
    LL_TIM_OC_SetCompareCH##CHANNEL(TIM(INDEX), VALUE)

#define __LL_TIM_EnableDMAReq_CC(INDEX, CHANNEL)                                 \
    _LL_TIM_EnableDMAReq_CC(INDEX, CHANNEL)
#define _LL_TIM_EnableDMAReq_CC(INDEX, CHANNEL)                                \
    LL_TIM_EnableDMAReq_CC##CHANNEL(TIM(INDEX))

#define __LL_DMA_IsActiveFlag(INDEX, CHANNEL, FLAG)                               \
    _LL_DMA_IsActiveFlag(INDEX, CHANNEL, FLAG)
#define _LL_DMA_IsActiveFlag(INDEX, CHANNEL, FLAG)                             \
    LL_DMA_IsActiveFlag_##FLAG##CHANNEL(DMA(INDEX))

#define __LL_DMA_ClearFlag(INDEX, CHANNEL, FLAG)                                 \
    _LL_DMA_ClearFlag(INDEX, CHANNEL, FLAG)
#define _LL_DMA_ClearFlag(INDEX, CHANNEL, FLAG)                                \
    LL_DMA_ClearFlag_##FLAG##CHANNEL(DMA(INDEX))

#define DMAx_Channelx_IRQHandler(INDEX, CHANNEL)                                \
    _DMAx_Channelx_IRQHandler(INDEX, CHANNEL)
#define _DMAx_Channelx_IRQHandler(INDEX, CHANNEL)                              \
    DMA##INDEX##_Channel##CHANNEL##_IRQHandler

#define SET_PIN(PORT, PIN)                                                     \
    WRITE_REG(HAL_GPIO_PORT(PORT)->BSRR, HAL_GPIO_PIN(PIN))
#define RESET_PIN(PORT, PIN)                                                   \
    WRITE_REG(HAL_GPIO_PORT(PORT)->BRR, HAL_GPIO_PIN(PIN))
#define READ_PIN(PORT, PIN)                                                    \
    READ_BIT(HAL_GPIO_PORT(PORT)->IDR, HAL_GPIO_PIN(PIN))

#define set_pin_define(PORT, PIN) set_pin_define_(PORT, PIN)
#define set_pin_define_(PORT, PIN)                                             \
    set_pin_declear(PORT, PIN)                                                 \
    {                                                                          \
        SET_PIN(PORT, PIN);                                                    \
    }

#define reset_pin_define(PORT, PIN) reset_pin_define_(PORT, PIN)
#define reset_pin_define_(PORT, PIN)                                           \
    void gpio_##PORT##_pin_##PIN##_reset()                                     \
    {                                                                          \
        RESET_PIN(PORT, PIN);                                                  \
    }

#define set_pin_state_define(PORT, PIN) set_pin_state_define_(PORT, PIN)
#define set_pin_state_define_(PORT, PIN)                                       \
    void gpio_##PORT##_pin_##PIN##_set_state(bool state)                       \
    {                                                                          \
        state ? SET_PIN(PORT, PIN) : RESET_PIN(PORT, PIN);                     \
    }

#define read_pin_define(PORT, PIN) read_pin_define_(PORT, PIN)
#define read_pin_define_(PORT, PIN)                                            \
    uint8_t gpio_##PORT##_pin_##PIN##_read()                                   \
    {                                                                          \
        return READ_PIN(PORT, PIN);                                            \
    }

#define set_pin(PORT, PIN)              set_pin_(PORT, PIN)
#define set_pin_(PORT, PIN)             gpio_##PORT##_pin_##PIN##_set(PORT, PIN)
#define reset_pin(PORT, PIN)            reset_pin_(PORT, PIN)
#define reset_pin_(PORT, PIN)           gpio_##PORT##_pin_##PIN##_reset()
#define set_pin_state(PORT, PIN, STATE) set_pin_state_(PORT, PIN, STATE)
#define set_pin_state_(PORT, PIN, STATE)                                       \
    gpio_##PORT##_pin_##PIN##_set_state(STATE)
#define read_pin(PORT, PIN)  read_pin_(PORT, PIN)
#define read_pin_(PORT, PIN) gpio_##PORT##_pin_##PIN##_read()

#define set_pin_declear(PORT, PIN)       set_pin_declear_(PORT, PIN)
#define set_pin_declear_(PORT, PIN)      void gpio_##PORT##_pin_##PIN##_set()
#define reset_pin_declear(PORT, PIN)     reset_pin_declear_(PORT, PIN)
#define reset_pin_declear_(PORT, PIN)    void gpio_##PORT##_pin_##PIN##_reset()
#define set_pin_state_declear(PORT, PIN) set_pin_state_declear_(PORT, PIN)
#define set_pin_state_declear_(PORT, PIN)                                      \
    void gpio_##PORT##_pin_##PIN##_set_state(bool state)
#define read_pin_declear(PORT, PIN)  read_pin_declear_(PORT, PIN)
#define read_pin_declear_(PORT, PIN) bool gpio_##PORT##_pin_##PIN##_read()

#define set_pin_function(PORT, PIN)        set_pin_function_(PORT, PIN)
#define set_pin_function_(PORT, PIN)       gpio_##PORT##_pin_##PIN##_set
#define reset_pin_function(PORT, PIN)      reset_pin_function_(PORT, PIN)
#define reset_pin_function_(PORT, PIN)     gpio_##PORT##_pin_##PIN##_reset
#define set_pin_state_function(PORT, PIN)  set_pin_state_function_(PORT, PIN)
#define set_pin_state_function_(PORT, PIN) gpio_##PORT##_pin_##PIN##_set_state
#define read_pin_function(PORT, PIN)       read_pin_function_(PORT, PIN)
#define read_pin_function_(PORT, PIN)      gpio_##PORT##_pin_##PIN##_read

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H */

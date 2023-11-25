/**
 ******************************************************************************
 * @file    bsp_ws2812b.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-10-07
 * @brief   driver of ws2812b.
 ******************************************************************************
 * @attention
 *¡¤
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "bsp_ws2812b.h"
#include "bsp_define.h"
#include "compiler.h"
#include "gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include <string.h>
#if defined(BSP_WS2812B_USE_DMA_TIM)
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_tim.h"
#endif

/* buffer setting */
#define WS2812B_BUFFER_SIZE 16
#define WS2812B_START_SIZE  6
/* timing setting (valid when use TIM and DMA) */
#define WS2812B_FREQUENCY   24000000
#define WS2812B_PERIOD      45
#define WS2812B_PULSE_HIGH  28
#define WS2812B_PULSE_LOW   10

#define wx2812b_DMA_IQR_handler                                                \
    DMAx_Channelx_IRQHandler(WS2812B_DMA, WS2812B_DMA_CHANNEL)

#define WS2812B_SET_PIN(PIN)                                                   \
    SET_PIN(WS2812B_##PIN##_GPIO_PORT, WS2812B_##PIN##_GPIO_PIN)
#define WS2812B_RESET_PIN(PIN)                                                 \
    RESET_PIN(WS2812B_##PIN##_GPIO_PORT, WS2812B_##PIN##_GPIO_PIN)

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

#if defined(__ICCARM__)
#define __nop() __asm("nop");
typedef __packed struct PWM
#else
typedef struct __attribute__((packed)) PWM
#endif
{
    uint16_t g[8], r[8], b[8];
} PWM_t;

#if defined(BSP_WS2812B_USE_DMA_TIM)
#define DMA_BUFFER_SIZE_IN_HALFWORD                                            \
    ((size_t)sizeof(DMA_buffer) / sizeof(uint16_t))

static PWM_t DMA_buffer[WS2812B_BUFFER_SIZE];
static uint32_t source_remain_amount = 0;
static led_rgb *source = NULL;
static bool is_DMA_busy = false;

static void ws2812b_DMA_init(void)
{
    NVIC_SetPriority(DMAx_Channelx_IRQn(WS2812B_DMA, WS2812B_DMA_CHANNEL),
                     WS2812B_DMA_IRQ_PRIORITY);
    NVIC_EnableIRQ(DMAx_Channelx_IRQn(WS2812B_DMA, WS2812B_DMA_CHANNEL));

    __LL_DMA_EnableClock(WS2812B_DMA);
    (void)LL_DMA_DeInit(DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL));

    LL_DMA_ConfigTransfer(
        DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL),
        LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_HIGH
            | LL_DMA_MODE_NORMAL | LL_DMA_PERIPH_NOINCREMENT
            | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_HALFWORD
            | LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_ConfigAddresses(
        DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL),
        (uint32_t)&DMA_buffer,
        (uint32_t) & (TIM_CCR(WS2812B_TIM, WS2812B_TIM_CHANNEL)),
        LL_DMA_GetDataTransferDirection(DMA(WS2812B_DMA),
                                        LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL)));
    LL_DMA_EnableIT_TC(DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL));
}

static void ws2812b_TIM_init(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

    LL_TIM_SetCounterMode(TIM(WS2812B_TIM), LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetPrescaler(TIM(WS2812B_TIM),
                        __LL_TIM_CALC_PSC(SystemCoreClock, WS2812B_FREQUENCY));
    LL_TIM_SetAutoReload(TIM(WS2812B_TIM), WS2812B_PERIOD);
    LL_TIM_SetRepetitionCounter(TIM(WS2812B_TIM), 0x00U);

    LL_TIM_OC_SetMode(TIM(WS2812B_TIM), LL_TIM_CHANNEL(WS2812B_TIM_CHANNEL),
                      LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_ConfigOutput(TIM(WS2812B_TIM),
                           LL_TIM_CHANNEL(WS2812B_TIM_CHANNEL),
                           LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);
    __LL_TIM_OC_SetCompareCH(WS2812B_TIM, WS2812B_TIM_CHANNEL, 0x00U);
    LL_TIM_OC_EnablePreload(TIM(WS2812B_TIM),
                            LL_TIM_CHANNEL(WS2812B_TIM_CHANNEL));

    __LL_TIM_EnableDMAReq_CC(WS2812B_TIM, WS2812B_TIM_CHANNEL);

    LL_TIM_CC_EnableChannel(TIM(WS2812B_TIM),
                            LL_TIM_CHANNEL(WS2812B_TIM_CHANNEL));
    LL_TIM_EnableAllOutputs(TIM(WS2812B_TIM));
    LL_TIM_EnableCounter(TIM(WS2812B_TIM));
}
#endif
/**
 * @brief  Initialize GPIO pin which WS2812B has used.
 * @param  None
 * @retval None
 */
void ws2812b_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_GPIO_CLOCK(WS2812B_DIN_GPIO_PORT));

    LL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin = LL_GPIO_PIN(WS2812B_DIN_GPIO_PIN);
#if defined(BSP_WS2812B_USE_GPIO)
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
#elif defined(BSP_WS2812B_USE_DMA_TIM)
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
#endif
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(LL_GPIO_PORT(WS2812B_DIN_GPIO_PORT), &GPIO_InitStruct);
#if defined(BSP_WS2812B_USE_DMA_TIM)
    ws2812b_DMA_init();
    ws2812b_TIM_init();
#endif
}

#if defined(BSP_WS2812B_USE_GPIO)
/**
 * @brief  To Write 0 to WS2818B.
 * @param  None
 * @retval None
 */
static void ws2812b_din_write_0(void)
{
    // clang-format off
    WS2812B_SET_PIN(DIN);
    __nop();__nop();__nop();__nop();
    WS2812B_RESET_PIN(DIN);
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();
    // clang-format on
}

/**
 * @brief  To Write 1 to WS2818B.
 * @param  None
 * @retval None
 */
static void ws2812b_din_write_1(void)
{
    // clang-format off
    WS2812B_SET_PIN(DIN);
    __nop();__nop();__nop();__nop();__nop();
    __nop();__nop();__nop();__nop();
    WS2812B_RESET_PIN(DIN);
    __nop();__nop();__nop();__nop();
    // clang-format on
}

static void WS2812B_DATA_Write_Byte(uint8_t byte)
{
    for (uint8_t mask = 0x80; mask != 0x00; mask >>= 1)
    {
        (byte & mask) ? ws2812b_din_write_1() : ws2812b_din_write_0();
    }
}

static void WS2812B_Write_24Bits(uint8_t green, uint8_t red, uint8_t blue)
{
    disable_irq();
    WS2812B_DATA_Write_Byte(green);
    WS2812B_DATA_Write_Byte(red);
    WS2812B_DATA_Write_Byte(blue);
    enable_irq();
}
#endif

#if defined(BSP_WS2812B_USE_DMA_TIM)
static void ws2812b_padding_PWM_buffer(led_rgb *rgb, PWM_t *pwm, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        for (uint32_t j = 0; j < 8; j++)
        {
            pwm[i].g[j] = ((rgb[i].g) & (0x80 >> j)) ? WS2812B_PULSE_HIGH
                                                     : WS2812B_PULSE_LOW;
            pwm[i].r[j] = ((rgb[i].r) & (0x80 >> j)) ? WS2812B_PULSE_HIGH
                                                     : WS2812B_PULSE_LOW;
            pwm[i].b[j] = ((rgb[i].b) & (0x80 >> j)) ? WS2812B_PULSE_HIGH
                                                     : WS2812B_PULSE_LOW;
        }
    }
}

void wx2812b_DMA_IQR_handler(void)
{
    if (LL_DMA_IsEnabledIT_TC(DMA(WS2812B_DMA),
                              LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL))
        && __LL_DMA_IsActiveFlag(WS2812B_DMA, WS2812B_DMA_CHANNEL, TC))
    {
        __LL_DMA_ClearFlag(WS2812B_DMA, WS2812B_DMA_CHANNEL, TC);
        LL_DMA_DisableChannel(DMA(WS2812B_DMA),
                              LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL));
        __LL_TIM_OC_SetCompareCH(WS2812B_TIM, WS2812B_TIM_CHANNEL, 0x00U);
        is_DMA_busy = false;
    }
}
#endif

void ws2812b_send_rgb(led_rgb *rgb, uint16_t count)
{
#if defined(BSP_WS2812B_USE_GPIO)
    for (uint8_t i = 0; i < count; i++)
    {
        WS2812B_Write_24Bits((rgb->g), (rgb->r), (rgb->b));
    }
#elif defined(BSP_WS2812B_USE_DMA_TIM)
    uint32_t valid_length = 0;
    if (is_DMA_busy == true)
    {
        return;
    }
    source_remain_amount = count;
    source = rgb;
    is_DMA_busy = true;
    /* padding the first part of transition */
    memset(DMA_buffer, 0, WS2812B_START_SIZE * sizeof(PWM_t));
    /* padding valid part */
    /* calulate how many led can buffer contain*/
    valid_length
        = min(source_remain_amount, WS2812B_BUFFER_SIZE - WS2812B_START_SIZE);
    ws2812b_padding_PWM_buffer(source, DMA_buffer + WS2812B_START_SIZE,
                               valid_length);
    source_remain_amount -= valid_length;
    source += valid_length;
    /* padding remain part */
    memset(DMA_buffer + WS2812B_START_SIZE + valid_length, 0x00U,
           (WS2812B_BUFFER_SIZE - WS2812B_START_SIZE - valid_length)
               * sizeof(PWM_t));
    /* start dma */
    LL_DMA_SetDataLength(DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL),
                         DMA_BUFFER_SIZE_IN_HALFWORD);
    LL_DMA_EnableChannel(DMA(WS2812B_DMA), LL_DMA_CHANNEL(WS2812B_DMA_CHANNEL));
    __LL_TIM_OC_SetCompareCH(WS2812B_TIM, WS2812B_TIM_CHANNEL, 0x00U);
#endif
}

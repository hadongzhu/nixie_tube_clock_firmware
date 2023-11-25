/**
 ******************************************************************************
 * @file    bsp_HV57708.c
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-06-07
 * @brief   Driver of HV57708.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "bsp_HV57708.h"
#include "gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "bsp_define.h"

// #define HV57708_SET_PIN(PIN)    HV57708_##PIN##_SET()
#define HV57708_SET_PIN(PIN)                                                   \
    SET_PIN(HV57708_##PIN##_GPIO_PORT, HV57708_##PIN##_GPIO_PIN)
// #define HV57708_RESET_PIN(PIN)  HV57708_##PIN##_RESET()
#define HV57708_RESET_PIN(PIN)                                                 \
    RESET_PIN(HV57708_##PIN##_GPIO_PORT, HV57708_##PIN##_GPIO_PIN)

#define UINT32_BITS (sizeof(uint32_t) * 8)

static inline void HV57708_Delay(void);

/**
 * @brief Map of HV57708. tube_num_map[n][0 - 9] means pin of corresponding
 * number, tube_num_map[n][10] means pin of dot, n means index of tube.
 */
static const uint8_t tube_num_map[4][11]
    = {{18, 20, 21, 22, 43, 44, 45, 46, 47, 48, 19},
       {13, 15, 16, 17, 49, 50, 51, 52, 53, 12, 14},
       {7, 9, 10, 11, 54, 55, 56, 57, 58, 59, 8},
       {2, 4, 5, 6, 60, 61, 62, 63, 64, 1, 3}};
/**
  * @brief  delay specially for HV57708. according to datasheet of HV57708,
            clock speed is no more than 36MHz.
  * @param  None.
  * @retval None.
  */
static inline void HV57708_Delay(void)
{
    //    __nop();
}

/**
 * @brief  Initialize GPIO pins which HV57708 has used. All pins work in OD.
 * @param  None.
 * @retval None.
 */
void HV57708_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_GPIO_CLOCK(HV57708_CLK_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_LE_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_BL_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_POL_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_DIN1_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_DIN2_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_DIN3_GPIO_PORT)
                             | LL_GPIO_CLOCK(HV57708_DIN4_GPIO_PORT));

    LL_GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_CLK_GPIO_PIN);
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_CLK_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_LE_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_LE_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_BL_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_BL_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_POL_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_POL_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_DIN1_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_DIN1_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_DIN2_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_DIN2_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_DIN3_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_DIN3_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(HV57708_DIN4_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(HV57708_DIN4_GPIO_PORT), &GPIO_InitStruct);

    HV57708_SET_PIN(POL);
    HV57708_SET_PIN(BL);

    HV57708_RESET_PIN(DIN1);
    HV57708_RESET_PIN(DIN2);
    HV57708_RESET_PIN(DIN3);
    HV57708_RESET_PIN(DIN4);

    HV57708_RESET_PIN(CLK);
    HV57708_RESET_PIN(LE);
}

/**
 * @brief  Send 64 bits to HV57708 without refreshing.
 * @param  rawdata: pointer to an array of uint32 of length 2 which contains
 *         the raw data which is going to be send to HV57708.
 * @retval None
 */
void HV57708_send_rawdata(uint32_t *rawdata)
{
    uint8_t index_4bits;
    uint32_t *radata_end = rawdata;
    uint32_t msak;
    for (rawdata += 1; rawdata >= radata_end; rawdata--)
    {
        for (msak = 0x80000000, index_4bits = 0; index_4bits < UINT32_BITS / 4;
             index_4bits++)
        {
            *rawdata &msak ? HV57708_SET_PIN(DIN4) : HV57708_RESET_PIN(DIN4);
            msak >>= 1;
            *rawdata &msak ? HV57708_SET_PIN(DIN3) : HV57708_RESET_PIN(DIN3);
            msak >>= 1;
            *rawdata &msak ? HV57708_SET_PIN(DIN2) : HV57708_RESET_PIN(DIN2);
            msak >>= 1;
            *rawdata &msak ? HV57708_SET_PIN(DIN1) : HV57708_RESET_PIN(DIN1);
            msak >>= 1;
            HV57708_Delay();
            HV57708_SET_PIN(CLK);
            HV57708_Delay();
            HV57708_RESET_PIN(CLK);
        }
    }
}

/**
 * @brief  convert numbers into raw data which can directly send to HV57708.
 *         depends on tube_num_map to map pins of HV57708
 * @param  tube_num: pointer to an array of uint8 of length 4 which contains
 *         the numbers need to be conerted.
 *         when number in tube_num < 10 : display corresponding number.
 *         when number in tube_num == 10 : display the dot.
 *         when number in tube_num > 10 : display nothing.
 *         rawdata: pointer to an array of uint32 of length 2 which is used
 *         to save the raw data.
 * @retval pointer to rawdata
 */
uint32_t *HV57708_convert_into_rawdata(uint8_t tube_num[], uint32_t rawdata[])
{
    uint8_t tube_num_index, bit_index;
    rawdata[0] = 0xFFFFFFFF;
    rawdata[1] = 0xFFFFFFFF;
    for (tube_num_index = 0; tube_num_index < 4; tube_num_index++)
    {
        if (tube_num[tube_num_index] <= 10)
        {
            bit_index
                = tube_num_map[tube_num_index][tube_num[tube_num_index]] - 1;
            rawdata[bit_index / UINT32_BITS]
                &= ~(0x00000001 << (bit_index % 32));
        }
    }
    return rawdata;
}

/**
 * @brief  Refresh HV57708 display.
 * @param  None
 * @retval None
 */
void HV57708_output_data(void)
{
    HV57708_SET_PIN(LE);
    HV57708_Delay();
    HV57708_RESET_PIN(LE);
    HV57708_Delay();
}

void HV57705_send_number(uint8_t number[], uint16_t amount)
{
    uint32_t rawdata[2];
    HV57708_convert_into_rawdata(number, rawdata);
    HV57708_send_rawdata(rawdata);
    HV57708_output_data();
}
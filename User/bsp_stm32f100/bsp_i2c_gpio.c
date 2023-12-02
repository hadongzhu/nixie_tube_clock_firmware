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

#include "bsp_i2c_gpio.h"
#include "bsp_define.h"
#include "gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_i2c.h"
#include "stm32f1xx_ll_rcc.h"
#include <stdbool.h>

#if defined USE_HARD_I2C
#define DS3231_I2Cx    I2C1
#define DS3231_I2C_CLK RCC_APB1Periph_I2C1
#define I2C_SPEEDCLOCK 400000
#define I2C_DUTYCYCLE  LL_I2C_DUTYCYCLE_2
#elif defined USE_SOFT_I2C
// #define I2C_SET_PIN(PIN)        I2C_##PIN##_SET()
#define I2C_SET_PIN(PIN) SET_PIN(I2C_##PIN##_GPIO_PORT, I2C_##PIN##_GPIO_PIN)
// #define I2C_RESET_PIN(PIN)      I2C_##PIN##_RESET()
#define I2C_RESET_PIN(PIN)                                                     \
    RESET_PIN(I2C_##PIN##_GPIO_PORT, I2C_##PIN##_GPIO_PIN)
// #define I2C_READ_PIN(PIN)       I2C_##PIN##_READ()
#define I2C_READ_PIN(PIN) READ_PIN(I2C_##PIN##_GPIO_PORT, I2C_##PIN##_GPIO_PIN)

static void I2C_start(void);
static void I2C_stop(void);
static void I2C_send_byte(uint8_t _ucByte);
static uint8_t I2C_read_byte(void);
static uint8_t I2C_wait_ack(void);
static void I2C_ack(void);
static void I2C_nack(void);

#define __I2C_wait_ack()                                                       \
    do                                                                         \
    {                                                                          \
        if (I2C_wait_ack() != 0)                                               \
        {                                                                      \
            I2C_stop();                                                        \
            return 1;                                                          \
        }                                                                      \
    }                                                                          \
    while (0)

#endif

/**
 * @brief  Initialize I2C GPIO.
 * @param  None.
 * @retval None.
 */
void I2C_init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB2_GRP1_EnableClock(LL_GPIO_CLOCK(I2C_SCL_GPIO_PORT)
                             | LL_GPIO_CLOCK(I2C_SDA_GPIO_PORT));

    GPIO_InitStruct.Pin = LL_GPIO_PIN(I2C_SCL_GPIO_PIN);
#if defined(USE_SOFT_I2C)
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
#elif defined(USE_HARD_I2C)
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
#endif
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(LL_GPIO_PORT(I2C_SCL_GPIO_PORT), &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN(I2C_SDA_GPIO_PIN);
    LL_GPIO_Init(LL_GPIO_PORT(I2C_SDA_GPIO_PORT), &GPIO_InitStruct);

#if defined USE_HARD_I2C
    LL_RCC_ClocksTypeDef rcc_clocks;
    /* Enable the peripheral clock for I2C1 */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
    /* Disable I2C1 prior modifying configuration registers */
    LL_I2C_Disable(I2C1);
    /* Retrieve Clock frequencies */
    LL_RCC_GetSystemClocksFreq(&rcc_clocks);
    /* Configure the SCL Clock Speed */
    LL_I2C_ConfigSpeed(I2C1, rcc_clocks.PCLK1_Frequency, I2C_SPEEDCLOCK,
                       I2C_DUTYCYCLE);
    LL_I2C_Enable(I2C1);
#endif

#if defined USE_SOFT_I2C
    I2C_stop();
#endif
}

#if defined USE_SOFT_I2C

/**
 * @brief  I2C delay. To meet the I2C timing requirements.
 * @param  None.
 * @retval None.
 */
static void I2C_delay(void)
{
    for (volatile uint8_t i = 0; i < 8; i++)
        ;
}

/**
 * @brief  I2C start signal.
 * @param  None.
 * @retval None.
 */
static void I2C_start(void)
{
    I2C_SET_PIN(SDA);
    I2C_SET_PIN(SCL);
    I2C_delay();
    I2C_RESET_PIN(SDA);
    I2C_delay();
    I2C_RESET_PIN(SCL);
    I2C_delay();
}

/**
 * @brief  I2C stop signal.
 * @param  None.
 * @retval None.
 */
static void I2C_stop(void)
{
    I2C_RESET_PIN(SDA);
    I2C_SET_PIN(SCL);
    I2C_delay();
    I2C_SET_PIN(SDA);
    I2C_delay();
}

/**
 * @brief  I2C send one byte.
 * @param  None.
 * @retval None.
 */
static void I2C_send_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        (byte & (0x80 >> i)) ? I2C_SET_PIN(SDA) : I2C_RESET_PIN(SDA);
        I2C_delay();
        I2C_SET_PIN(SCL);
        I2C_delay();
        I2C_RESET_PIN(SCL);
        I2C_delay();
    }
}

/**
 * @brief  I2C read one byte.
 * @param  None.
 * @retval None.
 */
static uint8_t I2C_read_byte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        byte <<= 1;
        I2C_SET_PIN(SCL);
        I2C_delay();
        byte |= I2C_READ_PIN(SDA) ? 0x01U : 0x00U;
        I2C_RESET_PIN(SCL);
        I2C_delay();
    }
    return byte;
}

/**
 * @brief  I2C wait ack.
 * @param  None.
 * @retval None.
 */
static uint8_t I2C_wait_ack(void)
{
    bool ret = false;
    I2C_SET_PIN(SDA);
    I2C_delay();
    I2C_SET_PIN(SCL);
    I2C_delay();
    ret = I2C_READ_PIN(SDA) ? 0x01U : 0x00U;
    I2C_RESET_PIN(SCL);
    I2C_delay();
    return ret;
}

/**
 * @brief  I2C ack.
 * @param  None.
 * @retval None.
 */
static void I2C_ack(void)
{
    I2C_RESET_PIN(SDA);
    I2C_delay();
    I2C_SET_PIN(SCL);
    I2C_delay();
    I2C_RESET_PIN(SCL);
    I2C_delay();
    I2C_SET_PIN(SDA);
}

/**
 * @brief  I2C nack.
 * @param  None.
 * @retval None.
 */
static void I2C_nack(void)
{
    I2C_SET_PIN(SDA);
    I2C_delay();
    I2C_SET_PIN(SCL);
    I2C_delay();
    I2C_RESET_PIN(SCL);
    I2C_delay();
}

#endif

/**
 * @brief  I2C check device.
 * @param  None.
 * @retval None.
 */
uint8_t I2C_check_device(uint8_t device_address)
{
#if defined USE_HARD_I2C

    return 0;
#elif defined USE_SOFT_I2C
    uint8_t ack;
    if (I2C_READ_PIN(SDA) && I2C_READ_PIN(SCL))
    {
        I2C_start();
        I2C_send_byte(device_address | I2C_WR);
        ack = I2C_wait_ack();
        I2C_stop();
        return ack;
    }
    return 1;
#endif
}

/**
 * @brief  I2C read multiple bytes.
 * @param  device_address: device address.
 * @param  reg_address: register address.
 * @param  length: read length.
 * @param  buffer: read buffer.
 * @retval function status.
 */
uint8_t I2C_read_multiple_bytes(uint8_t device_address, uint8_t reg_address,
                                uint8_t length, uint8_t *buffer)
{
#if defined USE_HARD_I2C
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
    LL_I2C_GenerateStartCondition(I2C1);
    while (!LL_I2C_IsActiveFlag_SB(I2C1))
    {};
    LL_I2C_TransmitData8(I2C1, device_address | I2C_WR);
    while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
    {};
    LL_I2C_ClearFlag_ADDR(I2C1);
    if (LL_I2C_IsActiveFlag_TXE(I2C1))
    {
        LL_I2C_TransmitData8(I2C1, reg_address);
    }
    // LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
    LL_I2C_GenerateStartCondition(I2C1);
    while (!LL_I2C_IsActiveFlag_SB(I2C1))
    {};
    LL_I2C_TransmitData8(I2C1, device_address | I2C_RD);
    while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
    {};
    return 0;
#elif defined USE_SOFT_I2C
    I2C_start();
    I2C_send_byte(device_address | I2C_WR);
    __I2C_wait_ack();
    I2C_send_byte(reg_address);
    __I2C_wait_ack();
    I2C_start();
    I2C_send_byte(device_address | I2C_RD);
    __I2C_wait_ack();
    for (uint16_t i = 0; i < length; i++)
    {
        buffer[i] = I2C_read_byte();
        i != length - 1 ? I2C_ack() : I2C_nack();
    }
    I2C_stop();
    return 0;
#endif
}

/**
 * @brief  I2C write multiple bytes.
 * @param  device_address: device address.
 * @param  reg_address: register address.
 * @param  length: write length.
 * @param  buffer: write buffer.
 * @retval function status.
 */
uint8_t I2C_write_multiple_bytes(uint8_t device_address, uint8_t reg_address,
                                 uint8_t length, uint8_t *buffer)
{
#if defined USE_HARD_I2C
    LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);
    LL_I2C_GenerateStartCondition(I2C1);
    while (!LL_I2C_IsActiveFlag_SB(I2C1))
    {};
    LL_I2C_TransmitData8(I2C1, device_address | I2C_WR);
    while (!LL_I2C_IsActiveFlag_ADDR(I2C1))
    {};
    LL_I2C_ClearFlag_ADDR(I2C1);
    while (!LL_I2C_IsActiveFlag_TXE(I2C1))
    {};
    LL_I2C_TransmitData8(I2C1, reg_address);
    while (length > 0)
    {
        if (LL_I2C_IsActiveFlag_TXE(I2C1))
        {
            LL_I2C_TransmitData8(I2C1, (*buffer++));
            length--;
        }
    }
    LL_I2C_GenerateStopCondition(I2C1);

    return 0;
#elif defined USE_SOFT_I2C
    I2C_start();
    I2C_send_byte(device_address | I2C_WR);
    __I2C_wait_ack();
    I2C_send_byte(reg_address);
    __I2C_wait_ack();
    for (uint16_t i = 0; i < length; i++)
    {
        I2C_send_byte(buffer[i]);
        __I2C_wait_ack();
    }
    I2C_stop();
    return 0;
#endif
}

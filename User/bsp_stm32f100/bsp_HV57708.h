/**
 ******************************************************************************
 * @file    bsp_HV57708.h
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

#ifndef __BSP_HV57708_H
#define __BSP_HV57708_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void HV57708_init(void);
void HV57708_send_rawdata(uint32_t *rawdata);
uint32_t *HV57708_convert_into_rawdata(uint8_t tube_num[], uint32_t rawdata[]);
void HV57708_output_data(void);
void HV57705_send_number(uint8_t number[], uint16_t amount);

#ifdef __cplusplus
}
#endif

#endif /* __HV57708_H */

/**
 ******************************************************************************
 * @file    utils.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide utils.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#include "utils.h"

uint32_t utils::number_breath_calculate(int32_t number, uint32_t min,
                                        uint32_t max, uint32_t step,
                                        utils::breath_direction &direction)
{
    if (number >= max)
    {
        direction = utils::breath_direction::down;
        number = max;
    }
    else if (number <= min)
    {
        direction = utils::breath_direction::up;
        number = min;
    }
    number += (int32_t)direction * step;
    if (number > max)
    {
        number = max;
    }
    else if (number < min)
    {
        number = min;
    }
    return number;
}

int32_t utils::number_change_with_limitation(int32_t origin_number,
                                             int32_t volume, int32_t min,
                                             int32_t max)
{
    if (origin_number == max && volume > 0)
    {
        return min;
    }
    else if (origin_number == min && volume < 0)
    {
        return max;
    }
    else if (origin_number + volume > max)
    {
        return max;
    }
    else if (origin_number + volume < min)
    {
        return min;
    }
    else
    {
        return origin_number + volume;
    }
}

uint32_t utils_number_breath_calculate(int32_t number, uint32_t min,
                                       uint32_t max, uint32_t step,
                                       utils_breath_direction *direction)
{
    return utils::number_breath_calculate(number, min, max, step, *direction);
}
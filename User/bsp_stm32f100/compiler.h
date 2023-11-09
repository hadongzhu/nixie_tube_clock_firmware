/**
 ******************************************************************************
 * @file    compiler.h
 * @author  hadongzhu
 * @version V1.0.0
 * @date    2023-09-06
 * @brief   provide compiler tools.
 ******************************************************************************
 * @attention
 *
 * Powered by Hadongzhu
 ******************************************************************************
 */

#ifndef __COMPILER_H
#define __COMPILER_H
#if defined(__ARMCC_VERSION)                                                   \
    && (__ARMCC_VERSION >= 6010050) /* ARM Compiler V6 */
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#ifndef __packed
#define __packed __attribute__((packed))
#endif
#elif defined(__GNUC__) && !defined(__CC_ARM) /* GNU Compiler */
#ifndef __weak
#define __weak __attribute__((weak))
#endif /* __weak */
#ifndef __packed
#define __packed __attribute__((__packed__))
#endif /* __packed */
#endif /* __GNUC__ */

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static __inline
#endif

#include "stm32f1xx_hal.h"

__STATIC_INLINE void enable_irq(void)
{
    __set_PRIMASK(0);
}

__STATIC_INLINE void disable_irq(void)
{
    __set_PRIMASK(1);
}
#endif /* __COMPILER_H */

/**
 * @file freertos_utility.h
 *
 * @brief FreeRTOS related.
 *
 * @date 06/2023
 */

#ifndef __FREERTOS_UTILITY_H__
#define __FREERTOS_UTILITY_H__

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"

/*****************************************
 * Public Macros
 *****************************************/

/** Macro to remove compilation warnings in Tasks */
#define UNUSED_ARG(x) (void) x

#define RTOS_TICKS_TO_US (1e+6 / osKernelGetTickFreq()) // (1e+6 us / s) / (1 ticks / s)

#define get_rtos_ticks(ticks) (osKernelGetTickCount() - ticks)
#define reset_rtos_ticks(ticks) (ticks = osKernelGetTickCount())

#define get_rtos_time_us(time) (osKernelGetTickCount() * RTOS_TICKS_TO_US - time)
#define reset_rtos_time_us(time) (time = osKernelGetTickCount() * RTOS_TICKS_TO_US)

/*****************************************
 * Public Functions Prototypes
 *****************************************/

/**
 * @brief Initializes hardware and peripherals.
 */
void freertos_utility_init(void);

/**
 * @brief Initializes FreeRTOS
 *
 * @note  Defined by cube.
 */
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  Provides accurate delay (in milliseconds) based
 *         on variable incremented.
 *
 * @param ms Specifies the delay time length, in milliseconds.
 */
void freertos_utility_sleep_ms(uint32_t ms);

#endif // __FREERTOS_UTILITY_H__

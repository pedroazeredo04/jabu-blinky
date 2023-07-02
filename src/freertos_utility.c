/**
 * @file freertos_utility.c
 *
 * @brief FreeRTOS related.
 *
 * @date 06/2023
 */

#include <stdint.h>

#include "freertos_utility.h"

/*****************************************
 * Private Variables
 *****************************************/

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

void freertos_utility_init(void) {
    osKernelInitialize();
    MX_FREERTOS_Init();
    osKernelStart();
}

void freertos_utility_sleep_ms(uint32_t ms) {
    osDelay(ms);
}

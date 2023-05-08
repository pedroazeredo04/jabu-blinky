/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/*****************************************
 * Private Functions Definitions
 *****************************************/

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

void SystemClock_Config(void);

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    // Initialize HAL and GPIO
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    // Initialize FreeRTOS
    osKernelInitialize();
    MX_FREERTOS_Init();
    osKernelStart();

    // Empty infinity loop
    for (;;)
        ;
}

void StartDefaultTask(void* argument) {
    for (;;) {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        osDelay(500);
    }
}

void StartTask02(void* argument) {
    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);
        osDelay(550);
    }
}

void StartTask03(void* argument) {
    for (;;) {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
        osDelay(600);
    }
}

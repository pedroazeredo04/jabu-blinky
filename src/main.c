/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "main.h"
#include "freertos_utility.h"

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    mcu_init();              // Initialize HAL and GPIO
    freertos_utility_init(); // Initialize FreeRTOS

    for (;;);      // Empty infinity loop
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

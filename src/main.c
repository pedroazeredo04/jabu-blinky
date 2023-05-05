/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 1500

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    mcu_init();
    led_toggle();
    led_toggle();
    MX_FREERTOS_Init();
    osKernelStart();

    for (;;);
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    osDelay(500);
  }
  /* USER CODE END StartDefaultTask */
}

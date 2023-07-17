/**
 * @file buzzer_task.c
 *
 * @brief Buzzer Task
 */

#include "button_task.h"
#include "freertos_utility.h"
#include "leds.h"

/*****************************************
 * Private Variables
 *****************************************/

static button_handler_t button = {
    .port = GPIOB,
    .pin = GPIO_PIN_0,
};

extern osMessageQueueId_t ButtonReadingHandle;

/*****************************************
 * Public Functions Bodies Definitions
 *****************************************/

bool button_is_pressed(button_handler_t* p_button) {
    if (HAL_GPIO_ReadPin(p_button->port, p_button->pin) == GPIO_PIN_RESET) {
        while (HAL_GPIO_ReadPin(p_button->port, p_button->pin) == GPIO_PIN_RESET) {
            freertos_utility_sleep_ms(1);
        }

        return true;
    }

    return false;
}

/*****************************************
 * Tasks Bodies Definitions
 *****************************************/

void readButtonTask(void* argument) {
    UNUSED_ARG(argument);
    uint16_t is_to_toggle_led;

    for (;;) {
        is_to_toggle_led = (uint16_t) button_is_pressed(&button);

        if (is_to_toggle_led) {
            toggle_extern_led2();
        }

        osMessageQueuePut(ButtonReadingHandle, &is_to_toggle_led, 0U, 0U);
    }
}

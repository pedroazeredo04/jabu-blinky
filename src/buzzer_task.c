/**
 * @file buzzer_task.c
 *
 * @brief Buzzer Task
 */

#include "buzzer_task.h"
#include "gpio.h"
#include "freertos_utility.h"
#include "leds.h"

/*****************************************
 * Private Variables
 *****************************************/

extern osMessageQueueId_t ButtonReadingHandle;

/*****************************************
 * Tasks Bodies Definitions
 *****************************************/

void BuzzerTask(void* argument) {
    UNUSED_ARG(argument);

    uint16_t message;
    osStatus_t status;

    for (;;) {
        status = osMessageQueueGet(ButtonReadingHandle, &message, NULL, 0U);

        if (status == osOK) {
            if (message == 1) {
                toggle_board_led();
            }
        }
    }
}

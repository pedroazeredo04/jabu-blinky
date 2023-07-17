/**
 * @file button_task.h
 *
 * @brief Buzzer Task
 */

#ifndef __BUTTON_TASK_H__
#define __BUTTON_TASK_H__

#include <stdbool.h>

#include "gpio.h"

/*****************************************
 * Public Types
 *****************************************/

/**
 * @brief Button handler structure.
 */
typedef struct __attribute__((packed)) button_handler {
    GPIO_TypeDef* port;
    uint16_t      pin;
} button_handler_t;

/*****************************************
 * Public Function Prototypes
 *****************************************/

bool button_is_pressed(button_handler_t* p_button);

#endif // __BUTTON_TASK_H__

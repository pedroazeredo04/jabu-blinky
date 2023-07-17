/**
 * @file leds.h
 *
 * @brief LED Task
 */

#ifndef __LEDS_H__
#define __LEDS_H__

#include "gpio.h"

/*****************************************
 * Public Function Prototypes
 *****************************************/

void toggle_board_led();

void toggle_extern_led();

void toggle_extern_led2();

#endif // __LEDS_H__

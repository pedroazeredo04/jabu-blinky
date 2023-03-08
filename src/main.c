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

    for (;;) {
        uart_transmit("Hello World! \n\r");
        mcu_sleep(LED_TOGGLE_DELAY_MS);
    }
}

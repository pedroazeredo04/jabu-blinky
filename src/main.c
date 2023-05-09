/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"
#include "utils.h"
#include "rgb.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

#define LED_TOGGLE_DELAY_MS 500

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    mcu_init();
    led_pwm_init();

    uint16_t red = 0;
    uint16_t green = 0;
    uint16_t blue = 0;
    uint16_t adc_value = 0;
    rgb_t rgb = {0, 0, 0};

    for (;;) {
        adc_value = get_adc_value();

        rgb = hue_to_rgb(adc_value);

        led_pwm_set(rgb.r, rgb.g, rgb.b);
    }
}

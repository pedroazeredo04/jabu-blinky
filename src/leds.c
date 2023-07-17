#include "leds.h"

void toggle_board_led() {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

void toggle_extern_led() {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
}

void toggle_extern_led2() {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
}

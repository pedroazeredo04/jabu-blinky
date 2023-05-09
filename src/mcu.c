/**
 * @file mcu.c
 *
 * @brief MCU related
 */

#include <stdint.h>

#include "mcu.h"

#include "gpio.h"
#include "main.h"
#include "tim.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/

void mcu_init(void) {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();

    MX_ADC1_Init();
}

void mcu_sleep(uint32_t ms) {
    HAL_Delay(ms);
}

void led_pwm_init(void) {
    MX_TIM1_Init();

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

uint16_t get_adc_value() {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

void led_pwm_set(uint16_t red, uint16_t green, uint16_t blue) {
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, red);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, green);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, blue);
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

/**
 * @file mcu.c
 *
 * @brief MCU related
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "mcu.h"

#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "main.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/

void mcu_init(void) {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART1_UART_Init();
}

void mcu_sleep(uint32_t ms) {
    HAL_Delay(ms);
}

void uart_transmit(char* data) {
    char uart_buf[100];
    int uart_buf_len = sprintf(uart_buf, "Hello, wowrld!\r\n");

    HAL_UART_Transmit(&huart1, (uint8_t*) uart_buf, uart_buf_len, 100);
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

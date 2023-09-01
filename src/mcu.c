/**
 * @file mcu.c
 *
 * @brief MCU related
 */

#include <stdint.h>

#include "mcu.h"

#include "gpio.h"
#include "main.h"
#include "i2c.h"
#include "stm32f4xx_hal_i2c.h"

/*****************************************
 * Public Function Body Definitions
 *****************************************/

void mcu_init(void) {
    HAL_Init();

    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void mcu_sleep(uint32_t ms) {
    HAL_Delay(ms);
}

void led_toggle(void) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
}

uint8_t readReg(uint8_t address, uint8_t reg, uint16_t timeout) {
    uint8_t val;

    if ((HAL_I2C_Mem_Read(&hi2c1, address, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, timeout)) != HAL_OK) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        return 0;
    }

    return val;
}

void checkMagnetPresence(void) {
    // This function runs in the setup() and it locks the MCU until the magnet is not positioned properly

    uint8_t magnetStatus = 0; // variable to store the magnet status

    while ((magnetStatus & 32) != 32) {
        // while the magnet is not adjusted to the proper distance - 32: MD = 1
        magnetStatus = 0;                                                                   // reset reading,
        HAL_I2C_Mem_Read(&hi2c1, 0x36, 0x0B, I2C_MEMADD_SIZE_8BIT, &magnetStatus, 1, 1000); // read the status register (fig 21
    }
}

/**
 * @file mcu.h
 *
 * @brief MCU related
 */

#ifndef __MCU_H__
#define __MCU_H__

#include <stdint.h>

/*****************************************
 * Public Function Prototypes
 *****************************************/

/**
 * @brief Initializes MCU and some peripherals.
 */
void mcu_init(void);

/**
 * @brief Initializes System Clock.
 * @note  Defined by cube.
 */
void SystemClock_Config(void);

/**
 * @brief Reads I2C slave
 *
 * @param adress I2C perifherical adress
 * @param reg Adress to register to be read
 * @param timeout Time to wait
 *
 * @return 0, if failed or number pointed by red, if succeeded
 */
uint8_t readReg(uint8_t address, uint8_t reg, uint16_t timeout);

void checkMagnetPresence(void);

#endif // __MCU_H__

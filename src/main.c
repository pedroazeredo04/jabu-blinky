/**
 * @file main.c
 *
 * @brief Main function
 */

#include "mcu.h"

/*****************************************
 * Private Constant Definitions
 *****************************************/

static uint16_t angle = 1;

#define TIMEOUT 1500
static uint8_t I2CAdress = 0x36;
static uint8_t I2CRawAngleRegisterHighByte = 0x0C;
static uint8_t I2CRawAngleRegisterLowByte = 0x0D;
static uint8_t lowbyte = 1;
static uint8_t highbyte = 1;

/*****************************************
 * Main Function
 *****************************************/

int main(void) {
    mcu_init();
    checkMagnetPresence();

    for (;;) {
        lowbyte = readReg(I2CAdress, I2CRawAngleRegisterLowByte, TIMEOUT);
        highbyte = readReg(I2CAdress, I2CRawAngleRegisterHighByte, TIMEOUT);

        angle = (highbyte << 8) | (lowbyte);
    }
}

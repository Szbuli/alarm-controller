/*
 * eeprom.h
 *
 *  Created on: 2017. m�rc. 31.
 *      Author: Szbuli
 */

#ifndef EEPROM_EEPROM_H_
#define EEPROM_EEPROM_H_

#define EEPROM_I2C hi2c1
#define EEPROM_I2C_ADDRESS 0b1010000

#include "stm32f1xx.h"
#include "cmsis_os.h"

void pingEEPROM();
uint8_t readByteEEPROM(uint16_t address);
uint64_t read8ByteEEPROM(uint16_t address);
HAL_StatusTypeDef writeByteEEPROM(uint16_t address, uint8_t data);
HAL_StatusTypeDef write8ByteEEPROM(uint16_t address, uint64_t data);
void writeProtectedByteEEPROM(uint16_t address, uint8_t data);

#endif /* EEPROM_EEPROM_H_ */

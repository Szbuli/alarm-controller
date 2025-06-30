/*
 * eeprom.c
 *
 *  Created on: 2017.03.31.
 *      Author: Szbuli
 */

#include "eeprom.h"
#include "i2c.h"

void pingEEPROM() {
	uint8_t buffer = 0;
	HAL_I2C_Master_Transmit(&EEPROM_I2C, ((uint16_t) EEPROM_I2C_ADDRESS << 1), &buffer, 1, (uint32_t) 100);
}

uint8_t readByteEEPROM(uint16_t address) {

	assert_param(address < 8192);

	uint8_t buffer[2] = { (address >> 8) & 0xFF, address & 0xFF };
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&EEPROM_I2C, ((uint16_t) EEPROM_I2C_ADDRESS << 1), buffer, 2, (uint32_t) 100);
	if (status != HAL_OK) {
		return -1;
	}

	uint8_t read = -1;
	osDelay(10);
	status = HAL_I2C_Master_Receive(&EEPROM_I2C, ((uint16_t) EEPROM_I2C_ADDRESS << 1), &read, 1, 100);
	if (status != HAL_OK) {
		return -1;
	}
	return read;
}

HAL_StatusTypeDef writeByteEEPROM(uint16_t address, uint8_t data) {
	assert_param(address < 8192);

	uint8_t buffer[3] = { (address >> 8) & 0xFF, address & 0xFF, data };
	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&EEPROM_I2C, ((uint16_t) EEPROM_I2C_ADDRESS << 1), buffer, 3, (uint32_t) 100);

	return status;
}

void writeProtectedByteEEPROM(uint16_t address, uint8_t data) {
	assert_param(address < 8192);

	if (address == 0) {
		return;
	}

	writeByteEEPROM(address, data);
}

uint64_t read8ByteEEPROM(uint16_t address) {
	assert_param(address < 8192);

	uint64_t result = 0;
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t byte = readByteEEPROM(address + i);
		result |= ((uint64_t) byte << (8 * (7 - i)));
	}
	return result;
}

HAL_StatusTypeDef write8ByteEEPROM(uint16_t address, uint64_t data) {
	assert_param(address < 8192);

	HAL_StatusTypeDef status = HAL_OK;
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t byte = (data >> (8 * (7 - i))) & 0xFF;
		status = writeByteEEPROM(address + i, byte);
		if (status != HAL_OK) {
			return status;
		}
	}
	return status;
}

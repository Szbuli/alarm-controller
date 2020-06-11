/*
 * home_config.c
 *
 *  Created on: 2017. máj. 23.
 *      Author: Szbuli
 */

#include "home_config.h"
#include "can_support.h"
#include "eeprom.h"
#include "can-ids.h"
#include "gpio.h"

HomeConfig homeConfig;

void blinkRapid() {
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
	osDelay(50);
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
	osDelay(50);
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
	osDelay(50);
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
	osDelay(50);
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
}

void checkAndDoFactoryResetIfNeeded() {
	GPIO_PinState factoryResetState = HAL_GPIO_ReadPin(FACTORY_RESET_GPIO_Port, FACTORY_RESET_Pin);
	if (factoryResetState == GPIO_PIN_SET) {
		return;
	}
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
	osDelay(5000);
	factoryResetState = HAL_GPIO_ReadPin(FACTORY_RESET_GPIO_Port, FACTORY_RESET_Pin);
	if (factoryResetState == GPIO_PIN_SET) {
		HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_RESET);
		return;
	}
	blinkRapid();
	HAL_GPIO_WritePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin, GPIO_PIN_SET);
	factoryReset();
	blinkRapid();
	HAL_NVIC_SystemReset();
}

void configureTamper(uint8_t state) {
	HAL_StatusTypeDef status = writeByteEEPROM(ADDRESS_TAMPER, state);

	osDelay(100);
	HAL_NVIC_SystemReset();
}

void configureSensor(uint16_t typeId, uint8_t state) {
	HAL_StatusTypeDef status;
	if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_1) {
		status = writeByteEEPROM(ADDRESS_ALARM_1, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_2) {
		status = writeByteEEPROM(ADDRESS_ALARM_2, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_3) {
		status = writeByteEEPROM(ADDRESS_ALARM_3, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_4) {
		status = writeByteEEPROM(ADDRESS_ALARM_4, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_5) {
		status = writeByteEEPROM(ADDRESS_ALARM_5, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_6) {
		status = writeByteEEPROM(ADDRESS_ALARM_6, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_7) {
		status = writeByteEEPROM(ADDRESS_ALARM_7, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_8) {
		status = writeByteEEPROM(ADDRESS_ALARM_8, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_9) {
		status = writeByteEEPROM(ADDRESS_ALARM_9, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_10) {
		status = writeByteEEPROM(ADDRESS_ALARM_10, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_11) {
		status = writeByteEEPROM(ADDRESS_ALARM_11, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_12) {
		status = writeByteEEPROM(ADDRESS_ALARM_12, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_13) {
		status = writeByteEEPROM(ADDRESS_ALARM_13, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_14) {
		status = writeByteEEPROM(ADDRESS_ALARM_14, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_15) {
		status = writeByteEEPROM(ADDRESS_ALARM_15, state);
	}

	osDelay(100);
	HAL_NVIC_SystemReset();
}

void readConfigOnStartup() {
	homeConfig.deviceId = readByteEEPROM(ADDRESS_DEVICE_ID_PART_0) | readByteEEPROM(ADDRESS_DEVICE_ID_PART_1) << 8;
	homeConfig.tamper = readByteEEPROM(ADDRESS_TAMPER);
	homeConfig.alarm_1 = readByteEEPROM(ADDRESS_ALARM_1);
	homeConfig.alarm_2 = readByteEEPROM(ADDRESS_ALARM_2);
	homeConfig.alarm_3 = readByteEEPROM(ADDRESS_ALARM_3);
	homeConfig.alarm_4 = readByteEEPROM(ADDRESS_ALARM_4);
	homeConfig.alarm_5 = readByteEEPROM(ADDRESS_ALARM_5);
	homeConfig.alarm_6 = readByteEEPROM(ADDRESS_ALARM_6);
	homeConfig.alarm_7 = readByteEEPROM(ADDRESS_ALARM_7);
	homeConfig.alarm_8 = readByteEEPROM(ADDRESS_ALARM_8);
	homeConfig.alarm_9 = readByteEEPROM(ADDRESS_ALARM_9);
	homeConfig.alarm_10 = readByteEEPROM(ADDRESS_ALARM_10);
	homeConfig.alarm_11 = readByteEEPROM(ADDRESS_ALARM_11);
	homeConfig.alarm_12 = readByteEEPROM(ADDRESS_ALARM_12);
	homeConfig.alarm_13 = readByteEEPROM(ADDRESS_ALARM_13);
	homeConfig.alarm_14 = readByteEEPROM(ADDRESS_ALARM_14);
	homeConfig.alarm_15 = readByteEEPROM(ADDRESS_ALARM_15);
}

void factoryReset() {
	HAL_StatusTypeDef status = writeByteEEPROM(ADDRESS_DEVICE_ID_PART_0, 0xFF);
	status = writeByteEEPROM(ADDRESS_DEVICE_ID_PART_1, 0xFF);
	status = writeByteEEPROM(ADDRESS_TAMPER, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_1, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_2, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_3, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_4, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_5, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_6, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_7, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_8, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_9, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_10, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_11, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_12, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_13, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_14, 0);
	status = writeByteEEPROM(ADDRESS_ALARM_15, 0);
}

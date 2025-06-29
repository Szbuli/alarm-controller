/*
 * home_config.c
 *
 *  Created on: 2017.05.23.
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
	writeByteEEPROM(ADDRESS_TAMPER, state);

	osDelay(100);
	HAL_NVIC_SystemReset();
}

void configureHeartbeat(uint8_t state) {
	writeByteEEPROM(ADDRESS_HEARTBEAT, state);

	osDelay(100);
	HAL_NVIC_SystemReset();
}

void configureSensor(uint16_t typeId, uint8_t state) {
	if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_1) {
		writeByteEEPROM(ADDRESS_ALARM_1, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_2) {
		writeByteEEPROM(ADDRESS_ALARM_2, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_3) {
		writeByteEEPROM(ADDRESS_ALARM_3, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_4) {
		writeByteEEPROM(ADDRESS_ALARM_4, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_5) {
		writeByteEEPROM(ADDRESS_ALARM_5, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_6) {
		writeByteEEPROM(ADDRESS_ALARM_6, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_7) {
		writeByteEEPROM(ADDRESS_ALARM_7, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_8) {
		writeByteEEPROM(ADDRESS_ALARM_8, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_9) {
		writeByteEEPROM(ADDRESS_ALARM_9, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_10) {
		writeByteEEPROM(ADDRESS_ALARM_10, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_11) {
		writeByteEEPROM(ADDRESS_ALARM_11, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_12) {
		writeByteEEPROM(ADDRESS_ALARM_12, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_13) {
		writeByteEEPROM(ADDRESS_ALARM_13, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_14) {
		writeByteEEPROM(ADDRESS_ALARM_14, state);
	} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_15) {
		writeByteEEPROM(ADDRESS_ALARM_15, state);
	}

	osDelay(100);
	HAL_NVIC_SystemReset();
}

void configureCounterCurrentValue(uint16_t typeId, uint64_t counterValue) {
	if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_1) {
		write8ByteEEPROM(ADDRESS_COUNTER_1, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_2) {
		write8ByteEEPROM(ADDRESS_COUNTER_2, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_3) {
		write8ByteEEPROM(ADDRESS_COUNTER_3, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_4) {
		write8ByteEEPROM(ADDRESS_COUNTER_4, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_5) {
		write8ByteEEPROM(ADDRESS_COUNTER_5, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_6) {
		write8ByteEEPROM(ADDRESS_COUNTER_6, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_7) {
		write8ByteEEPROM(ADDRESS_COUNTER_7, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_8) {
		write8ByteEEPROM(ADDRESS_COUNTER_8, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_9) {
		write8ByteEEPROM(ADDRESS_COUNTER_9, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_10) {
		write8ByteEEPROM(ADDRESS_COUNTER_10, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_11) {
		write8ByteEEPROM(ADDRESS_COUNTER_11, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_12) {
		write8ByteEEPROM(ADDRESS_COUNTER_12, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_13) {
		write8ByteEEPROM(ADDRESS_COUNTER_13, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_14) {
		write8ByteEEPROM(ADDRESS_COUNTER_14, counterValue);
	} else if (typeId == ALARM_CONTROLLER_COUNTER_SENSOR_STATE_15) {
		write8ByteEEPROM(ADDRESS_COUNTER_15, counterValue);
	}
}

void publishSensorConfig(uint16_t topicId, uint8_t type) {
	uint8_t config[] = { type };
	putCanMessageToQueue(topicId, config, 1, CAN_RTR_DATA);
}

void publishSensorConfigs() {
	if (homeConfig.listenForDeviceIdMode == 0) {
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_1, homeConfig.alarm_1);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_2, homeConfig.alarm_2);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_3, homeConfig.alarm_3);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_4, homeConfig.alarm_4);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_5, homeConfig.alarm_5);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_6, homeConfig.alarm_6);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_7, homeConfig.alarm_7);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_8, homeConfig.alarm_8);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_9, homeConfig.alarm_9);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_10, homeConfig.alarm_10);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_11, homeConfig.alarm_11);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_12, homeConfig.alarm_12);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_13, homeConfig.alarm_13);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_14, homeConfig.alarm_14);
		publishSensorConfig(ALARM_CONTROLLER_CONFIGURE_SENSOR_15, homeConfig.alarm_15);
	}
}

void readConfigOnStartup() {
	homeConfig.deviceId = readByteEEPROM(ADDRESS_DEVICE_ID_PART_0) | readByteEEPROM(ADDRESS_DEVICE_ID_PART_1) << 8;
	homeConfig.tamper = readByteEEPROM(ADDRESS_TAMPER);
	homeConfig.heartbeat = readByteEEPROM(ADDRESS_HEARTBEAT);
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
	writeByteEEPROM(ADDRESS_DEVICE_ID_PART_0, 0xFF);
	writeByteEEPROM(ADDRESS_DEVICE_ID_PART_1, 0xFF);
	writeByteEEPROM(ADDRESS_TAMPER, 0);
	writeByteEEPROM(ADDRESS_HEARTBEAT, 1);
	writeByteEEPROM(ADDRESS_ALARM_1, 0);
	writeByteEEPROM(ADDRESS_ALARM_2, 0);
	writeByteEEPROM(ADDRESS_ALARM_3, 0);
	writeByteEEPROM(ADDRESS_ALARM_4, 0);
	writeByteEEPROM(ADDRESS_ALARM_5, 0);
	writeByteEEPROM(ADDRESS_ALARM_6, 0);
	writeByteEEPROM(ADDRESS_ALARM_7, 0);
	writeByteEEPROM(ADDRESS_ALARM_8, 0);
	writeByteEEPROM(ADDRESS_ALARM_9, 0);
	writeByteEEPROM(ADDRESS_ALARM_10, 0);
	writeByteEEPROM(ADDRESS_ALARM_11, 0);
	writeByteEEPROM(ADDRESS_ALARM_12, 0);
	writeByteEEPROM(ADDRESS_ALARM_13, 0);
	writeByteEEPROM(ADDRESS_ALARM_14, 0);
	writeByteEEPROM(ADDRESS_ALARM_15, 0);
}

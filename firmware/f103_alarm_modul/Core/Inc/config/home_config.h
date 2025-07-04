/*
 * home_config.h
 *
 *  Created on: 2017.05.23.
 *      Author: Szbuli
 */

#ifndef CONFIG_HOME_CONFIG_H_
#define CONFIG_HOME_CONFIG_H_

#include "stm32f1xx.h"

#define ADDRESS_DEVICE_ID_PART_0 0
#define ADDRESS_DEVICE_ID_PART_1 1
#define ADDRESS_ALARM_1 10
#define ADDRESS_ALARM_2 11
#define ADDRESS_ALARM_3 12
#define ADDRESS_ALARM_4 13
#define ADDRESS_ALARM_5 14
#define ADDRESS_ALARM_6 15
#define ADDRESS_ALARM_7 16
#define ADDRESS_ALARM_8 17
#define ADDRESS_ALARM_9 18
#define ADDRESS_ALARM_10 19
#define ADDRESS_ALARM_11 20
#define ADDRESS_ALARM_12 21
#define ADDRESS_ALARM_13 22
#define ADDRESS_ALARM_14 23
#define ADDRESS_ALARM_15 24
#define ADDRESS_TAMPER 30
#define ADDRESS_HEARTBEAT 31

#define ADDRESS_COUNTER_1 40
#define ADDRESS_COUNTER_2 48
#define ADDRESS_COUNTER_3 56
#define ADDRESS_COUNTER_4 64
#define ADDRESS_COUNTER_5 72
#define ADDRESS_COUNTER_6 80
#define ADDRESS_COUNTER_7 88
#define ADDRESS_COUNTER_8 96
#define ADDRESS_COUNTER_9 104
#define ADDRESS_COUNTER_10 112
#define ADDRESS_COUNTER_11 120
#define ADDRESS_COUNTER_12 128
#define ADDRESS_COUNTER_13 136
#define ADDRESS_COUNTER_14 144
#define ADDRESS_COUNTER_15 152

#define STM32_UUID_32 ((uint32_t *)0x1FFF7A10)
#define STM32_UUID_16 ((uint16_t *)0x1FFF7A10)

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#define DEVICE_TYPE_ID_ALARM_CONTROLLER 1

typedef struct {
	uint8_t listenForDeviceIdMode;
	uint8_t deviceLoaded;
	uint16_t deviceId;
	uint8_t tamper;
	uint8_t heartbeat;
	uint8_t alarm_1;
	uint8_t alarm_2;
	uint8_t alarm_3;
	uint8_t alarm_4;
	uint8_t alarm_5;
	uint8_t alarm_6;
	uint8_t alarm_7;
	uint8_t alarm_8;
	uint8_t alarm_9;
	uint8_t alarm_10;
	uint8_t alarm_11;
	uint8_t alarm_12;
	uint8_t alarm_13;
	uint8_t alarm_14;
	uint8_t alarm_15;
} HomeConfig;

extern HomeConfig homeConfig;

void checkAndDoFactoryResetIfNeeded();
void readConfigOnStartup();
void factoryReset();
void configureSensor(uint16_t canId, uint8_t state);
void configureTamper(uint8_t state);
void configureHeartbeat(uint8_t state);
void publishSensorConfigs();
void configureCounterCurrentValue(uint16_t typeId, uint64_t counterValue);

#endif /* CONFIG_HOME_CONFIG_H_ */

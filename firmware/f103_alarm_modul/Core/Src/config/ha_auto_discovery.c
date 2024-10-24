/*
 * ha_auto_discovery.c
 *
 *  Created on: 2021.01.03.
 *      Author: Szbuli
 */

#include "home_config.h"
#include "can_support.h"
#include "can-ids.h"

void sendAutoDiscoveryMessage(uint16_t topicId, uint16_t stateTopicId, uint8_t data[]) {
	data[4] = stateTopicId >> 8;
	data[5] = stateTopicId;
	putCanMessageToQueue(topicId, data, 8, CAN_RTR_DATA);
}

void sendAutoDiscoveryMessageWithoutAvailability(uint16_t topicId, uint16_t stateTopicId, uint8_t data[]) {
	data[4] = stateTopicId >> 8;
	data[5] = stateTopicId;
	putCanMessageToQueue(topicId, data, 6, CAN_RTR_DATA);
}

void publishSensor(uint8_t type, uint8_t dataTemplate[], uint8_t resetDataTemplate[], uint16_t alarmStateTopicId, uint16_t counterStateTopicId) {
	if (type == 1) {
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_ALARM_SENSOR_CONFIG, alarmStateTopicId, dataTemplate);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_COUNTER_SENSOR_CONFIG, counterStateTopicId, resetDataTemplate);
	} else if (type == 3 || type == 4) {
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_ALARM_SENSOR_CONFIG, alarmStateTopicId, resetDataTemplate);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_COUNTER_SENSOR_CONFIG, counterStateTopicId, dataTemplate);
	} else {
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_ALARM_SENSOR_CONFIG, alarmStateTopicId, resetDataTemplate);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_COUNTER_SENSOR_CONFIG, counterStateTopicId, resetDataTemplate);
	}
}

void publishConfigForAutoDiscovery() {
	if (homeConfig.listenForDeviceIdMode == 0) {
		uint8_t data[] = { VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, DEVICE_TYPE_ID_ALARM_CONTROLLER, 0, 0,
		ALARM_CONTROLLER_HEARTBEAT >> 8, ALARM_CONTROLLER_HEARTBEAT };
		uint8_t resetData[] = { 0, 0, 0, DEVICE_TYPE_ID_ALARM_CONTROLLER, 0, 0, ALARM_CONTROLLER_HEARTBEAT >> 8, ALARM_CONTROLLER_HEARTBEAT };

		sendAutoDiscoveryMessageWithoutAvailability(ALARM_CONTROLLER_HA_STATUS_CONFIG, ALARM_CONTROLLER_HEARTBEAT, data);

		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_1, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_2, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_3, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_4, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_5, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_6, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_7, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_8, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_9, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_10, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_11, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_12, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_13, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_14, data);
		sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_CONFIGURE_SENSOR_TYPE_CONFIG, ALARM_CONTROLLER_CONFIGURE_SENSOR_15, data);


		publishSensor(homeConfig.alarm_1, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_1, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_1);
		publishSensor(homeConfig.alarm_2, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_2, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_2);
		publishSensor(homeConfig.alarm_3, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_3, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_3);
		publishSensor(homeConfig.alarm_4, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_4, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_4);
		publishSensor(homeConfig.alarm_5, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_5, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_5);
		publishSensor(homeConfig.alarm_6, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_6, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_6);
		publishSensor(homeConfig.alarm_7, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_7, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_7);
		publishSensor(homeConfig.alarm_8, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_8, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_8);
		publishSensor(homeConfig.alarm_9, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_9, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_9);
		publishSensor(homeConfig.alarm_10, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_10, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_10);
		publishSensor(homeConfig.alarm_11, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_11, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_11);
		publishSensor(homeConfig.alarm_12, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_12, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_12);
		publishSensor(homeConfig.alarm_13, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_13, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_13);
		publishSensor(homeConfig.alarm_14, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_14, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_14);
		publishSensor(homeConfig.alarm_15, data, resetData, ALARM_CONTROLLER_SENSOR_STATE_15, ALARM_CONTROLLER_COUNTER_SENSOR_STATE_15);
	}
}

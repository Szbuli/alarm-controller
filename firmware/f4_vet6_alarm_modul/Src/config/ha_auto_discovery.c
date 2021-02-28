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

void publishConfigForAutoDiscovery() {
	if (homeConfig.listenForDeviceIdMode == 0) {
		uint8_t data[] = { VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, DEVICE_TYPE_ID_ALARM_CONTROLLER, 0, 0,
				ALARM_CONTROLLER_HEARTBEAT >> 8, ALARM_CONTROLLER_HEARTBEAT };
		if (homeConfig.alarm_1 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_1, data);
		}
		if (homeConfig.alarm_2 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_2, data);
		}
		if (homeConfig.alarm_3 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_3, data);
		}
		if (homeConfig.alarm_4 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_4, data);
		}
		if (homeConfig.alarm_5 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_5, data);
		}
		if (homeConfig.alarm_6 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_6, data);
		}
		if (homeConfig.alarm_7 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_7, data);
		}
		if (homeConfig.alarm_8 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_8, data);
		}
		if (homeConfig.alarm_9 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_9, data);
		}
		if (homeConfig.alarm_10 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_10, data);
		}
		if (homeConfig.alarm_11 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_11, data);
		}
		if (homeConfig.alarm_12 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_12, data);
		}
		if (homeConfig.alarm_13 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_13, data);
		}
		if (homeConfig.alarm_14 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_14, data);
		}
		if (homeConfig.alarm_15 != 0) {
			sendAutoDiscoveryMessage(ALARM_CONTROLLER_HA_SENSOR_CONFIG, ALARM_CONTROLLER_SENSOR_STATE_15, data);
		}
	}

}

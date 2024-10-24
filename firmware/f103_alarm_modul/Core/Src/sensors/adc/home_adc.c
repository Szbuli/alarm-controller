/*
 * home_adc.c
 *
 *  Created on: 2017. m�rc. 25.
 *      Author: Szbuli
 */

#include "adc.h"
#include "tim.h"
#include "home_adc.h"
#include "can_support.h"
#include "home_config.h"

__IO uint16_t rawAdcData[16];

#define ONE_MINUTE_FREQ_COUNT 600

uint16_t previousRawSensorState[15] = { VOLTAGE_INVALID };
uint16_t previousSensorState[15] = { VOLTAGE_INVALID };
uint16_t sensorState[15] = { VOLTAGE_INVALID };
uint16_t sensorPublishFrequencyCounter[15] = { 0 };

uint16_t sensorCounter[15] = { 0 };

TaskHandle_t adcTask;

void initAndStartAdc() {
	adcTask = xTaskGetCurrentTaskHandle();

	if (HAL_TIM_Base_Start(&htim3) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) rawAdcData, 16) != HAL_OK) {
		Error_Handler();
	}

}

uint8_t convertAlarmState(uint8_t state) {
	if (state == 4) {
		return 0;
	} else {
		return 1;
	}
}

uint8_t getSensorState(uint8_t id) {
	uint16_t reference = rawAdcData[0];

	//TODO change if else order
	//TODO about floating point operations
	if (rawAdcData[id] <= reference && rawAdcData[id] > 0.83 * reference) {
		return VOLTAGE_12V;
	} else if (rawAdcData[id] <= 0.83 * reference && rawAdcData[id] > 0.5175 * reference) {
		return VOLTAGE_8V;
	} else if (rawAdcData[id] <= 0.5175 * reference && rawAdcData[id] > 0.1875 * reference) {
		return VOLTAGE_4V;
	} else if (rawAdcData[id] <= 0.1875 * reference && rawAdcData[id] >= 0) {
		return VOLTAGE_0V;
	} else {
		return VOLTAGE_INVALID;
	}
}

uint32_t getAlarmSensorTopic(uint8_t id) {
	if (id == 1) {
		return ALARM_CONTROLLER_SENSOR_STATE_1;
	} else if (id == 2) {
		return ALARM_CONTROLLER_SENSOR_STATE_2;
	} else if (id == 3) {
		return ALARM_CONTROLLER_SENSOR_STATE_3;
	} else if (id == 4) {
		return ALARM_CONTROLLER_SENSOR_STATE_4;
	} else if (id == 5) {
		return ALARM_CONTROLLER_SENSOR_STATE_5;
	} else if (id == 6) {
		return ALARM_CONTROLLER_SENSOR_STATE_6;
	} else if (id == 7) {
		return ALARM_CONTROLLER_SENSOR_STATE_7;
	} else if (id == 8) {
		return ALARM_CONTROLLER_SENSOR_STATE_8;
	} else if (id == 9) {
		return ALARM_CONTROLLER_SENSOR_STATE_9;
	} else if (id == 10) {
		return ALARM_CONTROLLER_SENSOR_STATE_10;
	} else if (id == 11) {
		return ALARM_CONTROLLER_SENSOR_STATE_11;
	} else if (id == 12) {
		return ALARM_CONTROLLER_SENSOR_STATE_12;
	} else if (id == 13) {
		return ALARM_CONTROLLER_SENSOR_STATE_13;
	} else if (id == 14) {
		return ALARM_CONTROLLER_SENSOR_STATE_14;
	} else {
		return ALARM_CONTROLLER_SENSOR_STATE_15;
	}
}

uint32_t getCounterSensorTopic(uint8_t id) {
	if (id == 1) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_1;
	} else if (id == 2) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_2;
	} else if (id == 3) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_3;
	} else if (id == 4) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_4;
	} else if (id == 5) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_5;
	} else if (id == 6) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_6;
	} else if (id == 7) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_7;
	} else if (id == 8) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_8;
	} else if (id == 9) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_9;
	} else if (id == 10) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_10;
	} else if (id == 11) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_11;
	} else if (id == 12) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_12;
	} else if (id == 13) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_13;
	} else if (id == 14) {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_14;
	} else {
		return ALARM_CONTROLLER_COUNTER_SENSOR_STATE_15;
	}
}

void processAlarmSensor(uint8_t id) {
	uint32_t topicId = getAlarmSensorTopic(id);

	uint8_t currentState = convertAlarmState(sensorState[id - 1]);
	uint8_t previousState = convertAlarmState(previousSensorState[id - 1]);

	//send state every 60 sec even if it is the same
	if (currentState != previousState || sensorPublishFrequencyCounter[id - 1] >= ONE_MINUTE_FREQ_COUNT) {
		sensorPublishFrequencyCounter[id - 1] = 0;
		uint8_t temps[] = { currentState };
		putCanMessageToQueue(topicId, temps, 1, CAN_RTR_DATA);
	}
}

void processCounter(uint8_t sensorType, uint8_t id) {
	uint32_t topicId = getCounterSensorTopic(id);

	uint8_t currentState = sensorState[id - 1];
	uint8_t previousState = previousSensorState[id - 1];

	if ((sensorType == SENSOR_TYPE_COUNTER_NO && (previousState == VOLTAGE_12V || previousState == VOLTAGE_8V)
			&& (currentState == VOLTAGE_0V || currentState == VOLTAGE_4V))
			|| (sensorType == SENSOR_TYPE_COUNTER_NC && (previousState == VOLTAGE_0V || previousState == VOLTAGE_4V)
					&& (currentState == VOLTAGE_12V || currentState == VOLTAGE_8V))) {
		sensorCounter[id - 1]++;
	}

	//send count every 60 sec
	if (sensorPublishFrequencyCounter[id - 1] >= ONE_MINUTE_FREQ_COUNT) {
		sensorPublishFrequencyCounter[id - 1] = 0;
		uint8_t temps[] = { sensorCounter[id - 1] >> 8, sensorCounter[id - 1] };
		putCanMessageToQueue(topicId, temps, 2, CAN_RTR_DATA);
		sensorCounter[id - 1] = 0;
	}
}

void processSensor(uint8_t sensorType, uint8_t id) {
	uint8_t currentState = getSensorState(id);
	uint8_t debouncedState;

	if (currentState != previousRawSensorState[id - 1]) {
		debouncedState = sensorState[id - 1];
	} else {
		debouncedState = currentState;
	}

	if (debouncedState != sensorState[id - 1]) {
		sensorState[id - 1] = debouncedState;
	}

	if (sensorType == SENSOR_TYPE_ALARM) {
		processAlarmSensor(id);
	} else if (sensorType == SENSOR_TYPE_COUNTER_NO || sensorType == SENSOR_TYPE_COUNTER_NC) {
		processCounter(sensorType, id);
	}

	previousRawSensorState[id - 1] = currentState;
	previousSensorState[id - 1] = sensorState[id - 1];
	sensorPublishFrequencyCounter[id - 1]++;
}

void processAdc() {
	for (;;) {
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY);

		processSensor(homeConfig.alarm_1, 1);
		processSensor(homeConfig.alarm_2, 2);
		processSensor(homeConfig.alarm_3, 3);
		processSensor(homeConfig.alarm_4, 4);
		processSensor(homeConfig.alarm_5, 5);
		processSensor(homeConfig.alarm_6, 6);
		processSensor(homeConfig.alarm_7, 7);
		processSensor(homeConfig.alarm_8, 8);
		processSensor(homeConfig.alarm_9, 9);
		processSensor(homeConfig.alarm_10, 10);
		processSensor(homeConfig.alarm_11, 11);
		processSensor(homeConfig.alarm_12, 12);
		processSensor(homeConfig.alarm_13, 13);
		processSensor(homeConfig.alarm_14, 14);
		processSensor(homeConfig.alarm_15, 15);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle) {
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	vTaskNotifyGiveFromISR(adcTask, &xHigherPriorityTaskWoken);

	/* If xHigherPriorityTaskWoken was set to true you
	 we should yield.  The actual macro used here is
	 port specific. */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


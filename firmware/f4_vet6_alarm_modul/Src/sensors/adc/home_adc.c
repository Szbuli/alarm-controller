/*
 * home_adc.c
 *
 *  Created on: 2017. márc. 25.
 *      Author: Szbuli
 */

#include "adc.h"
#include "tim.h"
#include "home_adc.h"
#include "can_support.h"
#include "home_config.h"

__IO uint16_t rawAdcData[16];

#define ok 0
#define alarm 1

uint16_t prevAlarmMeasure[15] = { ok };
uint16_t alarmState[15] = { ALARM_NORMAL };
uint8_t alarmStateCounter[15] = { 0 };

TaskHandle_t adcTask;

void initAndStartAdc() {
	adcTask = xTaskGetCurrentTaskHandle();

	if (HAL_TIM_Base_Start(&htim8) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *) rawAdcData, 16) != HAL_OK) {
		Error_Handler();
	}

}

void processAdc() {
	uint8_t i;

	for (;;) {
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY);

		//HAL_GPIO_TogglePin(BUSY_LED_GPIO_Port, BUSY_LED_Pin);

		uint16_t reference = rawAdcData[0];

		//TODO change if else order
		//TODO about floating point operations

		if (homeConfig.alarm_1 != 0) {
			processAlarmSensor(1, ALARM_CONTROLLER_SENSOR_STATE_1);
		}
		if (homeConfig.alarm_2 != 0) {
			processAlarmSensor(2, ALARM_CONTROLLER_SENSOR_STATE_2);
		}
		if (homeConfig.alarm_3 != 0) {
			processAlarmSensor(3, ALARM_CONTROLLER_SENSOR_STATE_3);
		}
		if (homeConfig.alarm_4 != 0) {
			processAlarmSensor(4, ALARM_CONTROLLER_SENSOR_STATE_4);
		}
		if (homeConfig.alarm_5 != 0) {
			processAlarmSensor(5, ALARM_CONTROLLER_SENSOR_STATE_5);
		}
		if (homeConfig.alarm_6 != 0) {
			processAlarmSensor(6, ALARM_CONTROLLER_SENSOR_STATE_6);
		}
		if (homeConfig.alarm_7 != 0) {
			processAlarmSensor(7, ALARM_CONTROLLER_SENSOR_STATE_7);
		}
		if (homeConfig.alarm_8 != 0) {
			processAlarmSensor(8, ALARM_CONTROLLER_SENSOR_STATE_8);
		}
		if (homeConfig.alarm_9 != 0) {
			processAlarmSensor(9, ALARM_CONTROLLER_SENSOR_STATE_9);
		}
		if (homeConfig.alarm_10 != 0) {
			processAlarmSensor(10, ALARM_CONTROLLER_SENSOR_STATE_10);
		}
		if (homeConfig.alarm_11 != 0) {
			processAlarmSensor(11, ALARM_CONTROLLER_SENSOR_STATE_11);
		}
		if (homeConfig.alarm_12 != 0) {
			processAlarmSensor(12, ALARM_CONTROLLER_SENSOR_STATE_12);
		}
		if (homeConfig.alarm_13 != 0) {
			processAlarmSensor(13, ALARM_CONTROLLER_SENSOR_STATE_13);
		}
		if (homeConfig.alarm_14 != 0) {
			processAlarmSensor(14, ALARM_CONTROLLER_SENSOR_STATE_14);
		}
		if (homeConfig.alarm_15 != 0) {
			processAlarmSensor(15, ALARM_CONTROLLER_SENSOR_STATE_15);
		}
	}
}

uint8_t convertState(uint8_t state) {
	if (state == 4) {
		return 0;
	} else {
		return 1;
	}
}

void processAlarmSensor(uint8_t id, uint32_t topicId) {
	uint16_t reference = rawAdcData[0];

	uint8_t state;
	uint8_t currentState;

	currentState = ALARM_NORMAL;

	if (rawAdcData[id] <= reference && rawAdcData[id] > 0.83 * reference) {
		//short
		state = ALARM_SHORTED;
		//home_log(MODULE_DS18B20, "short", 1);
	} else if (rawAdcData[id] <= 0.83 * reference && rawAdcData[id] > 0.5175 * reference) {
		//normal
		state = ALARM_NORMAL;
		//home_log(MODULE_DS18B20, "normal", 1);
	} else if (rawAdcData[id] <= 0.5175 * reference && rawAdcData[id] > 0.1875 * reference) {
		// alarm
		state = ALARM_TRIGGERED;
		//home_log(MODULE_DS18B20, "alarm", 1);
	} else if (rawAdcData[id] <= 0.1875 * reference && rawAdcData[id] >= 0) {
		//tamper
		state = ALARM_TAMPER;
		//home_log(MODULE_DS18B20, "tamper", 1);
	} else {
		//error
		state = ALARM_ERROR;
		//home_log(MODULE_DS18B20, "error", 1);
	}

	if (state != ALARM_NORMAL) {
		if (prevAlarmMeasure[id - 1] != ok) {
			currentState = state;
		}
		prevAlarmMeasure[id - 1] = alarm;
	} else {
		prevAlarmMeasure[id - 1] = ok;
	}

	if (alarmState[id - 1] != currentState) {
		alarmStateCounter[id - 1] = 0;
		alarmState[id - 1] = currentState;
		uint8_t temps[] = { convertState(currentState) };
		// id start from 1
		putCanMessageToQueue(topicId, temps, 1, CAN_RTR_DATA);
	} else {
		alarmStateCounter[id - 1]++;

		//send state every 60 sec if it is the same
		if (alarmStateCounter[id - 1] >= 240) {
			alarmStateCounter[id - 1] = 0;
			uint8_t temps[] = { convertState(currentState) };
			// id start from 1
			putCanMessageToQueue(topicId, temps, 1, CAN_RTR_DATA);
		}
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	vTaskNotifyGiveFromISR(adcTask, &xHigherPriorityTaskWoken);

	/* If xHigherPriorityTaskWoken was set to true you
	 we should yield.  The actual macro used here is
	 port specific. */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void initHardware() {
	ADC_ChannelConfTypeDef sConfig;

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = ENABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 8;
	hadc1.Init.DMAContinuousRequests = ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = 2;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = 3;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_11;
	sConfig.Rank = 4;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_12;
	sConfig.Rank = 5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = 6;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = 7;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = 8;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

}

void deInitHardware() {

}

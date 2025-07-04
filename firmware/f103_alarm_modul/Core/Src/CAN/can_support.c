/*
 * can_support.c
 *
 *  Created on: 2017. jan. 4.
 *      Author: Szbuli
 */

#include "can.h"
#include "can_support.h"
#include <stdlib.h>
#include "home_log.h"
#include "home_config.h"
#include "eeprom.h"

osMessageQId canSendQueueHandle;
osMessageQId canReceiveQueueHandle;

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;

HAL_StatusTypeDef initCan(osMessageQId canSendQueueHandleArgument, osMessageQId canReceiveQueueHandleArgument) {

	canSendQueueHandle = canSendQueueHandleArgument;
	canReceiveQueueHandle = canReceiveQueueHandleArgument;

	if (canReceiveQueueHandle == NULL) {
		home_error(CAN_RX_QUEUE_CANNOT_BE_CREATED);
		/* Queue was not created and must not be used. */
		return HAL_ERROR;
	}
	if (canSendQueueHandle == NULL) {
		home_error(CAN_TX_QUEUE_CANNOT_BE_CREATED);
		/* Queue was not created and must not be used. */
		return HAL_ERROR;
	}

	//with extension id
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.TransmitGlobalTime = DISABLE;

	//filter for room specific
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = (homeConfig.deviceId >> 13) & 0x7;
	sFilterConfig.FilterIdLow = (homeConfig.deviceId & 0x1FFF) << 3;
	//mask for roomId
	if (homeConfig.listenForDeviceIdMode != 0) {
		sFilterConfig.FilterMaskIdHigh = 0;
		sFilterConfig.FilterMaskIdLow = 0;
	} else {
		sFilterConfig.FilterMaskIdHigh = 0b0000000000000111;
		sFilterConfig.FilterMaskIdLow = 0b1111111111111000;
	}

	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	if (HAL_CAN_ConfigFilter(&MY_CAN, &sFilterConfig) != HAL_OK) {
		return HAL_ERROR;
	}

	//filter for broadcast
	/*sFilterConfig.FilterBank = 1;
	 sFilterConfig.FilterIdHigh = 0x0000;
	 sFilterConfig.FilterIdLow = 0x0000;
	 if (HAL_CAN_ConfigFilter(&MY_CAN, &sFilterConfig) != HAL_OK) {
	 return HAL_ERROR;
	 }*/

	if (HAL_CAN_Start(&MY_CAN) != HAL_OK) {
		home_error(CAN_INIT_FAILED);
	}

	if (HAL_CAN_ActivateNotification(&MY_CAN, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		home_error(CAN_INIT_FAILED);
	}

	return HAL_OK;
}

CAN_OBJECT* wrapCanMessage(uint16_t topicId, uint8_t *dataArray, uint8_t dataLength, uint8_t RTR) {
	assert_param(dataLength <= 8);

	vTaskSuspendAll();
	uint8_t *array = (uint8_t*) pvPortMalloc(dataLength * sizeof(uint8_t));

	for (uint8_t i = 0; i < dataLength; i++) {
		array[i] = dataArray[i];
	}

	CAN_OBJECT *canMessage = (CAN_OBJECT*) pvPortMalloc(sizeof(CAN_OBJECT));
	xTaskResumeAll();

	canMessage->topicId = topicId;
	canMessage->DLC = dataLength;
	canMessage->RTR = RTR;
	canMessage->data = array;

	return canMessage;

}

void putCanMessageToQueue(uint32_t stdId, uint8_t *dataArray, uint8_t dataLength, uint8_t RTR) {
	CAN_OBJECT *canMessage = wrapCanMessage(stdId, dataArray, dataLength, RTR);
	xQueueSend(canSendQueueHandle, &canMessage, (TickType_t ) 0);
}

void putCanMessageToQueueFromInterrupt(uint32_t stdId, uint8_t *dataArray, uint8_t dataLength, uint8_t RTR) {
	CAN_OBJECT *canMessage = wrapCanMessage(stdId, dataArray, dataLength, RTR);
	/* We have not woken a task at the start of the ISR. */
	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	if (xQueueSendFromISR(canSendQueueHandle, &canMessage, &xHigherPriorityTaskWoken) == errQUEUE_FULL) {
		home_error(CAN_TX_QUEUE_FULL);
	}

	/* If xHigherPriorityTaskWoken was set to true you
	 we should yield.  The actual macro used here is
	 port specific. */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void sendCANMessageFromQueue() {
	if (canSendQueueHandle == NULL) {
		vTaskDelete(NULL);
	} else {
		CAN_OBJECT *canObjectPointer;
		for (;;) {
			if (xQueueReceive(canSendQueueHandle, &canObjectPointer, portMAX_DELAY)) {

				TxHeader.ExtId = ((canObjectPointer->topicId << 16) | homeConfig.deviceId);

				for (uint8_t i = 0; i < canObjectPointer->DLC; i++) {
					TxData[i] = canObjectPointer->data[i];
				}

				TxHeader.DLC = canObjectPointer->DLC;
				TxHeader.RTR = canObjectPointer->RTR;

				while (HAL_CAN_GetTxMailboxesFreeLevel(&MY_CAN) == 0) {
					osDelay(10);
				}

				HAL_StatusTypeDef status = HAL_CAN_AddTxMessage(&MY_CAN, &TxHeader, TxData, &TxMailbox);
				if (status != HAL_OK) {
					home_error(CAN_FAILED_TX);
				}

				//just free now, error handling required
				vPortFree(canObjectPointer->data);
				vPortFree(canObjectPointer);
			}
		}
	}
}

void receiveCANMessageFromQueue() {
	if (canReceiveQueueHandle == NULL) {
		vTaskDelete(NULL);
	} else {
		RECEIVED_CAN_OBJECT receivedObject;
		for (;;) {
			if (xQueueReceive(canReceiveQueueHandle, &receivedObject, portMAX_DELAY)) {
				uint16_t typeId = receivedObject.id >> 16;
				//uint16_t homeId = receivedObject.id & 0xFFFF;

				if (homeConfig.listenForDeviceIdMode != 0) {
					if (typeId == ALARM_CONTROLLER_DEVICE_ID) {
						factoryReset();
						writeByteEEPROM(ADDRESS_DEVICE_ID_PART_0, receivedObject.data0);
						writeByteEEPROM(ADDRESS_DEVICE_ID_PART_1, receivedObject.data1);
						osDelay(100);
						HAL_NVIC_SystemReset();
					}
					continue;
				} else if (typeId == ALARM_CONTROLLER_FACTORY_RESET) {
					factoryReset();
					osDelay(100);
					HAL_NVIC_SystemReset();
				} else if (typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_1 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_2
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_3 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_4
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_5 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_6
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_7 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_8
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_9 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_10
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_11 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_12
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_13 || typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_14
						|| typeId == ALARM_CONTROLLER_CONFIGURE_SENSOR_15) {
					configureSensor(typeId, receivedObject.data0);
				} else if (typeId == ALARM_CONTROLLER_COUNTER_SET_1 || typeId == ALARM_CONTROLLER_COUNTER_SET_2
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_3 || typeId == ALARM_CONTROLLER_COUNTER_SET_4
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_5 || typeId == ALARM_CONTROLLER_COUNTER_SET_6
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_7 || typeId == ALARM_CONTROLLER_COUNTER_SET_8
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_9 || typeId == ALARM_CONTROLLER_COUNTER_SET_10
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_11 || typeId == ALARM_CONTROLLER_COUNTER_SET_12
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_13 || typeId == ALARM_CONTROLLER_COUNTER_SET_14
						|| typeId == ALARM_CONTROLLER_COUNTER_SET_15) {
					uint64_t counterValue = ((uint64_t) receivedObject.data0 << 56) | ((uint64_t) receivedObject.data1 << 48)
							| ((uint64_t) receivedObject.data2 << 40) | ((uint64_t) receivedObject.data3 << 32) | ((uint64_t) receivedObject.data4 << 24)
							| ((uint64_t) receivedObject.data5 << 16) | ((uint64_t) receivedObject.data6 << 8) | ((uint64_t) receivedObject.data7);
					configureCounterCurrentValue(typeId, counterValue);
				} else if (typeId == ALARM_CONTROLLER_CONFIGURE_TAMPER) {
					configureTamper(receivedObject.data0);
				} else if (typeId == ALARM_CONTROLLER_CONFIGURE_HEARTBEAT) {
					configureHeartbeat(receivedObject.data0);
				}
			}
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	/* Receive */

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		home_error(CAN_FAILED_RX);
	}

	RECEIVED_CAN_OBJECT receivedObject;
	receivedObject.id = RxHeader.ExtId;
	receivedObject.data0 = RxData[0];
	receivedObject.data1 = RxData[1];
	receivedObject.data2 = RxData[2];
	receivedObject.data3 = RxData[3];
	receivedObject.data4 = RxData[4];
	receivedObject.data5 = RxData[5];
	receivedObject.data6 = RxData[6];
	receivedObject.data7 = RxData[7];

	receivedObject.RTR = RxHeader.RTR;
	receivedObject.DLC = RxHeader.DLC;

	BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;

	if (xQueueSendFromISR(canReceiveQueueHandle, &receivedObject, &xHigherPriorityTaskWoken) == errQUEUE_FULL) {
		home_error(CAN_RX_QUEUE_FULL);
	}

	/* If xHigherPriorityTaskWoken was set to true you
	 we should yield.  The actual macro used here is
	 port specific. */
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


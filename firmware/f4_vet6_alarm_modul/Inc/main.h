/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUSY_LED_Pin GPIO_PIN_13
#define BUSY_LED_GPIO_Port GPIOC
#define ALARM_1_Pin GPIO_PIN_0
#define ALARM_1_GPIO_Port GPIOC
#define ALARM_2_Pin GPIO_PIN_1
#define ALARM_2_GPIO_Port GPIOC
#define ALARM_3_Pin GPIO_PIN_2
#define ALARM_3_GPIO_Port GPIOC
#define ALARM_4_Pin GPIO_PIN_3
#define ALARM_4_GPIO_Port GPIOC
#define ALARM_5_Pin GPIO_PIN_0
#define ALARM_5_GPIO_Port GPIOA
#define ALARM_6_Pin GPIO_PIN_1
#define ALARM_6_GPIO_Port GPIOA
#define ALARM_7_Pin GPIO_PIN_2
#define ALARM_7_GPIO_Port GPIOA
#define ALARM_8_Pin GPIO_PIN_3
#define ALARM_8_GPIO_Port GPIOA
#define ALARM_9_Pin GPIO_PIN_4
#define ALARM_9_GPIO_Port GPIOA
#define ALARM_10_Pin GPIO_PIN_5
#define ALARM_10_GPIO_Port GPIOA
#define ALARM_11_Pin GPIO_PIN_6
#define ALARM_11_GPIO_Port GPIOA
#define ALARM_12_Pin GPIO_PIN_7
#define ALARM_12_GPIO_Port GPIOA
#define ALARM_13_Pin GPIO_PIN_4
#define ALARM_13_GPIO_Port GPIOC
#define ALARM_14_Pin GPIO_PIN_5
#define ALARM_14_GPIO_Port GPIOC
#define ALARM_15_Pin GPIO_PIN_0
#define ALARM_15_GPIO_Port GPIOB
#define ALARM_12V_REFERENCE_Pin GPIO_PIN_1
#define ALARM_12V_REFERENCE_GPIO_Port GPIOB
#define TAMPER_Pin GPIO_PIN_12
#define TAMPER_GPIO_Port GPIOB
#define TAMPER_EXTI_IRQn EXTI15_10_IRQn
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
#define FACTORY_RESET_Pin GPIO_PIN_0
#define FACTORY_RESET_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

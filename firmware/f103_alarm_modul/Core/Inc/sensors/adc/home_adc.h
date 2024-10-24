/*
 * home_adc.h
 *
 *  Created on: 2017. m�rc. 25.
 *      Author: Szbuli
 */

#ifndef SENSORS_ADC_HOME_ADC_H_
#define SENSORS_ADC_HOME_ADC_H_

extern __IO uint16_t rawAdcData[16];

#define ALARM_NORMAL 4 		// 8V
#define ALARM_TAMPER 3		// 0V
#define ALARM_TRIGGERED 2	// 4,5V
#define ALARM_SHORTED 1		// 12V
#define ALARM_ERROR 0		// other

#define VOLTAGE_8V 4 		// 8V
#define VOLTAGE_0V 3		// 0V
#define VOLTAGE_4V 2	// 4,5V
#define VOLTAGE_12V 1		// 12V
#define VOLTAGE_INVALID	0	// other

#define LIGHT_OFF 1
#define LIGHT_ON 2

void initAndStartAdc();
void processAdc();

#endif /* SENSORS_ADC_HOME_ADC_H_ */

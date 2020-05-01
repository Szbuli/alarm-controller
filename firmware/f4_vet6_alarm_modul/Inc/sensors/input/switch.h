/*
 * switch.h
 *
 *  Created on: 2017. febr. 7.
 *      Author: Szbuli
 */

#ifndef INPUT_SWITCH_SWITCH_H_
#define INPUT_SWITCH_SWITCH_H_

#include "cmsis_os.h"

#define debounceTimeMs 50

void processTamperEvents();
HAL_StatusTypeDef initTamper();

#endif /* INPUT_SWITCH_SWITCH_H_ */

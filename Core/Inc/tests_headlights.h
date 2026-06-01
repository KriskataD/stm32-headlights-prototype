/*
 * tests.h
 *
 *  Created on: Jun 20, 2024
 *      Author: 35988
 */

#ifndef INC_TESTS_HEADLIGHTS_H_
#define INC_TESTS_HEADLIGHTS_H_

#include "stdio.h"
#include "headlights.h"

void Headlights_Init_Test();
void Headlights_Updates_Test();
void Headlights_Handle_Test();
GPIO_PinState ReadHeadGPIO(headlights_GPIO* gpioHandle, bool toState0);

#endif /* INC_TESTS_HEADLIGHTS_H_ */

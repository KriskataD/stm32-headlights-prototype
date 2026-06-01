/*
 * tests_indicators.h
 *
 *  Created on: Jun 24, 2024
 *      Author: Kristian Dimitrov
 */

#ifndef INC_TESTS_INDICATORS_H_
#define INC_TESTS_INDICATORS_H_

#include "stdio.h"
#include "indicators.h"

void Indicators_Init_Test();
void Indicators_Updates_Test();
void Indicators_Handle_Test();
GPIO_PinState ReadIndGPIO(indicators_GPIO* gpioHandle, bool toState0);

#endif /* INC_TESTS_INDICATORS_H_ */

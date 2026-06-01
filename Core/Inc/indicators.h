/*
 * indicators.h
 *
 *  Created on: Jun 13, 2024
 *      Author: Kristian Dimitrov
 */

#ifndef INC_INDICATORS_H_
#define INC_INDICATORS_H_

#include "stm32f4xx_hal.h"
#include "stdbool.h"


typedef enum {
	NOSIGNALS = 1,
	LEFT,
	RIGHT,
	HAZARD
}IndicatorsStates_t;


typedef struct
	{
	GPIO_TypeDef* port;
	uint16_t pin;
	}indicators_GPIO;

typedef struct
	{
	GPIO_PinState switchLeft;
	GPIO_PinState switchRight;
	}returnIndSwitches;

typedef struct
	{
	indicators_GPIO leftGPIO;
	indicators_GPIO rightGPIO;
	}returnIndGPIO;

void Indicators_Init(indicators_GPIO left, indicators_GPIO right);
void Indicators_Handle();
bool Indicators_Update();
IndicatorsStates_t GetStateInd();
returnIndSwitches GetIndSwitches();
returnIndGPIO GetIndGPIO();

#endif /* INC_INDICATORS_H_ */



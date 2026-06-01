/*
 * indicators.c
 *
 *  Created on: Jun 13, 2024
 *      Author: 35988
 */

#include "indicators.h"
#include "cmsis_os.h"
//E13 = D3
//F15 = D2

static indicators_GPIO switchLeftGPIO;
static indicators_GPIO switchRightGPIO;
static IndicatorsStates_t stateInd;
static IndicatorsStates_t stateIndOld;
static GPIO_PinState switchLeft;
static GPIO_PinState switchRight;


void Indicators_Init(indicators_GPIO left, indicators_GPIO right)
{
	switchLeftGPIO = left;
	switchRightGPIO = right;
}



bool Indicators_Update()
{
	switchLeft = HAL_GPIO_ReadPin(switchLeftGPIO.port, switchLeftGPIO.pin); //pin D8
	switchRight = HAL_GPIO_ReadPin(switchRightGPIO.port, switchRightGPIO.pin); //pin D11
	if(switchLeft == GPIO_PIN_SET && switchRight == GPIO_PIN_SET)
	{
		stateInd = HAZARD;
	}
	else if(switchLeft == GPIO_PIN_SET)
	{
		stateInd = LEFT;
	}
	else if(switchRight == GPIO_PIN_SET)
	{
		stateInd = RIGHT;
	}
	else if(switchLeft == GPIO_PIN_RESET && switchRight == GPIO_PIN_RESET)
	{
		stateInd = NOSIGNALS;
	}

	if (stateInd != stateIndOld)
	{
		stateIndOld = stateInd;
		return true;
	}
	return false;
}

returnIndSwitches GetIndSwitches()
{
	Indicators_Update();
	returnIndSwitches result;
	result.switchLeft = switchLeft;
	result.switchRight = switchRight;
	return result;
}

IndicatorsStates_t GetStateInd()
{
	Indicators_Update();
	return stateInd;
}

void Indicators_Handle()
{
	switch(stateInd)
		{
			case LEFT:
				HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_15);
				break;
			case RIGHT:
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
				break;
			case HAZARD:
				HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_15);
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
				break;
			case NOSIGNALS:
				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, GPIO_PIN_RESET);
				break;
		}
}

returnIndGPIO GetIndGPIO()
{
	Indicators_Handle();
	returnIndGPIO result;
	result.leftGPIO.port = GPIOF;
	result.leftGPIO.pin = GPIO_PIN_15;
	result.rightGPIO.port = GPIOE;
	result.rightGPIO.pin = GPIO_PIN_13;
	return result;
}

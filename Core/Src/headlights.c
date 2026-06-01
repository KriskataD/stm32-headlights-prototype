/*
 * functions.c
 *
 *  Created on: Jun 12, 2024
 *      Author: 35988
 */
#include <headlights.h>

static headlights_GPIO switchHighGPIO;
static headlights_GPIO switchLowGPIO;
static HeadlightsStates_t stateHead;
static HeadlightsStates_t stateHeadOld;
static GPIO_PinState switchBeamH;
static GPIO_PinState switchBeamL;


void Headlights_Init(headlights_GPIO high, headlights_GPIO low)
{
	switchHighGPIO = high;
	switchLowGPIO = low;
}

bool Headlights_Updates()
{
	switchBeamH = HAL_GPIO_ReadPin(switchHighGPIO.port, switchHighGPIO.pin); //pin D12
	switchBeamL = HAL_GPIO_ReadPin(switchLowGPIO.port, switchLowGPIO.pin);

	if (switchBeamL == GPIO_PIN_RESET && switchBeamH == GPIO_PIN_RESET)
		{
			stateHead = OFF;
		}
	else if (switchBeamL == GPIO_PIN_SET && switchBeamH == GPIO_PIN_RESET)
		{
			stateHead = LOW;
		}
	else
		{
			stateHead = HIGH;
		}

	if (stateHead != stateHeadOld)
	{
		stateHeadOld = stateHead;
		return true;
	}
	return false;
}

returnHeadSwitches GetHeadSwitches()
{
	Headlights_Updates();
	returnHeadSwitches result;
	result.switchHigh = switchBeamH;
	result.switchLow = switchBeamL;
	return result;
}

HeadlightsStates_t GetStateHead()
{
	Headlights_Updates();
	return stateHead;
}

void Headlights_Handle()
{
	switch (stateHead)
	{
	case HIGH:
		TIM1->CCR1 = 100;
		TIM1->CCR2 = 0;
		break;
	case LOW:
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 100;
		break;
	case OFF:
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 0;
		break;
	}
}

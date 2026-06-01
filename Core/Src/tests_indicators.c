/*
 * tests_indicators.c
 *
 *  Created on: Jun 24, 2024
 *      Author: 35988
 */

#include "tests_indicators.h"

GPIO_PinState ReadIndGPIO(indicators_GPIO* gpioHandle, bool toState)
{
	if (gpioHandle != NULL)
	{
		return HAL_GPIO_ReadPin(gpioHandle->port, gpioHandle->pin);
	}

	if (toState)
	{
		return GPIO_PIN_SET;
	}
	return GPIO_PIN_RESET;
}

void Indicators_Init_Test(indicators_GPIO mockSwitchLeftGPIO, indicators_GPIO mockSwitchRightGPIO)
{
	Indicators_Init(mockSwitchLeftGPIO, mockSwitchRightGPIO);

		if ((GPIO_TypeDef *)0x0001 <= mockSwitchLeftGPIO.port  && mockSwitchLeftGPIO.port <= (GPIO_TypeDef *)0x8000 && (GPIO_TypeDef *)0x0001 <= mockSwitchRightGPIO.port  && mockSwitchRightGPIO.port <= (GPIO_TypeDef *)0x8000)
		{
			printf("Test Case Passed: pins detected\n");
		}
		else
		{
			printf("Test Case Failed: pins not detected\n");
		}
}

void Indicators_Updates_Test()
{
	returnIndSwitches result = GetIndSwitches();
	GPIO_PinState switchLeft = result.switchLeft;
	GPIO_PinState switchRight = result.switchRight;
	IndicatorsStates_t tested = GetStateInd();

	Indicators_Update();

	if (switchLeft == ReadIndGPIO(NULL, true) && switchRight == ReadIndGPIO(NULL, true))
	{ // 00
		if(tested == HAZARD)
		{
			printf("Test Case 1 Passed: state changed to HAZARD\n");
		}
		else
		{
			printf("Test Case 1 Failed: no such state\n");
		}
	}
	else if (switchLeft == ReadIndGPIO(NULL, true))
	{ // 01
		if(tested == LEFT)
		{
			printf("Test Case 2 Passed: state changed to LEFT\n");
		}
		else
		{
			printf("Test Case 2 Failed: no such state\n");
		}
	}
	else if (switchRight == ReadIndGPIO(NULL, true))
	{ // 10 || 11
		if(tested == RIGHT)
		{
			printf("Test Case 3 Passed: state changed to RIGHT\n");
		}
		else
		{
			printf("Test Case 3 Failed: no such state\n");
		}
	}
	else if (switchLeft == ReadIndGPIO(NULL, false) && switchRight == ReadIndGPIO(NULL, false))
	{ // 10 || 11
		if(tested == NOSIGNALS)
		{
			printf("Test Case 4 Passed: state changed to NOSIGNALS\n");
		}
		else
		{
			printf("Test Case 4 Failed: no such state\n");
		}
	}
	else
	{
		printf("Test Cases Failed: state error\n");
	}
}

void Indicators_Handle_Test()
{
	IndicatorsStates_t tested = GetStateInd();

	GPIO_PinState initialF15State = HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_15);
	GPIO_PinState initialE13State = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);

	returnIndGPIO result = GetIndGPIO();
	indicators_GPIO F15 = result.leftGPIO;
	indicators_GPIO E13 = result.rightGPIO;

	Indicators_Handle();

	switch(tested)
	{
		case LEFT:
			if (HAL_GPIO_ReadPin(F15.port, F15.pin) != initialF15State)
				{
					printf("Test Case 1 Passed: LEFT\n");
				}
			else
				{
					printf("Test Case 1 Failed: no LEFT\n");
				}
			break;
		case RIGHT:
			if (HAL_GPIO_ReadPin(E13.port, E13.pin) != initialE13State)
				{
					printf("Test Case 2 Passed: RIGHT\n");
				}
			else
				{
					printf("Test Case 2 Failed: no RIGHT\n");
				}
			break;
		case HAZARD:
			if (HAL_GPIO_ReadPin(F15.port, F15.pin) != initialF15State && HAL_GPIO_ReadPin(E13.port, E13.pin) != initialE13State)
				{
					printf("Test Case 3 Passed: HAZARD\n");
				}
			else
				{
					printf("Test Case 3 Failed: no HAZARD\n");
				}
			break;
		case NOSIGNALS:
			if (HAL_GPIO_ReadPin(F15.port, F15.pin) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(E13.port, E13.pin) == GPIO_PIN_RESET)
				{
					printf("Test Case 4 Passed: NOSIGNALS\n");
				}
			else
				{
					printf("Test Case 4 Failed: no NOSIGNALS\n");
				}
			break;
	}
}

/*
 * tests.c
 *
 *  Created on: Jun 20, 2024
 *      Author: Kristian Dimitrov
 */

#include <tests_headlights.h>

GPIO_PinState ReadHeadGPIO(headlights_GPIO* gpioHandle, bool toState)
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



void Headlights_Init_Test(headlights_GPIO mockSwitchHighGPIO, headlights_GPIO mockSwitchLowGPIO)
{
	Headlights_Init(mockSwitchHighGPIO, mockSwitchLowGPIO);

	if ((GPIO_TypeDef *)0x0001 <= mockSwitchHighGPIO.port  && mockSwitchHighGPIO.port <= (GPIO_TypeDef *)0x8000 && (GPIO_TypeDef *)0x0001 <= mockSwitchLowGPIO.port  && mockSwitchLowGPIO.port <= (GPIO_TypeDef *)0x8000)
	{
		printf("Test Case Passed: pins detected\n");
	}
	else
	{
		printf("Test Case Failed: pins not detected\n");
	}
}

void Headlights_Updates_Test()
{
	returnHeadSwitches result = GetHeadSwitches();
	GPIO_PinState switchHighB = result.switchHigh;
	GPIO_PinState switchLowB = result.switchLow;
	HeadlightsStates_t tested = GetStateHead();

	Headlights_Updates();

	// queue - use to test
	if (switchHighB == ReadHeadGPIO(NULL, false) && switchLowB == ReadHeadGPIO(NULL, false))
	{ // 00
		if(tested == OFF)
		{
			printf("Test Case 1 Passed: state changed to Off\n");
		}
		else
		{
			printf("Test Case 1 Failed: no such state\n");
		}
	}
	else if (switchHighB == ReadHeadGPIO(NULL, false) && switchLowB == ReadHeadGPIO(NULL, true))
	{ // 01
		if(tested == LOW)
		{
			printf("Test Case 2 Passed: state changed to LOW\n");
		}
		else
		{
			printf("Test Case 2 Failed: no such state\n");
		}
	}
	else if ( (switchHighB == ReadHeadGPIO(NULL, true) && switchLowB == ReadHeadGPIO(NULL, false) ) || ( switchHighB == ReadHeadGPIO(NULL, true) && switchLowB == ReadHeadGPIO(NULL, true) ) )
	{ // 10 || 11
		if(tested == HIGH)
		{
			printf("Test Case 3 Passed: state changed to HIGH\n");
		}
		else
		{
			printf("Test Case 3 Failed: no such state\n");
		}
	}
	else
	{
		printf("Test Cases Failed: state error\n");
	}
}

void Headlights_Handle_Test()
{
	HeadlightsStates_t tested = GetStateHead();

	Headlights_Handle();

	switch(tested)
	{
		case HIGH:
			if (TIM1->CCR1 == 100 && TIM1->CCR2 == 0)
				{
					printf("Test Case 1 Passed: HIGH\n");
				}
			else
				{
					printf("Test Case 1 Failed: no HIGH\n");
				}
			break;
		case LOW:
			if (TIM1->CCR1 == 0 && TIM1->CCR2 == 100)
				{
					printf("Test Case 2 Passed: LOW\n");
				}
			else
				{
					printf("Test Case 2 Failed: no LOW\n");
				}
			break;
		default:
			if (TIM1->CCR1 == 0 && TIM1->CCR2 == 0)
				{
					printf("Test Case 2 Passed: OFF\n");
				}
			else
				{
					printf("Test Case 2 Failed: no OFF\n");
				}
			break;
	}
}

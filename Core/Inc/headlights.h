#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "tests_headlights.h"
#ifndef HEADLIGHTS_H
#define HEADLIGHTS_H


typedef enum{
	OFF = 1,
	LOW,
	HIGH
}HeadlightsStates_t;

typedef struct
	{
	GPIO_TypeDef* port;
	uint16_t pin;
	}headlights_GPIO;

typedef struct
	{
	GPIO_PinState switchHigh;
	GPIO_PinState switchLow;
	}returnHeadSwitches;

void Headlights_Init(headlights_GPIO left, headlights_GPIO right);
void Headlights_Handle();
bool Headlights_Updates();
HeadlightsStates_t GetStateHead();
returnHeadSwitches GetHeadSwitches();

#endif

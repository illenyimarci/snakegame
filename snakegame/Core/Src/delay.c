
#include "delay.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
uint32_t us = 0;
uint16_t ms;

void delayStart(TIM_HandleTypeDef htim2)
{
	HAL_TIM_Base_Start(&htim2);
}

void delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2, 0);			//0-ról indul
	while(__HAL_TIM_GET_COUNTER(&htim2) < us);	//1MHz órajellel várakozik annyi us-ot, amennyit megadunk a függvény hívásakor
}

void delay_ms(uint16_t ms)
{
	us = ms*1000;
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while(__HAL_TIM_GET_COUNTER(&htim2) < us);
}

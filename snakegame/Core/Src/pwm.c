
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "main.h"

uint8_t DutyCycle = 80;
extern TIM_HandleTypeDef htim3;

void pwmInit()
{
	TIM3->CCR3 = DutyCycle;
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

void pwmSet(uint8_t dtcyc)
{
	if(dtcyc >= 100)
	{
		DutyCycle = 100;
	}
	else if(dtcyc <= 20)
	{
		DutyCycle = 20;
	}
	else
	{
		DutyCycle = dtcyc;
	}
	TIM3->CCR3 = DutyCycle;
}

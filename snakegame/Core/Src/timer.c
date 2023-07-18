
#include "main.h"
#include "timer.h"
#include "delay.h"

TIM_HandleTypeDef htim2;	//us késleltetést megvalósító timer
TIM_HandleTypeDef htim3;	//pwm-et megvalósító timer
TIM_HandleTypeDef htim4;	//kígyó léptetést megvalósító timer
TIM_HandleTypeDef htim5;	//pergésmentesítést megvalósító timer

void TIMER_Init()
{
	MX_TIM2_Init();
	delayStart(htim2);

	MX_TIM3_Init();

	TIM4_Init();
	TIM5_Init();

}

void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0x1000000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void MX_TIM3_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 839;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 99;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim3);

}

void TIM4_Init(void)
{
	__HAL_RCC_TIM4_CLK_ENABLE();

	TIM4 -> PSC = (16000-1);
	TIM4 -> ARR = (700-1);

	TIM4 -> EGR |= TIM_EGR_UG;
	TIM4 ->DIER |= TIM_DIER_UIE;


	HAL_NVIC_SetPriority(TIM4_IRQn, 0,1);
}

void TIM5_Init(void)
{
	__HAL_RCC_TIM5_CLK_ENABLE();

	TIM5 -> PSC = (16000-1);
	TIM5 -> ARR = (10000-1);

	TIM5 -> EGR |= TIM_EGR_UG;
	//TIM5 ->DIER |= TIM_DIER_UIE;
}


//kilolvassa TIM5 aktuális értékét
uint32_t read_TIM5()
{
	return TIM5->CNT;
}
//átírija TIM5 értékét
void write_TIM5()
{
	TIM5 -> CNT = 0;
}


/*
 * timer.h
 *
 *  Created on: Jun 1, 2023
 *      Author: illen
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

void TIMER_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void TIM4_Init(void);
void TIM5_Init(void);
void TIM7_Init();
uint32_t read_TIM5();
void write_TIM5();

#endif /* INC_TIMER_H_ */

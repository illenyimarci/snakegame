/*
 * delay.h
 *
 *  Created on: Jun 1, 2023
 *      Author: illen
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "main.h"

extern TIM_HandleTypeDef htim4;
extern uint32_t us;

void delayStart(TIM_HandleTypeDef htim4);
void delay_us (uint16_t us);
void delay_ms(uint16_t ms);

#endif /* INC_DELAY_H_ */

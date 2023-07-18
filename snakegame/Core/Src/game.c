
#include "game.h"
#include "timer.h"
#include "glcd.h"
#include "delay.h"
#include "snake.h"
#include "stdbool.h"
#include <stdlib.h>
#include <time.h>
#include "pwm.h"

uint8_t score = 0;
uint8_t maxscore = 0;
uint8_t multiplier = 1;
uint8_t difficultylevel = 1;
/*élelem koordinátái*/
uint8_t xrand1;
uint8_t xrand2;

bool btn_pressed = false;
bool game_started = true;

enum direction randpage;
extern enum direction currdirect;
extern UART_HandleTypeDef huart2;
extern uint8_t DutyCycle;
extern uint8_t snakesize;
extern uint8_t temp_buffer;
extern pos snake[98];
extern uint8_t foodcount;


void IDLE_State()
{
	for(int i = 0; i < 99; i++)
	{
		snake[i].x1 = 0;
		snake[i].x2 = 0;
		snake[i].page = 0;
	}
	GLCD_WriteString("ntb sserp", 4, 0);
	GLCD_WriteString("trats ot", 4, 75);
	HAL_UART_Receive_IT(&huart2, &temp_buffer, 1);
	game_started = false;
	while(!game_started)
	{
		if(!HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin))
		{
			GLCD_Clear();
			game_started = true;
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin))
		{
			GLCD_Clear();
			game_started = true;
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin))
		{
			GLCD_Clear();
			game_started = true;
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin))
		{
			GLCD_Clear();
			game_started = true;
			gameStart();
		}
	}
}

void gameStart()
{
	for(int i = 0; i < 99; i++)
	{
		snake[i].x1 = 0;
		snake[i].x2 = 0;
		snake[i].page = 0;
	}
	snakesize = 4;
	for(int i = 3; i >= 0; i--)
	{
		snake[i].page = 4-i;
		snake[i].x1 = 0;
		snake[i].x2 = 7;

		GLCD_DrawBlock(snake[i].page, snake[i].x1, snake[i].x2);
	}

	score = 0;
	difficultylevel = 1;
	TIM4 -> PSC = (16000-1);
	TIM4 -> ARR = (700-1);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4 -> CNT = 0;
	TIM5 -> CNT = 0;
	TIM4 -> CR1 |= TIM_CR1_CEN;
	TIM5 -> CR1 |= TIM_CR1_CEN;
	wrappedSend('g', 1);
	delay_ms(40);
	wrappedSend('p', score);
	delay_ms(40);
	wrappedSend('d', difficultylevel);
	delay_ms(40);
	wrappedSend('m', maxscore);
	srand(HAL_GetTick());
}

void gameOver()
{
	NVIC_DisableIRQ(TIM4_IRQn);
	TIM4->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
	wrappedSend('g', 0);
	GLCD_Clear();
	score = 0;
	difficultylevel = 1;
	GLCD_WriteString("emag", 5, 1);
	GLCD_WriteString("revo", 5, 102);
	GLCD_WriteString("ntb sserp", 3, 5);
	GLCD_WriteString("tratser ot", 3, 69);
	btn_pressed = true;
	while(btn_pressed)
	{
		if(!HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin))
		{
			btn_pressed = false;
			GLCD_Clear();
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin))
		{
			btn_pressed = false;
			GLCD_Clear();
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin))
		{
			btn_pressed = false;
			GLCD_Clear();
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin))
		{
			btn_pressed = false;
			GLCD_Clear();
			gameStart();
		}
		else if(!HAL_GPIO_ReadPin(BTN5_GPIO_Port, BTN5_Pin))
		{
			DutyCycle -= 10;
			pwmSet(DutyCycle);
		}
		else if(!HAL_GPIO_ReadPin(BTN5_GPIO_Port, BTN5_Pin))
		{
			DutyCycle += 10;
			pwmSet(DutyCycle);
		}
	}
}

void Food(uint8_t fpg, uint8_t lpg, uint8_t x1, uint8_t x2, uint8_t x4)
{
	do{
		randpage = rand()%8;
	}while(randpage > 7 && randpage < 0);

	xrand1 = rand()%128;
	xrand2 = xrand1 + 7;
	checkCoords(&xrand1, &xrand2);

	for(int i = 0; i < snakesize-1; i++)
	{
		if(onSnake || (xrand2 > 63 && xrand2 < 70) || xrand1 > 120)
		{
			xrand1 = rand()%128;
			xrand2 = xrand1+7;
			checkCoords(&xrand1, &xrand2);
			i = 0;
		}
	}

	checkCoords(&xrand1, &xrand2);
	GLCD_DrawBlock(randpage, xrand2, xrand1);
}


void incScore(uint8_t fp, uint8_t x1, uint8_t x2)
{
	score++;

	GLCD_EraseBlock(randpage, xrand2, xrand1);
	randpage = 0;
	xrand2 = 0;
	xrand1 = 0;
	foodcount = 0;
	write_TIM5();

	snakesize++;
	wrappedSend('p', score);
	GLCD_EraseBlock(randpage, xrand1, xrand2);
	GLCD_DrawBlock(fp, x1, x2);
	if((score%2) == 0)
	{
		if(difficultylevel == 10)
		{
			return;
		}
		else
		{
			difficultylevel++;
			delay_ms(10);
			wrappedSend('d', difficultylevel);
			TIM4 -> ARR -= 50;
		}
	}
	if(score > maxscore)
	{
		maxscore = score;
		delay_ms(10);
		wrappedSend('m', maxscore);
	}

}

/*megvizsgálja a két paraméterlistán kapott koordinátát,
  ha érvénytelen az értékük, akkor korrigálja*/
void checkCoords(uint8_t* coord1, uint8_t* coord2)
{
	/*a kígyó jobbra mozog és eléri a kijelző közepén levő határt*/
	if(*coord1 > 127 && *coord2 < 64 && *coord2 >= 0)
	{
		/*0 után eggyel jobbra a következő x koordináta 127*/
		switch(*coord1)
		{
			case 255 : *coord1 = 127; break;
			case 254 : *coord1 = 126; break;
			case 253 : *coord1 = 125; break;
			case 252 : *coord1 = 124; break;
			case 251 : *coord1 = 123; break;
			case 250 : *coord1 = 122; break;
			case 249 : *coord1 = 121; break;
			case 248 : *coord1 = 120; break;
			case 247 : *coord1 = 119; break;
			default: break;
		}
	}
	if(*coord2 > 127 && *coord1 > 63 && *coord1 <= 127)
	{
		switch(*coord2)
		{
			case 255 : *coord2 = 127; break;
			case 254 : *coord2 = 126; break;
			case 253 : *coord2 = 125; break;
			case 252 : *coord2 = 124; break;
			case 251 : *coord2 = 123; break;
			case 250 : *coord2 = 122; break;
			case 249 : *coord2 = 121; break;
			case 248 : *coord1 = 120; break;
			case 247 : *coord1 = 119; break;
			default: break;
		}
	}
	/*a kígyó balra mozog és eléri a kijelző közepén levő határt*/
	if(*coord1 > 127 && *coord2 > 63 && *coord2 <=127)
	{
		/*127 után balra eggyel a 0 koordináta következik*/
		switch(*coord1)
		{
			case 136 : *coord1 = 8; break;
			case 135 : *coord1 = 7; break;
			case 134 : *coord1 = 6; break;
			case 133 : *coord1 = 5; break;
			case 132 : *coord1 = 4; break;
			case 131 : *coord1 = 3; break;
			case 130 : *coord1 = 2; break;
			case 129 : *coord1 = 1; break;
			case 128 : *coord1 = 0; break;
			default: break;
		}
	}
	/*kígyó vége átlép a kijelző felénél levő határvonalon*/
	if(*coord2 > 127 && *coord1 < 64 && *coord1 >=0)
	{
		switch(*coord2)
		{
			case 136 : *coord1 = 8; break;
			case 135 : *coord1 = 7; break;
			case 134 : *coord2 = 6; break;
			case 133 : *coord2 = 5; break;
			case 132 : *coord2 = 4; break;
			case 131 : *coord2 = 3; break;
			case 130 : *coord2 = 2; break;
			case 129 : *coord2 = 1; break;
			case 128 : *coord2 = 0; break;
			default: break;
		}
	}
	if(*coord2 > 127 && *coord1 < 64 && *coord1 >=0)
	{
		switch(*coord2)
		{
			case 255 : *coord2 = 127; break;
			case 254 : *coord2 = 126; break;
			case 253 : *coord2 = 125; break;
			case 252 : *coord2 = 124; break;
			case 251 : *coord2 = 123; break;
			default: break;
		}
	}
	/*ha a kígyó fejének mindkét része pont átlép a határon*/
	if(*coord1 > 127 && *coord2 > 127) //254->247
	{
		switch(*coord1)
		{
			case 255: *coord1 = 127; *coord2 = 120; break;
			case 254: *coord1 = 126; *coord2 = 119; break;
			case 136: *coord1 = 8; *coord2 = 1; break;
			case 135: *coord1 = 7; *coord2 = 0; break;
		}
	}
}

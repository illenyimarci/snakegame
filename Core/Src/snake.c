
#include "timer.h"
#include "glcd.h"
#include "pwm.h"
#include "snake.h"
#include "stdbool.h"
#include "delay.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include "string.h"

//változók
uint8_t count = 0;
uint8_t foodcount = 0;
uint16_t dly;
uint8_t prevtailx = 0;
uint8_t prevtailpage = 0;
uint8_t snakesize = 4;
//máshol deklarált változók
extern uint8_t DutyCycle;
extern uint8_t randpage;
extern uint8_t xrand1;
extern uint8_t xrand2;
extern uint8_t difficultylevel;
extern uint8_t score;
extern uint8_t maxscore;
//máshol deklarált perifériák
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
//mozgási irányok: jelenlegi, előző
enum direction currdirect;
enum direction prevdirect;
//kígyó
pos snake[98];


void goLeft()
{
	prevdirect = LEFT;
	GLCD_EraseBlock(snake[snakesize-1].page, snake[snakesize-1].x1, snake[snakesize-1].x2);

	prevtailx = snake[snakesize-1].x1;
	prevtailpage = snake[snakesize-1].page;

	for(int i = snakesize-1; i > 0; i--){
		snake[i].page = snake[i-1].page;
		snake[i].x1 = snake[i-1].x1;
		snake[i].x2 = snake[i-1].x2;
	}

	snake[0].x1 += 9;
	snake[0].x2 += 9;
	checkCoords(&snake[0].x2, &snake[0].x1);
	//léptetett kígyó megjelenítése
	for(int i = snakesize-1; i >= 0; i--)
	{
		GLCD_DrawBlock(snake[i].page, snake[i].x1, snake[i].x2);
	}
	selfBump();
	//ha kimentünk a pályáról vége a játéknak
	if(snake[0].x2 > 63 && snake[0].x2 < 72){
		gameOver();
	}
	if(foodeaten1 || foodeaten2)
	{
		incScore(snake[0].page, snake[0].x1, snake[0].x2);
	}
}

void goRight()
{
	prevdirect = RIGHT;
	//kígyó végének törlése
	GLCD_EraseBlock(snake[snakesize-1].page, snake[snakesize-1].x1, snake[snakesize-1].x2);

	prevtailx = snake[snakesize-1].x1;
	prevtailpage = snake[snakesize-1].page;
	//kígyó testének léptetése
	for(int i = snakesize-1; i > 0; i--){
		snake[i].page = snake[i-1].page;
		snake[i].x1 = snake[i-1].x1;
		snake[i].x2 = snake[i-1].x2;
	}
	//kígyó fejének léptetése
	snake[0].x1 -= 9;
	snake[0].x2 -= 9;
	checkCoords(&snake[0].x2, &snake[0].x1);
	//léptetett kígyó megjelenítése
	for(int i = snakesize-1; i >= 0; i--)
	{
		GLCD_DrawBlock(snake[i].page, snake[i].x1, snake[i].x2);
	}
	//megvizsgálja, hogy a kígyó magának ütközött-e
	selfBump();
	//ha kimentünk a pályáról, akkor vége a játéknak
	if(snake[0].x1  < 64 && snake[0].x1 > 55){
		gameOver();
	}
	else if(foodeaten1 || foodeaten2)
	{
		incScore(snake[0].page, snake[0].x1, snake[0].x2);
	}
}

void goUp()
{
	prevdirect = UP;

	GLCD_EraseBlock(snake[snakesize-1].page, snake[snakesize-1].x1, snake[snakesize-1].x2);
	prevtailx = snake[snakesize-1].x1;
	prevtailpage = snake[snakesize-1].page;
	for(int i = snakesize-1; i > 0; i--){
		snake[i].page = snake[i-1].page;
		snake[i].x1 = snake[i-1].x1;
		snake[i].x2 = snake[i-1].x2;
	}
	snake[0].page += 1;

	for(int i = snakesize-1; i >= 0; i--)
	{
		GLCD_DrawBlock(snake[i].page, snake[i].x1, snake[i].x2);
	}
	//megvizsgálja, hogy a kígyó magának ütközött-e
	selfBump();
	//ha kimentünk a pályáról, akkor vége a játéknak
	if(snake[0].page > 7)
	{
		gameOver();
	}
	if(foodeaten1 || foodeaten2)
	{
		incScore(snake[0].page, snake[0].x1, snake[0].x2);
	}
}

void goDown()
{
	prevdirect = DOWN;

	GLCD_EraseBlock(snake[snakesize-1].page, snake[snakesize-1].x1, snake[snakesize-1].x2);
	prevtailx = snake[snakesize-1].x1;
	prevtailpage = snake[snakesize-1].page;
	for(int i = snakesize-1; i > 0; i--){
		snake[i].page = snake[i-1].page;
		snake[i].x1 = snake[i-1].x1;
		snake[i].x2 = snake[i-1].x2;
	}
	snake[0].page -= 1;
	for(int i = snakesize-1; i >= 0; i--)
	{
		GLCD_DrawBlock(snake[i].page, snake[i].x1, snake[i].x2);
	}
	//megvizsgálja, hogy a kígyó magának ütközött-e
	selfBump();
	//ha kimentünk a pályáról
	if(snake[0].page > 7)
	{
		gameOver();
	}
	//ha belement a kígyó az élelembe
	if(foodeaten1 || foodeaten2)
	{
		incScore(snake[0].page, snake[0].x1, snake[0].x2);
	}
}

void selfBump()
{
	//kígyó legvégébe ütköztünk
	if(selfbump)
	{
		gameOver();
	}
	//kígyó bámely másik részébe ütköztünk
	for(int i = 3; i < snakesize; i++)
	{
		if(snake[0].page == snake[i].page && snake[0].x1 == snake[i].x1 )
		{
			gameOver();
		}
	}
	return;
}
void DrawFood(){
	uint32_t currtim = read_TIM5();
	if(currtim >= 1000)
	{
		foodcount++;
		if(foodcount == 1)
		{
			Food(snake[0].page, snake[snakesize-1].page, snake[0].x1, snake[0].x2, snake[snakesize-1].x2);
		}
	}
	if(currtim >= 9000)
	{
		GLCD_EraseBlock(randpage, xrand2, xrand1);
		randpage = 0;
		xrand2 = 0;
		xrand1 = 0;
		foodcount = 0;
		write_TIM5();
	}
	return;
}

void wrappedSend(char t, int i)
{
	char temp[20] = {0};
	temp[0] = t;
	char temp2[10] = {0};
	itoa(i,temp2, 10);
	strcat(temp, temp2);
	uartSend(temp);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//megnézzük, hogy melyik gomb lett lenyomva

	switch(GPIO_Pin)
	{
		case BTN1_Pin:
			if(prevdirect == RIGHT)
			{
				return;
			}
			currdirect = LEFT;
			break;

		case BTN2_Pin:
			if(prevdirect == DOWN)
			{
				return;
			}
			currdirect = UP;
			break;

		case BTN3_Pin:
			if(prevdirect == UP)
			{
				return;
			}
			currdirect = DOWN;
			break;

		case BTN4_Pin:
			if(prevdirect == LEFT)
			{
				return;
			}
			currdirect = RIGHT;
			break;

		case BTN5_Pin:
			DutyCycle -= 10;
			pwmSet(DutyCycle);
			break;

		case BTN6_Pin:
			DutyCycle += 10;
			pwmSet(DutyCycle);
			break;
		default: break;
	}
}

void TIM4_IRQHandler(void)
{
	DrawFood();

	if(TIM4 -> SR & TIM_SR_UIF)
	{
		TIM4 -> SR = ~(TIM_SR_UIF);
		if(currdirect == LEFT)
		{
			goLeft();
		}
		if(currdirect == UP)
		{
			goUp();
		}
		if(currdirect == DOWN)
		{
			goDown();
		}
		if(currdirect == RIGHT)
		{
			goRight();
		}
	}
}

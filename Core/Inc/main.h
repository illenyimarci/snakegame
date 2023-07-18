
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
enum direction{ UP, DOWN, LEFT, RIGHT };
extern enum direction curdirection;
typedef struct {
	uint8_t x1;
	uint8_t x2;
	uint8_t page;
}pos;

/* USER CODE END Includes */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void uartSend(char* msg);


/* Private defines -----------------------------------------------------------*/
#define DI_Pin GPIO_PIN_13
#define DI_GPIO_Port GPIOC
#define EN_Pin GPIO_PIN_14
#define EN_GPIO_Port GPIOC
#define CS2_Pin GPIO_PIN_0
#define CS2_GPIO_Port GPIOC
#define CS1_Pin GPIO_PIN_1
#define CS1_GPIO_Port GPIOC
#define RW_Pin GPIO_PIN_2
#define RW_GPIO_Port GPIOC
#define BTN2_Pin GPIO_PIN_1
#define BTN2_GPIO_Port GPIOA
#define BTN2_EXTI_IRQn EXTI1_IRQn
#define DB3_Pin GPIO_PIN_5
#define DB3_GPIO_Port GPIOA
#define DB4_Pin GPIO_PIN_6
#define DB4_GPIO_Port GPIOA
#define DB5_Pin GPIO_PIN_7
#define DB5_GPIO_Port GPIOA
#define BTN1_Pin GPIO_PIN_0
#define BTN1_GPIO_Port GPIOB
#define BTN1_EXTI_IRQn EXTI0_IRQn
#define DB1_Pin GPIO_PIN_10
#define DB1_GPIO_Port GPIOB
#define BTN6_Pin GPIO_PIN_13
#define BTN6_GPIO_Port GPIOB
#define BTN6_EXTI_IRQn EXTI15_10_IRQn
#define DB6_Pin GPIO_PIN_8
#define DB6_GPIO_Port GPIOA
#define DB7_Pin GPIO_PIN_9
#define DB7_GPIO_Port GPIOA
#define DB2_Pin GPIO_PIN_10
#define DB2_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_15
#define E_GPIO_Port GPIOA
#define RST_Pin GPIO_PIN_12
#define RST_GPIO_Port GPIOC
#define BTN3_Pin GPIO_PIN_2
#define BTN3_GPIO_Port GPIOD
#define BTN3_EXTI_IRQn EXTI2_IRQn
#define BTN4_Pin GPIO_PIN_3
#define BTN4_GPIO_Port GPIOB
#define BTN4_EXTI_IRQn EXTI3_IRQn
#define DB0_Pin GPIO_PIN_5
#define DB0_GPIO_Port GPIOB
#define BTN5_Pin GPIO_PIN_8
#define BTN5_GPIO_Port GPIOB
#define BTN5_EXTI_IRQn EXTI9_5_IRQn

#define selfbump (snake[0].x1 == prevtailx && snake[0].page == prevtailpage)
#define foodeaten1 (snake[0].page == randpage && (snake[0].x1 <= xrand2 && snake[0].x1 >= xrand1))
#define foodeaten2 (snake[0].page == randpage && (snake[0].x2 <= xrand2 && snake[0].x2 >= xrand1))


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

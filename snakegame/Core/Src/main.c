
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "glcd.h"
#include "timer.h"
#include "gpio.h"
#include "snake.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "game.h"

/*flagek mindegyik gombhoz
  ebből tudjuk, hogy honnan
  érkezett az interrupt*/


extern TIM_HandleTypeDef htim5;
extern enum direction currdirect;
uint8_t startbyte = 20;
uint8_t endbyte = 10;
extern uint8_t temp_buffer;

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_USART2_UART_Init(void);



int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  TIMER_Init();
  GLCD_Init();
  //játék kezdeti állapot függvényének hívása
  IDLE_State();
  while (1)
  {
	  //kliens alkalmazástól érkező üzenetek figyelése
	  //ha ASCII b érkezik, szünetelteti a játékot
	  if(temp_buffer == 'b')
	  {
		  //KIKAPCS
		  NVIC_DisableIRQ(TIM4_IRQn);
		  TIM4->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
		  temp_buffer = 0;
	  }
	  //ha ASCII r érkezik, újra elindítja a játékot
	  else if(temp_buffer == 'r')
	  {
		  //BEKAPCS
		  NVIC_EnableIRQ(TIM4_IRQn);
		  TIM4 -> CR1 |= TIM_CR1_CEN;
		  temp_buffer = 0;
	  }
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

void uartSend(char* msg)
{
	char tosend[50] = { 0 };
	tosend[0] = startbyte;
	strcpy(tosend+1, msg);
	tosend[strlen(tosend)] = endbyte;
	if((HAL_UART_Transmit(&huart2, (uint8_t*)tosend, strlen(tosend), 300)) != HAL_OK)
	{
		Error_Handler();
	}
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

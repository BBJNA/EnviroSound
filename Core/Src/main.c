/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac_ch1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint16_t sintable[] = {0x800,0x880,0x900,0x97f,0x9fd,0xa78,0xaf1,0xb67,0xbda,0xc49,
		0xcb3,0xd19,0xd79,0xdd4,0xe29,0xe78,0xec0,0xf02,0xf3c,0xf6f,
		0xf9b,0xfbf,0xfdb,0xfef,0xffb,0xfff,0xffb,0xfef,0xfdb,0xfbf,
		0xf9b,0xf6f,0xf3c,0xf02,0xec0,0xe78,0xe29,0xdd4,0xd79,0xd19,
		0xcb3,0xc49,0xbda,0xb67,0xaf1,0xa78,0x9fd,0x97f,0x900,0x880,
		0x800,0x77f,0x6ff,0x680,0x602,0x587,0x50e,0x498,0x425,0x3b6,
		0x34c,0x2e6,0x286,0x22b,0x1d6,0x187,0x13f,0x0fd,0x0c3,0x090,
		0x064,0x040,0x024,0x010,0x004,0x000,0x004,0x010,0x024,0x040,
		0x064,0x090,0x0c3,0x0fd,0x13f,0x187,0x1d6,0x22b,0x286,0x2e6,
		0x34c,0x3b6,0x425,0x498,0x50e,0x587,0x602,0x680,0x6ff,0x77f};

uint16_t sawtable[] = {0x052,0x0a4,0x0f6,0x148,0x19a,0x1eb,0x23d,0x28f,0x2e1,0x333,
		0x385,0x3d7,0x429,0x47b,0x4cd,0x51e,0x570,0x5c2,0x614,0x666,
		0x6b8,0x70a,0x75c,0x7ae,0x800,0x851,0x8a3,0x8f5,0x947,0x999,
		0x9eb,0xa3d,0xa8f,0xae1,0xb33,0xb84,0xbd6,0xc28,0xc7a,0xccc,
		0xd1e,0xd70,0xdc2,0xe14,0xe66,0xeb7,0xf09,0xf5b,0xfad,0xfff,
		0x000,0x052,0x0a4,0x0f6,0x148,0x19a,0x1eb,0x23d,0x28f,0x2e1,
		0x333,0x385,0x3d7,0x429,0x47b,0x4cd,0x51e,0x570,0x5c2,0x614,
		0x666,0x6b8,0x70a,0x75c,0x7ae,0x800,0x851,0x8a3,0x8f5,0x947,
		0x999,0x9eb,0xa3d,0xa8f,0xae1,0xb33,0xb84,0xbd6,0xc28,0xc7a,
		0xccc,0xd1e,0xd70,0xdc2,0xe14,0xe66,0xeb7,0xf09,0xf5b,0xfad};

uint16_t driptable[] = {0xfff,0xec4,0xda2,0xc95,0xb9e,0xab9,0x9e6,0x923,0x86f,0x7c9,
		0x730,0x6a3,0x620,0x5a7,0x538,0x4d1,0x473,0x41b,0x3ca,0x380,
		0x33b,0x2fb,0x2c1,0x28a,0x258,0x22a,0x200,0x1d8,0x1b4,0x192,
		0x173,0x157,0x13d,0x124,0x10e,0x0f9,0x0e6,0x0d4,0x0c4,0x0b5,
		0x0a7,0x09a,0x08e,0x083,0x079,0x070,0x067,0x05f,0x058,0x051,
		0x000,0x051,0x058,0x05f,0x067,0x070,0x079,0x083,0x08e,0x09a,
		0x0a7,0x0b5,0x0c4,0x0d4,0x0e6,0x0f9,0x10e,0x124,0x13d,0x157,
		0x173,0x192,0x1b4,0x1d8,0x200,0x22a,0x258,0x28a,0x2c1,0x2fb,
		0x33b,0x380,0x3ca,0x41b,0x473,0x4d1,0x538,0x5a7,0x620,0x6a3,
		0x730,0x7c9,0x86f,0x923,0x9e6,0xab9,0xb9e,0xc95,0xda2,0xec4};

uint8_t tablesize = sizeof(sintable)/sizeof(sintable[0]);

/*Variable to change wave type
 * Sin = 0
 * Saw = 1
 * Drip = 2
 */
uint8_t waveselect = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void EXI_GPIO_Config(void);
static void WaveChange(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  //Start TIM6 for DAC event trigger
  HAL_TIM_Base_Start(&htim6);

  //Start DAC DMA to initial configuration
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sintable, (uint32_t) tablesize, DAC_ALIGN_12B_R);

  //Setup Push Button (C13) as an interrupt.
  EXI_GPIO_Config();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 452;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel2_3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void EXI_GPIO_Config(void){

//Enable the GPIOC AHB CLOCK
	__HAL_RCC_GPIOC_CLK_ENABLE();

//Configure GPIO Pin(s)
/* Commenting out this code because I already initialize
 * the GPIO to be an input and specified interrupt
 * config.*/
//	GPIO_InitTypeDef BlueButton;
//	BlueButton.Pin = B1_Pin;
//	BlueButton.Mode = GPIO_MODE_IT_FALLING;
//	BlueButton.Pull = GPIO_PULLUP;
//	HAL_GPIO_Init(GPIOC, &BlueButton;

//Enable and set the Interupt line for the Push Button to lowest priority
	HAL_NVIC_SetPriority(EXTI4_15_IRQn,2,0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if (GPIO_Pin == GPIO_PIN_13)
	  {
	    /* Toggle LED2 */
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		WaveChange();
	  }
}


static void WaveChange(void){

	if(waveselect < 2
			){

		waveselect++;

	}else{

		waveselect = 0;
	}

	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);

	switch(waveselect){

	case 0:
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sintable, (uint32_t) tablesize, DAC_ALIGN_12B_R);
		break;

	case 1:
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) sawtable, (uint32_t) tablesize, DAC_ALIGN_12B_R);
		break;

	case 2:
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) driptable, (uint32_t) tablesize, DAC_ALIGN_12B_R);
		break;

	default:
		break;
	}


}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

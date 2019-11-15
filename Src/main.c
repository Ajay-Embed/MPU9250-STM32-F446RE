
#include "main.h"
#include "MPU9250.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define BUFFER_LEN 50

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);

RawData_Def myAccelRaw, myGyroRaw;

typedef struct{
	char x[sizeof(myAccelRaw.x)];
	char y[sizeof(myAccelRaw.y)];
	char z[sizeof(myAccelRaw.z)];
}char_t;


int main(void)
{

MPU_ConfigTypeDef myMPUConfig;

  HAL_Init();

  SystemClock_Config();


  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();


//Initializing the MPU, by passing the pointeR to the I2C handle
MPU9250_Init(&hi2c1);

//configure the accelerometer and the gyroscope parameter
myMPUConfig.Accel_Full_Scale 					= AFS_SEL_4g;
myMPUConfig.ClockSource							= Internal_8MHz;
myMPUConfig.CONFIG_DLPF							= DLPF_184A_188G_Hz;
myMPUConfig.Gyro_Full_Scale						= FS_SEL_500;
myMPUConfig.Sleep_Mode_Bit						= 0; //zero is for ON mode and 1 is for sleep mode // We can set it alternatively 1 if issuing wake up interrupts


MPU9250_Config(&myMPUConfig);


//ScaledData_Def myAccelScaled, myGyroScaled;




 while (1)
 {

	    MPU9250_Get_Accel_RawData(&myAccelRaw);				//Raw Accel data
		MPU9250_Get_Gyro_RawData(&myGyroRaw);				//Raw Gyro data



		char_t buffer;

		itoa(myAccelRaw.x,buffer.x,10);
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer.x, sizeof(myAccelRaw), HAL_MAX_DELAY);

		char next_line=',';
		uint8_t *ptr=&next_line;
		HAL_UART_Transmit(&huart2, ptr, sizeof(myAccelRaw), HAL_MAX_DELAY);

		itoa(myAccelRaw.y,buffer.y,10);
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer.y, sizeof(myAccelRaw), HAL_MAX_DELAY);

		next_line=',';
		ptr=&next_line;
		HAL_UART_Transmit(&huart2, ptr, sizeof(myAccelRaw), HAL_MAX_DELAY);

		itoa(myAccelRaw.y,buffer.y,10);
		HAL_UART_Transmit(&huart2, (uint8_t*)buffer.y, sizeof(myAccelRaw), HAL_MAX_DELAY);

		next_line='\r';
		ptr=&next_line;
		HAL_UART_Transmit(&huart2, ptr, sizeof(myAccelRaw), HAL_MAX_DELAY);

		next_line='\n';
		ptr=&next_line;
		HAL_UART_Transmit(&huart2, ptr, sizeof(myAccelRaw), HAL_MAX_DELAY);

		//		MPU9250_Get_Accel_Scale();						//Scaled 	Accelerometer data
		//		MPU9250_Get_Gyro_Scale();						//Scaled 	Gyroscope data

		//TOGGLE PIN 5 OF GPIOA SO THAT WE CAN KNOW IF THE CODE IS HANGING IN THE ERROR HANDLER OR NOT -PA5
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		HAL_Delay(100);
 }

}





/*
*************************************STM CUBE GENERATED HAL DRIVERS***********************************************************
*/



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

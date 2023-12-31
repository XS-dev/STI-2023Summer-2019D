/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
int Chande_flag = 0;
char screen_message[30] = {0};
uint32_t frequency = 1000;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
ALIGN_32BYTES (uint16_t adc1_data[10000])  __attribute__((section(".ARM.__at_0x30020000")));
ALIGN_32BYTES (uint16_t adc3_data[2000]) 	__attribute__((section(".ARM.__at_0x30000000")));

//ALIGN_32BYTES (uint16_t comp[1000]) 	__attribute__((section(".ARM.__at_0x300200000")));
uint16_t comp[2000] = {0};
__IO uint8_t AdcConvEnd = 0;//表示adc采集完成__at_0x24000000
float scan_data[480] = {0};
float Upper_K = 0;
float InputRes = 0;

float InputAC = 0;

float InputAC_kailu = 0;

float OutputAC = 0;

float OutputResAC = 0;

float OutputRes = 0;

float Gain = 0;

uint16_t Error_flag = 0;

float Check_OutputRes = 0;

float Check_InputRes = 0;

float Check_Gain = 0;

float Check_DC = 0;

float OutputDC = 0;

uint16_t Check_flag = 0;

float CheckAC = 0;

void adc_saopin(void)
{
//	HAL_ADC_DeInit(&hadc1);
	MX_ADC1_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后面    
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1_data, 1000) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim15);
	while (!AdcConvEnd);
	AdcConvEnd = 0;
	HAL_ADC_DeInit(&hadc1);
}



void adc_init(void)
{
//	HAL_ADC_DeInit(&hadc1);
	MX_ADC1_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后面    
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准

	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1_data, 10000) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim15);
	while (!AdcConvEnd);
	AdcConvEnd = 0;
	HAL_ADC_DeInit(&hadc1);
}






void adc3_init(void)
{
//	HAL_ADC_DeInit(&hadc3);
	MX_ADC3_Init();	//初始化调用放这里, 确保在MX_DMA_Init()初始化后面    
	HAL_Delay(100);	//有地方说这里可以等等电压稳定后再校准

	if (HAL_ADCEx_Calibration_Start(&hadc3, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK)
	{
			printf("hadc1 error with HAL_ADCEx_Calibration_Start\r\n");
			Error_Handler();
	}

	if (HAL_ADC_Start_DMA(&hadc3, (uint32_t *)adc3_data, 2000) != HAL_OK)
	{
			printf("hadc3 error with HAL_ADC_Start_DMA\r\n");
			Error_Handler();
	}

	HAL_TIM_Base_Start(&htim1);
	while (!AdcConvEnd);
	AdcConvEnd = 0;
		HAL_ADC_DeInit(&hadc3);

}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//系统初始化代码
//	{
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	uint32_t saopin = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM7_Init();
  MX_TIM6_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_ADC3_Init();
  MX_TIM15_Init();
  MX_TIM1_Init();
  MX_UART7_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
//	}
	Init_ad9910();
	Write_Amplitude(100);
	Freq_convert(frequency);
	Close_All();
	show_x_ray();
	for(int i = 0; i < 480; i++)
	{
		sprintf(screen_message,"add 10,0,%d\xff\xff\xff",(int)(0));
		USART7PutString(screen_message);
	}
	//开始测量输入电阻
//	{
	//继电器打开输入电阻测量电路，进入开路串联电阻状态
	Start_Input_Resistor_Measure();
	Delay_Ms(200);
  //测量输入部分开路电压
	adc3_init();
	Mea_Input(1);
	//继电器关闭输入电阻测量电路，短路串联电阻
	Stop_Input_Resistor_Measure();	
	Delay_Ms(200);
	
	//测量短路输入电压
	adc3_init();
	Mea_Input(2);	
	//显示输入电阻阻值
	show_Input_res(InputRes);
	
//	sprintf(screen_message,"x0.val=%d\xff\xff\xff",(uint16_t)(InputAC_kailu*100));
//	USART7PutString(screen_message);
//	
//	sprintf(screen_message,"x3.val=%d\xff\xff\xff",(uint16_t)(InputAC*100));
//	USART7PutString(screen_message);
	
	
	Start_Input_Resistor_Measure();
	Stop_Input_Resistor_Measure();
	Delay_Ms(200);
//	}
	
	//开始测量输出电阻
//	{
	//输出交流信号进行采样，并联电阻
	Start_Output_Resistor_Measure();
	//改变输出通道adc采样频率到100KHz
	TIM15->PSC = 150-1;	
	//测量输出电压
	adc_init();
	Mea_Output(1);

	//输出交流信号进行采样，不并联电阻
	Open_DC();
	//测量输出电压
	adc_init();
	Mea_Output(2);	
	show_Output_res(OutputRes);
	Init_ad9910();
	Write_Amplitude(100);
	Freq_convert(frequency);
	Open_DC();
	
	Gain = OutputAC/InputAC*76.019;
	if(Gain < 10)
	{
		Gain = 0.90+Gain/10;
	}
	show_Gain(Gain);
	Mea_Scan();
	Upper_K_Find();
	
	frequency = 1000;
	Freq_convert(frequency);
	TIM15->PSC = 150-1;	

//Start_Input_Resistor_Measure();
	HAL_TIM_Base_Start_IT(&htim3);

	Check_Gain = Gain+3;	

 float C2_Gain = Gain+2.5;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  

	
  while (1)
  {
		if(Check_flag == 1)
		{
			Open_DC();
//			printf("here_begin\n");
			
			frequency = 1000;
			Freq_convert(frequency);
			adc_saopin();

			Check_DC = Mea_Scan_DC();
			
			if( Check_DC > 2600)
			{
				printf("2600\n");
				
				Check_flag = 0;
				if(Check_DC < 2850)
				{
					Error_flag = 5;
					
					Check_flag = 0;
					Delay_Ms(3000);
				}
				else	
				{
					//继电器打开输入电阻测量电路，进入开路串联电阻状态
					Start_Input_Resistor_Measure();
					Delay_Ms(10);
					//测量输入部分开路电压
					adc3_init();
					Mea_Input(1);
					//继电器关闭输入电阻测量电路，短路串联电阻
					Stop_Input_Resistor_Measure();	
					Delay_Ms(10);
					
					//测量短路输入电压
					adc3_init();
					Mea_Input(2);	
					//显示输入电阻阻值
//					show_Input_res(InputRes);
					if(InputRes < 5)
					{
//							printf("%f\r\n", InputRes);
							if(InputRes < 1.5)
							{
								Error_flag = 6;
							}
							else
							{
								Error_flag = 7;
							}
					}		
					else
					{
						if(InputRes > 14.5)
						{
							Error_flag = 1;
						}
						else{Error_flag = 4;}					
					}		
				}		
			}			
			
			else if( Check_DC >= 1500)
			{
								printf(">1500\n");
				Close_All();
				Freq_convert(1000);
				//继电器打开输入电阻测量电路，进入开路串联电阻状态
				Start_Input_Resistor_Measure();
				Delay_Ms(10);
				//测量输入部分开路电压
				adc3_init();
				Mea_Input(1);
				//继电器关闭输入电阻测量电路，短路串联电阻
				Stop_Input_Resistor_Measure();	
				Delay_Ms(10);
				
				//测量短路输入电压
				adc3_init();
				Mea_Input(2);	
				InputRes = fabs(InputRes);
//				printf("%f\r\n", InputRes);			
				if((InputRes == 0) ||  (InputRes > 100))
				{
					Error_flag = 9;
				}
				else if(InputRes > 10 && (InputRes < 20))
				{	
					Error_flag = 10;		

					printf("here\n");
				}
				else
				{
					Open_DC();
					frequency = 200000;
					Freq_convert(frequency);
					TIM15->PSC = 5-1;	
					adc_saopin();
					CheckAC = Mea_Scan_Output();
//					show_Gain(CheckAC);
//					printf("%f\r\n", CheckAC);
					if(CheckAC >350)
					{	
						Error_flag = 11;
					}
					else if((CheckAC <230)&&(CheckAC >210))
					{	
						Error_flag = 14;
					}	
					//if((CheckAC <=320)&&(CheckAC >265))
					else 
					{	
						//输出交流信号进行采样，不并联电阻
						Open_DC();
						//测量输出电压
						Freq_convert(1000);
						Check_Gain = Gain+2;
						TIM15->PSC = 150-1;	
						adc_saopin();
						CheckAC = Mea_Scan_Output();

						Gain = OutputAC/InputAC*80.819;
						show_Gain(Gain);
//						show_Gain(Gain);
//						if(Gain<C2_Gain)
						if(Gain>167)
						{
							Error_flag = 13;
						}
						else
						{
//							frequency = 20;
//							Close_All();							
//							Freq_convert(frequency);
//							Mea_Input_Low();	
//							Delay_Ms(10);
//							//测量短路输入电压
//							TIM1->PSC = 3000-1;	
//							adc3_init();
//							Mea_Input(2);	
//							
//							Open_DC();
//							TIM15->PSC = 3000-1;	
//							adc_saopin();
//							for(u16 ordertemp=1;ordertemp<1000;ordertemp++)
//							{				
//								printf("%d\r\n", adc1_data[ordertemp]);
//							}
//							CheckAC = Mea_Scan_Low();
//							Gain = OutputAC/InputAC*100;
//							//printf("%f\r\n", Gain);		
//							show_Gain(Gain);
							if(Gain>10.87)
							{
								Error_flag = 0;
							}
							else
							{
								Error_flag = 0;
							}
							
						}
					}	

				}
			}
			
			
			else if( Check_DC < 1500)
			{
				printf("<1500\n");
//				printf("%f\r\n", Check_DC);
				if(Check_DC>800)
				{
					Error_flag  =2;
				}
				else if(Check_DC>30)
				{
					Error_flag = 3;
				}
				else
				{
//					Close_All();
//					Freq_convert(1000);
//					//继电器打开输入电阻测量电路，进入开路串联电阻状态
//					Start_Input_Resistor_Measure();
//					Delay_Ms(10);
//					//测量输入部分开路电压
//					adc3_init();
//					Mea_Input(1);
//					//继电器关闭输入电阻测量电路，短路串联电阻
//					Stop_Input_Resistor_Measure();	
//					Delay_Ms(10);
//					
//					//测量短路输入电压
//					adc3_init();
//					Mea_Input(2);	
//					InputRes = fabs(InputRes);
//					if(InputRes < 1)
//					{
					Error_flag = 8;
//					}
//					printf("DC=%d\n",Check_DC);
//					printf("Error_flag=%d\n",Error_flag);
				}
			}

			
			printf("here_end\n");
			
			display_Error();			
			Check_flag = 0;
		}

//		
//		
//		
//		
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 360;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
   if(hadc->Instance == ADC1) 
	{
      //SCB_InvalidateDCache_by_Addr((uint32_t *) &adc1_data[0], ADC1_BUFFER_SIZE);
   }
	 AdcConvEnd = 1;
}

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x24000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_128KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.BaseAddress = 0x30020000;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0x30040000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.BaseAddress = 0x38000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

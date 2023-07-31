/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "system.h"
#include "motor.h"
#include "servo.h"
#include "track.h"
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

/* USER CODE BEGIN PV */
uint8_t UART1_RX;
uint8_t Servo_Control_State = SERVO_0_GET;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Motor_Init();
  Track_Init();
  Servo_Init();
	Move_X = 0;
  HAL_TIM_Base_Start_IT(&PERIOD_INTERRUPT_TIM_HANDLER);
  HAL_UART_Receive_IT(&huart1, &UART1_RX, 1);
  HAL_UART_Receive_IT(&TRACK_TASK_UART_HANDLER, &Track_UART_Rx_Byte, 1);
  HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_0.Response_UART_Rx.Response_Temp, 10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		Servo_Set_PWM(0x00, 500);
//		HAL_Delay(5);
//		Servo_Set_PWM(0x01, 500);
//		HAL_Delay(5);
    //Determine_Angle();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  // 10ms
{
  if (htim == &PERIOD_INTERRUPT_TIM_HANDLER)
  {
    Measure_Motor_Speed();
    Solve_Speed(Move_X, Move_Y, Move_Z);
    Update_Motor_PID();
    Motor_Set_PWM();
		
		switch (Servo_Control_State)
    {
    case SERVO_0_GET:
      Servo_Get_Position(0x00);
      HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_0.Response_UART_Rx.Response_Temp, 10);
      break;

    case SERVO_0_SET:
      if (Servo_Control_Mode == SERVO_CONTROL_BY_PWM)
      {
        Servo_Set_PWM(0x00, Servo_0.Target_PWM);
      }
      else if (Servo_Control_Mode == SERVO_CONTROL_BY_ANGLE)
      {
        Servo_Set_Angle(0x00, Servo_0.Target_Angle);
      }
      Servo_Control_State = SERVO_1_GET;
      break;

    case SERVO_1_GET:
      Servo_Get_Position(0x01);
      HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_1.Response_UART_Rx.Response_Temp, 10);
      break;
      
    case SERVO_1_SET:
      if (Servo_Control_Mode == SERVO_CONTROL_BY_PWM)
      {
        Servo_Set_PWM(0x01, Servo_1.Target_PWM);
      }
      else if (Servo_Control_Mode == SERVO_CONTROL_BY_ANGLE)
      {
        Servo_Set_Angle(0x01, Servo_1.Target_Angle);
      }
      Servo_Control_State = SERVO_0_GET;
      break;
    
    default:
      Servo_Control_State = SERVO_0_GET;
      break;
    }
		
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart1)
  {
    HAL_UART_Receive_IT(&huart1, &UART1_RX, 1);
  }
  else if (huart == &TRACK_TASK_UART_HANDLER)
  {
    Coordinates_UART_Rx_Byte();
    HAL_UART_Receive_IT(&TRACK_TASK_UART_HANDLER, &Track_UART_Rx_Byte, 1);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart == &SERVO_UART_HANDLER)
  {
    switch (Servo_Control_State)
    {
    case SERVO_0_GET:
      memcpy(Servo_0.Response_UART_Rx.Response, Servo_0.Response_UART_Rx.Response_Temp, 10);
      Servo_0.Current_PWM = Get_PWM_From_Response(Servo_0.Response_UART_Rx.Response);
      Servo_0.Current_Angle = Servo_0.Current_PWM * (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (SERVO_PWM_MAX - SERVO_PWM_MIN) + SERVO_ANGLE_MIN;
      HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_1.Response_UART_Rx.Response_Temp, 10);
			Servo_Control_State = SERVO_0_SET;
      break;

    case SERVO_1_GET:
      memcpy(Servo_1.Response_UART_Rx.Response, Servo_1.Response_UART_Rx.Response_Temp, 10);
      Servo_1.Current_PWM = Get_PWM_From_Response(Servo_1.Response_UART_Rx.Response);
      Servo_1.Current_Angle = Servo_1.Current_PWM * (SERVO_ANGLE_MAX - SERVO_ANGLE_MIN) / (SERVO_PWM_MAX - SERVO_PWM_MIN) + SERVO_ANGLE_MIN;
      HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_0.Response_UART_Rx.Response_Temp, 10);
      Servo_Control_State = SERVO_1_SET;
      break;
    
    default:
      HAL_UARTEx_ReceiveToIdle_DMA(&SERVO_UART_HANDLER, Servo_1.Response_UART_Rx.Response_Temp, 10);
      break;
    }
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

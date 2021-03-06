/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adxl345.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART2_BUFFER_SIZE 10 //8 Byte of data with a start and a stop byte
#define TRANSMISSION_START 0x02
#define TRANSMISSION_END 0x04

//Commands define (enum too small, only 32 bit)
#define HANDSHAKE 0x0000AA55AA55
#define SET_TIME 0x0000C0FFEE11
#define START_READING 0x0000ABCDDCBA
#define STOP_READING 0x0000DCBAABCD
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t UART2_rxBuffer[UART2_BUFFER_SIZE];
char message[256];
uint64_t utc_timestamp_ms = 0; //Holds the timestamp of each measurement
bool handshake = false;
bool send = false;
bool setting_time = false;
//enum commands {HANDSHAKE = 0x0000AA55AA55, SET_TIME = 0x0000C0FFEE11, START_READING = 0x0000ABCDDCBA, STOP_READING = 0x0000DCBAABCD};
//enum commands rcv_command;
uint64_t rcv_command = 0;
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
    //Initialize the DMA callback (will be called when we receive an instruction from the PC on UART)
    HAL_UART_Receive_DMA(&huart2, UART2_rxBuffer, UART2_BUFFER_SIZE);
    HAL_TIM_Base_Start_IT(&htim10);

    setTargetDeviceAddress(&hi2c1, 0x53 << 1);

    HAL_StatusTypeDef status;
    int id = 0;
    Accelerations accelerometerData;

    //Initialize the module
    status = AccelInit(&hi2c1);
    if (status == HAL_OK)
    {
        sprintf(message, "ADXL345 Module has been initialized successfully!\n");
        HAL_UART_Transmit(&huart2, (uint8_t *)&message, 50, 100);
    }
    else
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"ERROR!\n", 7, 100);
    }

    //Get ID of Accelerometer module (ensures the connection is working correctly)
    status = AccelGetId(&hi2c1, &id);
    if (status == HAL_OK)
    {
        sprintf(message, "ADXL345 ID is : %o\n", id);
        HAL_UART_Transmit(&huart2, (uint8_t *)&message, 20, 100);
    }
    else
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)"ERROR!\n", 7, 100);
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        if (handshake && send)
        {
            HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
            status = getAccelerations(&hi2c1, &accelerometerData);
            if (status == HAL_OK)
            {
                memset(message, 0, sizeof(message));
                // float xf = (float)accelerometerData.x * 3.9f / 1000.0;
                // float yf = (float)accelerometerData.y * 3.9f / 1000.0;
                // float zf = (float)accelerometerData.z * 3.9f / 1000.0;
                // sprintf(message, "X : %5.5f    Y : %5.5f    Z : %5.5f\n", xf, yf, zf);
                // Add the accelerometer data to the message to send
                message[0] = (accelerometerData.x & 0xFF00) >> 8;
                message[1] = (accelerometerData.x & 0x00FF);
                message[2] = (accelerometerData.y & 0xFF00) >> 8;
                message[3] = (accelerometerData.y & 0x00FF);
                message[4] = (accelerometerData.z & 0xFF00) >> 8;
                message[5] = (accelerometerData.z & 0x00FF);
                for (int i = 0; i < 8; ++i)
                {
                    uint64_t offset = (8 << 3) - ((i + 1) << 3);
                    message[6 + i] = (utc_timestamp_ms & ((uint64_t)0xFF << offset)) >> offset;
                }
                message[14] = 0xAA;
                message[15] = 0x55;
                HAL_UART_Transmit(&huart2, (uint8_t *)message, 16, 100);
            }
            else
            {
                sprintf(message, "Error!\n");
                HAL_UART_Transmit(&huart2, (uint8_t *)message, 7, 100);
            }
        }
        HAL_Delay(1);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 90;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

int modulo(int a, int b)
{
    if (b < 0)
        return modulo(-a, -b);
    const int result = a % b;
    return result >= 0 ? result : result + b;
}

// This function is called when the UART2_rxBuffer gets full
// we can than check it's content to know which command was sent to us
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    //Find start of command (if we ever miss a byte, this serves as an alignement byte since we use a circular buffer)
    int i = 0;
    for (i = 0; i < UART2_BUFFER_SIZE; ++i)
    {
        if (UART2_rxBuffer[i] == TRANSMISSION_START)
        {
            while (UART2_rxBuffer[modulo(i - 1, UART2_BUFFER_SIZE)] != TRANSMISSION_END)
                ; //Wait for the complete command to get transfered
            i += 1;
            break;
        }
    }
    rcv_command = 0;
    for (int j = 0; j < UART2_BUFFER_SIZE - 2; ++j)
    {
        rcv_command += ((uint64_t)UART2_rxBuffer[modulo(i + j, UART2_BUFFER_SIZE)] << (((UART2_BUFFER_SIZE - 2) << 3) - ((j + 1) << 3)));
    }
    if (setting_time)
    {
        setting_time = false;
        utc_timestamp_ms = rcv_command;
    }
    else
    {
        switch (rcv_command)
        {
        case HANDSHAKE:
            handshake = true;
            break;
        case SET_TIME:
            setting_time = true;
            break;
        case START_READING:
            if (handshake)
                send = true;
            break;
        case STOP_READING:
            if (handshake)
                send = false;
            break;
        default:
            memset(message, 0, sizeof(message));
            sprintf(message, "Error during transmission, received : 0x%08x", rcv_command);
            HAL_UART_Transmit(&huart2, message, 50, 100);
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim10){
        utc_timestamp_ms++;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

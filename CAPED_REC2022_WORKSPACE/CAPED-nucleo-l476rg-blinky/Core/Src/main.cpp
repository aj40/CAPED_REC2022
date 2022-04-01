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
#include "cmsis_os.h"
#include "AccelStepper.hh"
#include "Adafruit_PWMServoDriver.h"
#include "../../Drivers/Adafruit_VL53L0X-master/src/Adafruit_VL53L0X.h"

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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TowerTransition */
osThreadId_t TowerTransitionHandle;
const osThreadAttr_t TowerTransition_attributes = {
  .name = "TowerTransition",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for QueueToTop */
osThreadId_t QueueToTopHandle;
const osThreadAttr_t QueueToTop_attributes = {
  .name = "QueueToTop",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ActuationDrop */
osThreadId_t ActuationDropHandle;
const osThreadAttr_t ActuationDrop_attributes = {
  .name = "ActuationDrop",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
void StartDefaultTask(void *argument);
void StartTowerTransition(void *argument);
void StartQueueToTop(void *argument);
void StartActuationDrop(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static bool drop1Complete = false;

void unlockCabinServo(int i){
	if(i == 1){
		//TODO move mapped servo for lift tower
	}
	else{
		//TODO move mapped servo for drop tower
	}
}

void lockCabinServo(int i){
	if(i == 1){
		//TODO move mapped servo for lift tower
	}
	else{
		//TODO move mapped servo for drop tower
	}
}
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  //osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  //defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of TowerTransition */
  //TowerTransitionHandle = osThreadNew(StartTowerTransition, NULL, &TowerTransition_attributes);

  /* creation of QueueToTop */
  //QueueToTopHandle = osThreadNew(StartQueueToTop, NULL, &QueueToTop_attributes);

  /* creation of ActuationDrop */
  //ActuationDropHandle = osThreadNew(StartActuationDrop, NULL, &ActuationDrop_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  //osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // called this way, it uses the default address 0x40
  //Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
  // you can also call it with a different address you want
  //Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
  // you can also call it with a different address and I2C interface
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, &hi2c3);

  // Depending on your servo make, the pulse width min and max may vary, you
  // want these to be as small/large as possible without hitting the hard stop
  // for max range. You'll have to tweak them as necessary to match the servos you
  // have!
  #define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
  #define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
  #define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
  #define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
  #define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

  // our servo # counter
  uint8_t servonum = 0;

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  HAL_Delay(10);

  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
      pwm.setPWM(servonum, 0, pulselen);
  }

  HAL_Delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
      pwm.setPWM(servonum, 0, pulselen);
  }

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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x10909CEC;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Servo_EN_Pin|Step2_step_Pin|Step2_en_Pin|Step1_dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|Step2_dir_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Step1_step_GPIO_Port, Step1_step_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Step1_en_GPIO_Port, Step1_en_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Servo_EN_Pin Step2_step_Pin Step2_en_Pin Step1_dir_Pin */
  GPIO_InitStruct.Pin = Servo_EN_Pin|Step2_step_Pin|Step2_en_Pin|Step1_dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ACC_Z_Pin */
  GPIO_InitStruct.Pin = ACC_Z_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACC_Z_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin Step2_dir_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|Step2_dir_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IR1_Pin IR2_Pin Override3_Pin Override2_Pin
                           Override1_Pin RIDESTOP_Pin ESTOP_Pin */
  GPIO_InitStruct.Pin = IR1_Pin|IR2_Pin|Override3_Pin|Override2_Pin
                          |Override1_Pin|RIDESTOP_Pin|ESTOP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IR3_Pin IR4_Pin Limit2_Pin Limit1_Pin
                           Override5_Pin Override4_Pin */
  GPIO_InitStruct.Pin = IR3_Pin|IR4_Pin|Limit2_Pin|Limit1_Pin
                          |Override5_Pin|Override4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : IR5_Pin IR6_Pin IR7_Pin Limit6_Pin
                           Limit5_Pin Limit4_Pin Limit3_Pin IR11_Pin
                           IR10_Pin IR9_Pin IR8_Pin */
  GPIO_InitStruct.Pin = IR5_Pin|IR6_Pin|IR7_Pin|Limit6_Pin
                          |Limit5_Pin|Limit4_Pin|Limit3_Pin|IR11_Pin
                          |IR10_Pin|IR9_Pin|IR8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Step1_step_Pin */
  GPIO_InitStruct.Pin = Step1_step_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Step1_step_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Step1_en_Pin */
  GPIO_InitStruct.Pin = Step1_en_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Step1_en_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTowerTransition */
/**
* @brief Function implementing the TowerTransition thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTowerTransition */
void StartTowerTransition(void *argument)
{
  /* USER CODE BEGIN StartTowerTransition */
  /* Infinite loop */
  for(;;)
  {
	  /* If:
	  	 * - The LIFT tower top IR receiver is triggered by the cabin IR emitter (IR4 - PC5) AND
	  	 * - The top LIFT tower limit switch is activated (Limit1 - PC7) AND
	  	 * - The DROP tower top IR receiver is triggered by the cabin IR emitter (IR5 - PB0) AND
	  	 * - The top DROP tower limit switches is activated (Limit2 - PC6)
	  	 */
	  	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) && HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) && HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)) {
	  		// First, unlock the cabin servo holding the RV

	  		unlockCabinServo(1);

	        //TODO: I2C communication to mapped servo.

	  		// Wait a 1/2 second for the servo to unlock, then drive the stepper motor (STEPPER1_EN - PB3)
	  		osDelay(500);
	  		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	  	}
  }
  /* USER CODE END StartTowerTransition */
}

/* USER CODE BEGIN Header_StartQueueToTop */
/**
* @brief Function implementing the QueueToTop thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartQueueToTop */
void StartQueueToTop(void *argument)
{
  /* USER CODE BEGIN StartQueueToTop */
  /* Infinite loop */
  for(;;)
  {
    /*
     * Queue IR osDelay, input TOF, Cabin IR, bottom limit switch,
     * outputs of drive motors and brakes into tower, servo to lock,
     * and stepper, serial monitor
     */

	 //if something in queue
	 if(HAL_GPIO_ReadPin(GPIOA, IR1_Pin)){

		 //wait to load
		 osDelay(10000);

		 //if cabin at the bottom
		 if(HAL_GPIO_ReadPin(GPIOA, IR2_Pin) && HAL_GPIO_ReadPin(GPIOC, IR3_Pin)){
			 //TODO drive servos

			 //TODO set brakes based on TOF

			 //TODO drive lock servo
			 lockCabinServo(1);

			 // Wait a 1/2 second for the servo to unlock, then drive the stepper motor (STEPPER2_EN)
			 osDelay(500);
			 HAL_GPIO_WritePin(GPIOC, Step2_en_Pin, GPIO_PIN_SET);
		 }

	 }

  }
  /* USER CODE END StartQueueToTop */
}

/* USER CODE BEGIN Header_StartActuationDrop */
/**
* @brief Function implementing the ActuationDrop thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartActuationDrop */
void StartActuationDrop(void *argument)
{
  /* USER CODE BEGIN StartActuationDrop */
  /* Infinite loop */
  for(;;)
  {
	 //if drop complete, and at the top, amd wait clear
    if(drop1Complete && HAL_GPIO_ReadPin(GPIOB, Limit4_Pin) && HAL_GPIO_ReadPin(GPIOB, IR11_Pin)){
    	//TODO actuation servo

    	//if actuated and still wait clear
    	if(HAL_GPIO_ReadPin(GPIOB, Limit6_Pin) && HAL_GPIO_ReadPin(GPIOB, IR11_Pin)){
    		//drop
    		HAL_GPIO_WritePin(GPIOB, Step1_en_Pin, GPIO_PIN_SET);
    		drop1Complete = true;
    	}
    }
  }
  /* USER CODE END StartActuationDrop */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
   //test
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
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


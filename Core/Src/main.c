/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attentionF
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
#include "fonts.h"
#include "ssd1306.h"
#include <stdbool.h>
#include <time.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
int block[7][4][4][4] = {
	{ // T모양 블럭
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{0,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{    // 번개 블럭
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,1,0,0}
		}
	},
	{   // 번개 블럭 반대
		{   
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{1,0,0,0}
		}
	},
	{   // 1자형 블럭
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		},
		{
			{0,0,0,0},
			{0,0,0,0},
			{1,1,1,1},
			{0,0,0,0}
		}
	},
	{   // L자형 블럭
		{
			{0,0,0,0},
			{1,0,0,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{1,0,0,0},
			{1,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{0,0,1,0},
			{0,0,0,0}
		},
		{
			{0,1,0,0},
			{0,1,0,0},
			{1,1,0,0},
			{0,0,0,0}
		}
	},
	{   // L자형 블럭 반대
		{
			{0,0,0,0},
			{0,0,1,0},
			{1,1,1,0},
			{0,0,0,0}
		},
		{
			{1,0,0,0},
			{1,0,0,0},
			{1,1,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0},
			{1,0,0,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		}
	},
	{   // 네모 블럭
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0},
			{0,1,1,0},
			{0,0,0,0}
		}
	}
};
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int map[15][7]={0,};      
int blockForm;
int blockRotation=0;

volatile int count=0;
int x_pos[16] = {1, 9, 17, 25, 33, 41, 49, 57, 65, 73, 81, 89, 97, 105, 113, 121}; 
int y_pos[7] = {4, 12, 20, 28, 36, 44, 52};

int x_nowpos=14;
int y_nowpos=2;

int x_target=0;
int y_target=0; 

int score=0;
int cnt_target=0;

volatile int flag=1;

char rx_buff; 
char tx_buff;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void StartScreen();
void DrawMap() ;
void DrawBlock();
void CreateRandomForm();
void DropBlock();
bool CheckCrash(int x, int y); 
void BlockToGround();
void RemoveLine();
void Game_init();
bool Gameover(); 
void EndScreen();

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */
char rx_buff; 
char tx_buff;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_I2C1_Init(void);
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
  MX_LPUART1_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT (&hlpuart1, &rx_buff, 1);
  uint8_t str[] = "uploade check\n\r"; 
  HAL_UART_Transmit(&hlpuart1, (uint8_t*)str, 16, 1000);
  
  srand(time(NULL)); // 난수발생 
  
  SSD1306_Init();
  SSD1306_Fill(0);

   
Restart :
  StartScreen();
 flag=1;
//  Game_init();
  while(flag){;}
  HAL_Delay(10);
  CreateRandomForm();
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        DrawMap();
    DrawBlock();
    DropBlock();
    BlockToGround();
    RemoveLine();
    if(Gameover()==true) {
      EndScreen();
      goto Restart;}
    SSD1306_UpdateScreen();
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hi2c1.Init.Timing = 0x30A0A7FB;
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
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void USART1_IRQHandler(void){
  HAL_UART_IRQHandler(&hlpuart1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  if(huart->Instance == LPUART1){
    tx_buff=rx_buff;
    HAL_UART_Transmit(&hlpuart1, &tx_buff, 1, 10);
    if (tx_buff=='q') flag=0;
    if (tx_buff=='d') {
      if (CheckCrash(x_nowpos , y_nowpos+1) == false) y_nowpos++; }
    if (tx_buff=='a') {
      if (CheckCrash(x_nowpos , y_nowpos-1) == false) y_nowpos--; }
    if (tx_buff=='w') {
      if (y_nowpos<4) blockRotation=(blockRotation+1)%4;
      else {
         for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][(blockRotation+1)%4][i][j] == 1) {
                          if (y_nowpos+j>8) continue;
                          else blockRotation=(blockRotation+1)%4;
			}
		}}}}
    HAL_UART_Receive_IT(&hlpuart1, &rx_buff, 1);}
}


void CreateRandomForm() {
	blockForm = rand() % 7; // test 가로 3 
}


void StartScreen(){
  
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    
  SSD1306_GotoXY(0, 100);
  char sentence[7]= "Tetris";
    for (int i=0; i<sizeof(sentence)-1; i++){
      SSD1306_Screen(*(sentence+i), &Font_11x18, 1);
    } 
    
  SSD1306_GotoXY(0, 70);
   char sentence_[]= "Press";
       for (int i=0; i<sizeof(sentence_)-1; i++){
      SSD1306_Screen(*(sentence_+i), &Font_7x10, 1);
    } 
   
    SSD1306_GotoXY(0, 55);
   char sentence__[]= "the swch";
       for (int i=0; i<sizeof(sentence__)-1; i++){
      SSD1306_Screen(*(sentence__+i), &Font_7x10, 1);
    } 
   
    SSD1306_GotoXY(0, 40);
   char sentence___[]= "to start";
       for (int i=0; i<sizeof(sentence___)-1; i++){
      SSD1306_Screen(*(sentence___+i), &Font_7x10, 1);
    } 
    
    SSD1306_UpdateScreen();     
}




void DrawMap() {
  
    SSD1306_Fill(SSD1306_COLOR_BLACK);
    
    SSD1306_GotoXY(37, 126);
    int ones = score%10;
    int tens = (score/10)%10;
    int thou = score/100;
    
    SSD1306_Screen(thou+'0', &Font_7x10, 1);
    SSD1306_Screen(tens+'0', &Font_7x10, 1);
    SSD1306_Screen(ones+'0', &Font_7x10, 1);
  
    SSD1306_DrawLine(0, 61, 128, 61, 1);
    SSD1306_DrawLine(0, 3, 128, 3, 1); // 좌측 x 
    SSD1306_DrawLine(0, 3, 0, 61, 1); // 맨 끝 짧은거  
    // 떨어진 블록에 대해 
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 7; j++) {
			if (map[i][j] == 1) {
                          SSD1306_GotoXY(x_pos[i],y_pos[j]);    
                          SSD1306_Putc (0, &Font_8x8, 1); 
			}
		}
	}
}

void DrawBlock() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                          SSD1306_GotoXY(x_pos[x_nowpos-i],y_pos[y_nowpos+j]);    
                          SSD1306_Putc (0, &Font_8x8, 1); 
			}
		}
	}
}

void DropBlock() {
	count++;
	if (count==4) {
		 x_nowpos--;  
                count=0;
	}
}


bool CheckCrash(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
                              if(x-i<0 || y+j<0 || y+j>6  ) // **
                                     return true;
                              
                              int t =map[x-i][y+j];
				if (t==1)   
					return true;

			}
		}
	}
	return false;
}

void BlockToGround() {
	if (CheckCrash(x_nowpos-1, y_nowpos) == true) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						map[x_nowpos-i][y_nowpos+j]=1;
					}
				}
			}
                        x_nowpos=14;
                        y_nowpos=2;
                        blockRotation=0;
                        CreateRandomForm();
	}
}
 

void RemoveLine() {
  int combo=1;
	for (int i = 14; i >= 0; i--) { 
		int cnt = 0;
		for (int j = 0; j < 7; j++) { // 
			if (map[i][j] == 1) 
                          cnt++;}
                if(cnt==7){
                  memset(map[i], 0, sizeof(map[i]));
                  for (int k=i; k<=13; k++){
                  memcpy(map[k], map[k+1], sizeof(map[k]));
                  memset(map[k+1], 0, sizeof(map[k+1]));
/*
		//for (int k = 14; k >= i; k--) {
			for (int x = 0; x < 7; x++) { 
                        map[i][x] =0; // map[k+1][x]; 
                                }
			}*/
		}
                score+=10*(combo++);
	}
}
}

bool Gameover(){
  cnt_target++;
  if (cnt_target==7){
    if(y_target==y_nowpos && x_target==x_nowpos) return true;
    else {
        y_target=y_nowpos;
        x_target=x_nowpos;
       cnt_target=0;
       return false;
    }
  }
  else return false;
}



void Game_init(){

     blockRotation=0;
     count=0;
     x_nowpos=14;
     y_nowpos=2; 
     score=0;
      for ( int i = 0 ;  i < 16; i++ )
      memset( map[i], 0, sizeof(map[i]));

}
void EndScreen(){
  HAL_Delay(1500);
  SSD1306_Fill(SSD1306_COLOR_BLACK);
    
  SSD1306_GotoXY(0, 70);
  char print_[]= "score is";
  for (int i=0; i<sizeof(print_)-1; i++){
  SSD1306_Screen(*(print_+i), &Font_7x10, 1);} 
  
  SSD1306_GotoXY(0, 55);
  int ones = score%10;
  int tens = (score/10)%10;
  int thou = score/100;
    
  SSD1306_Screen(thou+'0', &Font_7x10, 1);
  SSD1306_Screen(tens+'0', &Font_7x10, 1);
  SSD1306_Screen(ones+'0', &Font_7x10, 1);
    
  SSD1306_UpdateScreen();    
  HAL_Delay(3000);
    
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

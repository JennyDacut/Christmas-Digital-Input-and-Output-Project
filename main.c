/* USER CODE BEGIN Header */
/**
  ****************************************************************************
  * @file           : main.c
  * @brief          : Main program body - Rudolph / Deck the Halls / Jingle Bells
  ****************************************************************************
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

// --- MUSIC NOTE FREQUENCIES (Hertz) ---
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define REST      0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void ControlLED_DigitalIO(uint8_t red_hi, uint8_t yellow_hi);
void ControlLEDMatrix(uint8_t red_hi, uint8_t yellow_hi);
void PlayTone(uint32_t frequency, uint32_t duration_ms);
void BuzzerMusicManager(uint8_t red_hi, uint8_t yellow_hi);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
  * @brief  Controls the RED (PF12) and YELLOW (PF11) LEDs.
  */
void ControlLED_DigitalIO(uint8_t red_hi, uint8_t yellow_hi)
{
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, red_hi    ? GPIO_PIN_RESET : GPIO_PIN_SET);   // RED
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11, yellow_hi ? GPIO_PIN_RESET : GPIO_PIN_SET);   // YELLOW
}

/**
  * @brief  Controls the LED Matrix Enable Pin (PB2).
  */
void ControlLEDMatrix(uint8_t red_hi, uint8_t yellow_hi)
{
    // Active-Low Logic (Reset = ON)
    if (red_hi || yellow_hi) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET); // Matrix ON
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);   // Matrix OFF
    }
}

/**
  * @brief  Microsecond delay for audio frequencies.
  */
void Delay_us(uint32_t us)
{
    // *** PITCH TUNING ***
    // 4 is a good baseline for higher pitch.
    volatile uint32_t cycles_per_us = 4;

    volatile uint32_t count = us * cycles_per_us;
    while(count--) {
        __NOP();
    }
}

/**
  * @brief  Generates a specific frequency on the Buzzer pin (PB0).
  */
void PlayTone(uint32_t frequency, uint32_t duration_ms)
{
    if (frequency == 0) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        HAL_Delay(duration_ms);
        return;
    }

    uint32_t period_us = 1000000 / frequency;
    uint32_t half_period = period_us / 2;
    uint32_t start_time = HAL_GetTick();

    while((HAL_GetTick() - start_time) < duration_ms)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        Delay_us(half_period);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        Delay_us(half_period);
    }
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    HAL_Delay(20); // Small gap between notes
}

/**
  * @brief  Manages Songs!
  */
void BuzzerMusicManager(uint8_t red_hi, uint8_t yellow_hi)
{
    // RED ONLY: "Rudolph the Red-Nosed Reindeer"
    if (red_hi && !yellow_hi)
    {
        // Ru-dolph the red-nosed rein-deer
        PlayTone(NOTE_G4, 150); PlayTone(NOTE_A4, 150); PlayTone(NOTE_G4, 150); PlayTone(NOTE_E4, 150);
        PlayTone(NOTE_C5, 300); PlayTone(NOTE_A4, 300); PlayTone(NOTE_G4, 400);

        // Had a ve-ry shi-ny nose
        PlayTone(NOTE_G4, 150); PlayTone(NOTE_A4, 150); PlayTone(NOTE_G4, 150); PlayTone(NOTE_A4, 150);
        PlayTone(NOTE_G4, 300); PlayTone(NOTE_C5, 300); PlayTone(NOTE_B4, 400);

        HAL_Delay(500); // Pause
    }
    // YELLOW ONLY: "Deck the Halls"
    else if (yellow_hi && !red_hi)
    {
        // Deck the halls with boughs of hol-ly
        PlayTone(NOTE_G4, 300); PlayTone(NOTE_F4, 100); PlayTone(NOTE_E4, 200); PlayTone(NOTE_D4, 200);
        PlayTone(NOTE_C4, 200); PlayTone(NOTE_D4, 200); PlayTone(NOTE_E4, 200); PlayTone(NOTE_C4, 200);

        // Fa la la la la, la la la la
        PlayTone(NOTE_D4, 100); PlayTone(NOTE_E4, 100); PlayTone(NOTE_F4, 100); PlayTone(NOTE_D4, 100);
        PlayTone(NOTE_E4, 300); PlayTone(NOTE_D4, 100); PlayTone(NOTE_C4, 400);

        HAL_Delay(500); // Pause
    }
    // ORANGE (BOTH): "Jingle Bells" (Kept as requested!)
    else if (red_hi && yellow_hi)
    {
        // Jin-gle bells, jin-gle bells
        PlayTone(NOTE_E4, 200); PlayTone(NOTE_E4, 200); PlayTone(NOTE_E4, 400);
        PlayTone(NOTE_E4, 200); PlayTone(NOTE_E4, 200); PlayTone(NOTE_E4, 400);

        // Jin-gle all the way
        PlayTone(NOTE_E4, 200); PlayTone(NOTE_G4, 200); PlayTone(NOTE_C4, 300); PlayTone(NOTE_D4, 100); PlayTone(NOTE_E4, 600);

        HAL_Delay(500);
    }
    // NO BUTTONS: Silence
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // 1. Read Buttons (Invert active-low)
    uint8_t b1 = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
    uint8_t b2 = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
    uint8_t b3 = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4);
    uint8_t b4 = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);

    // 2. Logic
    uint8_t red_hi    = ((!b4 && (!b3 || b2)) || (b4 && b3 && b2 && b1));
    uint8_t yellow_hi = ((b4 ^ b3) || (b4 && b3 && b2 && b1));

    if (b4) { // UP override
        red_hi = 1;
        yellow_hi = 1;
    }

    // 3. Control Hardware
    ControlLED_DigitalIO(red_hi, yellow_hi);
    ControlLEDMatrix(red_hi, yellow_hi);

    // 4. Play Songs
    BuzzerMusicManager(red_hi, yellow_hi);

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

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Levels */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);    // LEDs OFF
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);               // Buzzer OFF
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);                 // Matrix EN OFF (Active-Low)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);               // Matrix Data LOW

  /*Configure Buttons (PC0, PC1, PC4, PC5) */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure Buzzer (PB0) and Matrix EN (PB2) */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure Matrix Data (PA6) */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure LEDs (PF11, PF12) */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { }
#endif

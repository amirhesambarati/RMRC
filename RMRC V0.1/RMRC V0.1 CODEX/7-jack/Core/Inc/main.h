/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ENC_MENU_A_Pin GPIO_PIN_11
#define ENC_MENU_A_GPIO_Port GPIOC
#define ENC_MENU_B_Pin GPIO_PIN_12
#define ENC_MENU_B_GPIO_Port GPIOC
#define MENU_BUTTON_Pin GPIO_PIN_13
#define MENU_BUTTON_GPIO_Port GPIOC
#define MENU_BUTTON_EXTI_IRQn EXTI4_15_IRQn
#define LED10_Pin GPIO_PIN_14
#define LED10_GPIO_Port GPIOC
#define LED8_Pin GPIO_PIN_0
#define LED8_GPIO_Port GPIOC
#define LED9_Pin GPIO_PIN_1
#define LED9_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_3
#define LED1_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_0
#define LED3_GPIO_Port GPIOA
#define LED7_Pin GPIO_PIN_1
#define LED7_GPIO_Port GPIOA
#define LED6_Pin GPIO_PIN_2
#define LED6_GPIO_Port GPIOA
#define LED5_Pin GPIO_PIN_3
#define LED5_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOA
#define MOTOR_CLOSE_OUTPUT_2_Pin GPIO_PIN_5
#define MOTOR_CLOSE_OUTPUT_2_GPIO_Port GPIOA
#define MOTOR_OPEN_OUTPUT_2_Pin GPIO_PIN_6
#define MOTOR_OPEN_OUTPUT_2_GPIO_Port GPIOA
#define MOTOR_CLOSE_OUTPUT_1_Pin GPIO_PIN_7
#define MOTOR_CLOSE_OUTPUT_1_GPIO_Port GPIOA
#define MOTOR_OPEN_OUTPUT_1_Pin GPIO_PIN_0
#define MOTOR_OPEN_OUTPUT_1_GPIO_Port GPIOB
#define ENC_MOTOR1_A_Pin GPIO_PIN_1
#define ENC_MOTOR1_A_GPIO_Port GPIOB
#define ENC_MOTOR1_A_EXTI_IRQn EXTI0_1_IRQn
#define ENC_MOTOR1_B_Pin GPIO_PIN_2
#define ENC_MOTOR1_B_GPIO_Port GPIOB
#define ENC_MOTOR2_A_Pin GPIO_PIN_12
#define ENC_MOTOR2_A_GPIO_Port GPIOB
#define ENC_MOTOR2_A_EXTI_IRQn EXTI4_15_IRQn
#define ENC_MOTOR2_B_Pin GPIO_PIN_13
#define ENC_MOTOR2_B_GPIO_Port GPIOB
#define ENC_JACK2_A_Pin GPIO_PIN_14
#define ENC_JACK2_A_GPIO_Port GPIOB
#define ENC_JACK2_A_EXTI_IRQn EXTI4_15_IRQn
#define ENC_JACK2_B_Pin GPIO_PIN_15
#define ENC_JACK2_B_GPIO_Port GPIOB
#define ENC_JACK1_A_Pin GPIO_PIN_8
#define ENC_JACK1_A_GPIO_Port GPIOA
#define ENC_JACK1_A_EXTI_IRQn EXTI4_15_IRQn
#define ENC_JACK1_B_Pin GPIO_PIN_9
#define ENC_JACK1_B_GPIO_Port GPIOA
#define LED11_Pin GPIO_PIN_6
#define LED11_GPIO_Port GPIOC
#define LED12_Pin GPIO_PIN_7
#define LED12_GPIO_Port GPIOC
#define LED13_Pin GPIO_PIN_8
#define LED13_GPIO_Port GPIOD
#define LED14_Pin GPIO_PIN_9
#define LED14_GPIO_Port GPIOD
#define LED15_Pin GPIO_PIN_10
#define LED15_GPIO_Port GPIOA
#define JACK_FUALT_2_Pin GPIO_PIN_11
#define JACK_FUALT_2_GPIO_Port GPIOA
#define IPROPI2_Pin GPIO_PIN_12
#define IPROPI2_GPIO_Port GPIOA
#define POWER_SUPPLY_FEEDBACK_Pin GPIO_PIN_15
#define POWER_SUPPLY_FEEDBACK_GPIO_Port GPIOA
#define LED16_Pin GPIO_PIN_8
#define LED16_GPIO_Port GPIOC
#define JACK_nSLEEP_2_Pin GPIO_PIN_9
#define JACK_nSLEEP_2_GPIO_Port GPIOC
#define JACK_DIR_2_Pin GPIO_PIN_0
#define JACK_DIR_2_GPIO_Port GPIOD
#define JACK_EN_2_Pin GPIO_PIN_1
#define JACK_EN_2_GPIO_Port GPIOD
#define IPROPI1_Pin GPIO_PIN_2
#define IPROPI1_GPIO_Port GPIOD
#define JACK_FUALT_1_Pin GPIO_PIN_3
#define JACK_FUALT_1_GPIO_Port GPIOD
#define JACK_nSLEEP_1_Pin GPIO_PIN_4
#define JACK_nSLEEP_1_GPIO_Port GPIOD
#define JACK_DIR_1_Pin GPIO_PIN_5
#define JACK_DIR_1_GPIO_Port GPIOD
#define JACK_EN_1_Pin GPIO_PIN_6
#define JACK_EN_1_GPIO_Port GPIOD
#define CLOSE_RECEIVER_INPUT_3_Pin GPIO_PIN_3
#define CLOSE_RECEIVER_INPUT_3_GPIO_Port GPIOB
#define OPEN_RECEIVER_INPUT_3_Pin GPIO_PIN_4
#define OPEN_RECEIVER_INPUT_3_GPIO_Port GPIOB
#define CLOSE_RECEIVER_INPUT_4_Pin GPIO_PIN_5
#define CLOSE_RECEIVER_INPUT_4_GPIO_Port GPIOB
#define OPEN_RECEIVER_INPUT_4_Pin GPIO_PIN_6
#define OPEN_RECEIVER_INPUT_4_GPIO_Port GPIOB
#define OPEN_RECEIVER_INPUT_2_Pin GPIO_PIN_7
#define OPEN_RECEIVER_INPUT_2_GPIO_Port GPIOB
#define CLOSE_RECEIVER_INPUT_1_Pin GPIO_PIN_8
#define CLOSE_RECEIVER_INPUT_1_GPIO_Port GPIOB
#define OPEN_RECEIVER_INPUT_1_Pin GPIO_PIN_9
#define OPEN_RECEIVER_INPUT_1_GPIO_Port GPIOB
#define CLOSE_RECEIVER_INPUT_2_Pin GPIO_PIN_10
#define CLOSE_RECEIVER_INPUT_2_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

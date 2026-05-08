/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, motor_1_green_led_Pin|jack_2_zero_point_green_led_Pin|motor_1_red_led_Pin|motor_2_green_led_Pin
                          |motor_2_red_led_Pin|jack_1_up_limit_red_led_Pin|jack_1_up_limit_green_led_Pin|jack_1_zero_point_green_led_Pin
                          |jack_2_sleep_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, jack_2_up_limit_red_led_Pin|jack_2_zero_point_red_led_Pin|jack_2_down_limit_green_led_Pin|jack_2_down_limit_red_led_Pin
                          |jack_2_up_limit_green_led_Pin|motor_2_move_down_Pin|motor_2_move_up_Pin|motor_1_move_down_Pin
                          |jack_1_zero_point_red_led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, jack_1_down_limit_red_led_Pin|jack_1_down_limit_green_led_Pin|jack_2_move_direction_Pin|jack_2_move_enable_Pin
                          |jack_1_sleep_Pin|jack_1_move_direction_Pin|jack_1_move_enable_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : interface_encoder_A_Pin */
  GPIO_InitStruct.Pin = interface_encoder_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(interface_encoder_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : interface_encoder_B_Pin interface_button_Pin receiver_2_up_second_order_Pin */
  GPIO_InitStruct.Pin = interface_encoder_B_Pin|interface_button_Pin|receiver_2_up_second_order_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : motor_1_green_led_Pin jack_2_zero_point_green_led_Pin motor_1_red_led_Pin motor_2_green_led_Pin
                           motor_2_red_led_Pin jack_1_up_limit_red_led_Pin jack_1_up_limit_green_led_Pin jack_1_zero_point_green_led_Pin
                           jack_2_sleep_Pin */
  GPIO_InitStruct.Pin = motor_1_green_led_Pin|jack_2_zero_point_green_led_Pin|motor_1_red_led_Pin|motor_2_green_led_Pin
                          |motor_2_red_led_Pin|jack_1_up_limit_red_led_Pin|jack_1_up_limit_green_led_Pin|jack_1_zero_point_green_led_Pin
                          |jack_2_sleep_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : jack_2_up_limit_red_led_Pin jack_2_zero_point_red_led_Pin jack_2_down_limit_green_led_Pin jack_2_down_limit_red_led_Pin
                           jack_2_up_limit_green_led_Pin motor_2_move_down_Pin motor_2_move_up_Pin motor_1_move_down_Pin
                           jack_1_zero_point_red_led_Pin */
  GPIO_InitStruct.Pin = jack_2_up_limit_red_led_Pin|jack_2_zero_point_red_led_Pin|jack_2_down_limit_green_led_Pin|jack_2_down_limit_red_led_Pin
                          |jack_2_up_limit_green_led_Pin|motor_2_move_down_Pin|motor_2_move_up_Pin|motor_1_move_down_Pin
                          |jack_1_zero_point_red_led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : motor_1_move_up_Pin */
  GPIO_InitStruct.Pin = motor_1_move_up_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(motor_1_move_up_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : motor_1_position_A_Pin motor_2_position_A_Pin jack_2_position_A_Pin */
  GPIO_InitStruct.Pin = motor_1_position_A_Pin|motor_2_position_A_Pin|jack_2_position_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : motor_1_position_B_Pin motor_2_position_B_Pin jack_2_position_B_Pin receiver_1_down_first_order_Pin
                           receiver_1_down_second_order_Pin receiver_1_up_first_order_Pin receiver_1_up_second_order_Pin receiver_2_down_first_order_Pin
                           receiver_2_down_second_order_Pin receiver_2_up_first_order_Pin */
  GPIO_InitStruct.Pin = motor_1_position_B_Pin|motor_2_position_B_Pin|jack_2_position_B_Pin|receiver_1_down_first_order_Pin
                          |receiver_1_down_second_order_Pin|receiver_1_up_first_order_Pin|receiver_1_up_second_order_Pin|receiver_2_down_first_order_Pin
                          |receiver_2_down_second_order_Pin|receiver_2_up_first_order_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : jack_1_position_A_Pin */
  GPIO_InitStruct.Pin = jack_1_position_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(jack_1_position_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : jack_1_position_B_Pin jack_2_fault_Pin */
  GPIO_InitStruct.Pin = jack_1_position_B_Pin|jack_2_fault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : jack_1_down_limit_red_led_Pin jack_1_down_limit_green_led_Pin jack_2_move_direction_Pin jack_2_move_enable_Pin
                           jack_1_sleep_Pin jack_1_move_direction_Pin jack_1_move_enable_Pin */
  GPIO_InitStruct.Pin = jack_1_down_limit_red_led_Pin|jack_1_down_limit_green_led_Pin|jack_2_move_direction_Pin|jack_2_move_enable_Pin
                          |jack_1_sleep_Pin|jack_1_move_direction_Pin|jack_1_move_enable_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PA12 supply_feedback_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12|supply_feedback_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : jack_1_fault_Pin */
  GPIO_InitStruct.Pin = jack_1_fault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(jack_1_fault_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

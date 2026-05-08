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
#define on  1
#define off 0

#define open  1
#define close 0

#define write  1
#define read   0

#define POSITION_HYSTERSIS  4
#define ZERO_POINT          100
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
extern volatile uint32_t hundred_millis;


////////////////////////  encoder   ////////////////////////
extern volatile uint16_t jack_1_position;
extern volatile uint16_t jack_2_position;
extern volatile uint16_t motor_1_position;
extern volatile uint16_t motor_2_position;

extern void setMotorPosition(uint8_t id, uint16_t position);
extern void setJackPosition(uint8_t id, uint16_t position);
extern uint16_t getJackPosition(uint8_t id);
extern uint16_t getMotorPosition(uint8_t id);
extern void encoderCallback(uint16_t GPIO_Pin);

////////////////////////  interface   ////////////////////////
#define INTERFACE_RUNNING                 0
#define INTERFACE_JACK_ZERO_POINT_SET     1
#define INTERFACE_JACK_DOWN_LIMIT_SET     2
#define INTERFACE_JACK_UP_LIMIT_SET       3
#define INTERFACE_MOTOR_TOTAL_LENGTH_SET  4

#define SETTING_NOTHING                0
#define SETTING_JACK_ZERO_POINT_SET    1
#define SETTING_JACK_DOWN_LIMIT_SET    2
#define SETTING_JACK_UP_LIMIT_SET      3
#define SETTING_COMPLETED              4

typedef struct {
	
		// leds = zero point set - down limit - up limit - motor total length
		uint8_t green_led[8];
		uint8_t red_led[8];
		uint8_t setting_changed;
		volatile uint8_t encoder_btn_clicked;
		volatile int16_t encoder_position;
		volatile uint32_t encoder_btn_clicked_timer;
		uint8_t state[2];
		uint8_t menu_number;
		uint8_t set_value_completed;
			
} interface_t;

extern volatile uint8_t setting_state[2];


extern interface_t interface;



extern void mainMenu(void);
extern void settingMenu(void);
extern void setValueMenu(void);

extern int16_t getInterfaceEncoderPosition(void);
extern void interfaceInit(void);

extern void interfaceRun(void);
extern void ledControl(uint8_t green_led[8], uint8_t red_led[8]);

extern void interfaceCallback(uint16_t GPIO_Pin);

////////////////////////  jack   ////////////////////////
typedef struct {
		volatile uint16_t position;
		uint16_t previous_position;
		uint16_t current_feedback;
		volatile uint16_t close_limit;
		volatile uint16_t open_limit;
		uint8_t zero_point_timer_flag;
		uint32_t zero_point_timer;
	
	
		uint16_t interface_close_limit;
		uint16_t interface_open_limit;
	
		uint8_t close_limit_set_flag;
		uint32_t close_limit_set_timer;
		uint8_t close_limit_set_counter;
		
		uint8_t open_limit_set_flag;
		uint32_t open_limit_set_timer;
		uint8_t open_limit_set_counter;
	
} JACK_t;

extern JACK_t jack[2];



extern void jackRun(void);
extern void jackInit(void);

extern void jackControl(uint8_t id, uint8_t enable, uint8_t direction);

////////////////////////  receiver   ////////////////////////
typedef struct {
    
		uint8_t up_order;
		uint8_t down_order;
		uint8_t up_order_counter;
		uint8_t down_order_counter;
	
} RECEIVER_t;

extern RECEIVER_t receiver[2];

extern uint32_t receiver_run_timer;
extern uint8_t  receiver_order[2];

extern void receiverRun(void);
extern void receiverInit(void);

////////////////////////  motor   ////////////////////////
typedef struct {
    volatile uint16_t position;
		uint16_t previous_position;
		
		volatile uint16_t open_limit;
		uint16_t close_limit;
	
		uint8_t total_length_flag;
		uint32_t total_length_timer;
} MOTOR_t;

extern MOTOR_t motor[2];

extern void motorRun(void);
extern void motorInit(void);

extern void motorControl(uint8_t id, uint8_t enable, uint8_t direction);


////////////////////////  eeprom   ////////////////////////

#define AT24_ADDR     (0x50 << 1)
#define EEPROM_SIZE   24

typedef struct {
    volatile uint16_t vars[12];   // 24 bytes
		volatile uint16_t jack_prev_position[2];
		volatile uint16_t jack_prev_close_limit[2];
		volatile uint16_t jack_prev_open_limit[2];
		volatile uint16_t motor_prev_position[2];
		volatile uint16_t motor_prev_open_limit[2];
		
		uint8_t setting_prev_state[2];
		
} eeprom24_t;




extern eeprom24_t AT24_Data;

extern uint32_t AT24_write_timer;
extern uint8_t AT24_write_flag;

extern volatile uint8_t AT24_State;

extern void AT24Init(I2C_HandleTypeDef *hi2c);


HAL_StatusTypeDef AT24_WriteAll_IT(void);
HAL_StatusTypeDef AT24_ReadAll_IT(void);


void AT24Run(void);


void AT24_TxComplete(void);
void AT24_RxComplete(void);


uint8_t AT24_GetState(void);




/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define interface_encoder_A_Pin GPIO_PIN_11
#define interface_encoder_A_GPIO_Port GPIOC
#define interface_encoder_B_Pin GPIO_PIN_12
#define interface_encoder_B_GPIO_Port GPIOC
#define interface_button_Pin GPIO_PIN_13
#define interface_button_GPIO_Port GPIOC
#define motor_1_green_led_Pin GPIO_PIN_14
#define motor_1_green_led_GPIO_Port GPIOC
#define jack_2_zero_point_green_led_Pin GPIO_PIN_0
#define jack_2_zero_point_green_led_GPIO_Port GPIOC
#define motor_1_red_led_Pin GPIO_PIN_1
#define motor_1_red_led_GPIO_Port GPIOC
#define motor_2_green_led_Pin GPIO_PIN_2
#define motor_2_green_led_GPIO_Port GPIOC
#define motor_2_red_led_Pin GPIO_PIN_3
#define motor_2_red_led_GPIO_Port GPIOC
#define jack_2_up_limit_red_led_Pin GPIO_PIN_0
#define jack_2_up_limit_red_led_GPIO_Port GPIOA
#define jack_2_zero_point_red_led_Pin GPIO_PIN_1
#define jack_2_zero_point_red_led_GPIO_Port GPIOA
#define jack_2_down_limit_green_led_Pin GPIO_PIN_2
#define jack_2_down_limit_green_led_GPIO_Port GPIOA
#define jack_2_down_limit_red_led_Pin GPIO_PIN_3
#define jack_2_down_limit_red_led_GPIO_Port GPIOA
#define jack_2_up_limit_green_led_Pin GPIO_PIN_4
#define jack_2_up_limit_green_led_GPIO_Port GPIOA
#define motor_2_move_down_Pin GPIO_PIN_5
#define motor_2_move_down_GPIO_Port GPIOA
#define motor_2_move_up_Pin GPIO_PIN_6
#define motor_2_move_up_GPIO_Port GPIOA
#define motor_1_move_down_Pin GPIO_PIN_7
#define motor_1_move_down_GPIO_Port GPIOA
#define motor_1_move_up_Pin GPIO_PIN_0
#define motor_1_move_up_GPIO_Port GPIOB
#define motor_1_position_A_Pin GPIO_PIN_1
#define motor_1_position_A_GPIO_Port GPIOB
#define motor_1_position_B_Pin GPIO_PIN_2
#define motor_1_position_B_GPIO_Port GPIOB
#define motor_2_position_A_Pin GPIO_PIN_12
#define motor_2_position_A_GPIO_Port GPIOB
#define motor_2_position_B_Pin GPIO_PIN_13
#define motor_2_position_B_GPIO_Port GPIOB
#define jack_2_position_A_Pin GPIO_PIN_14
#define jack_2_position_A_GPIO_Port GPIOB
#define jack_2_position_B_Pin GPIO_PIN_15
#define jack_2_position_B_GPIO_Port GPIOB
#define jack_1_position_A_Pin GPIO_PIN_8
#define jack_1_position_A_GPIO_Port GPIOA
#define jack_1_position_B_Pin GPIO_PIN_9
#define jack_1_position_B_GPIO_Port GPIOA
#define jack_1_up_limit_red_led_Pin GPIO_PIN_6
#define jack_1_up_limit_red_led_GPIO_Port GPIOC
#define jack_1_up_limit_green_led_Pin GPIO_PIN_7
#define jack_1_up_limit_green_led_GPIO_Port GPIOC
#define jack_1_down_limit_red_led_Pin GPIO_PIN_8
#define jack_1_down_limit_red_led_GPIO_Port GPIOD
#define jack_1_down_limit_green_led_Pin GPIO_PIN_9
#define jack_1_down_limit_green_led_GPIO_Port GPIOD
#define jack_1_zero_point_red_led_Pin GPIO_PIN_10
#define jack_1_zero_point_red_led_GPIO_Port GPIOA
#define jack_2_fault_Pin GPIO_PIN_11
#define jack_2_fault_GPIO_Port GPIOA
#define supply_feedback_Pin GPIO_PIN_15
#define supply_feedback_GPIO_Port GPIOA
#define jack_1_zero_point_green_led_Pin GPIO_PIN_8
#define jack_1_zero_point_green_led_GPIO_Port GPIOC
#define jack_2_sleep_Pin GPIO_PIN_9
#define jack_2_sleep_GPIO_Port GPIOC
#define jack_2_move_direction_Pin GPIO_PIN_0
#define jack_2_move_direction_GPIO_Port GPIOD
#define jack_2_move_enable_Pin GPIO_PIN_1
#define jack_2_move_enable_GPIO_Port GPIOD
#define jack_1_fault_Pin GPIO_PIN_3
#define jack_1_fault_GPIO_Port GPIOD
#define jack_1_sleep_Pin GPIO_PIN_4
#define jack_1_sleep_GPIO_Port GPIOD
#define jack_1_move_direction_Pin GPIO_PIN_5
#define jack_1_move_direction_GPIO_Port GPIOD
#define jack_1_move_enable_Pin GPIO_PIN_6
#define jack_1_move_enable_GPIO_Port GPIOD
#define receiver_1_down_first_order_Pin GPIO_PIN_3
#define receiver_1_down_first_order_GPIO_Port GPIOB
#define receiver_1_down_second_order_Pin GPIO_PIN_4
#define receiver_1_down_second_order_GPIO_Port GPIOB
#define receiver_1_up_first_order_Pin GPIO_PIN_5
#define receiver_1_up_first_order_GPIO_Port GPIOB
#define receiver_1_up_second_order_Pin GPIO_PIN_6
#define receiver_1_up_second_order_GPIO_Port GPIOB
#define receiver_2_down_first_order_Pin GPIO_PIN_7
#define receiver_2_down_first_order_GPIO_Port GPIOB
#define receiver_2_down_second_order_Pin GPIO_PIN_8
#define receiver_2_down_second_order_GPIO_Port GPIOB
#define receiver_2_up_first_order_Pin GPIO_PIN_9
#define receiver_2_up_first_order_GPIO_Port GPIOB
#define receiver_2_up_second_order_Pin GPIO_PIN_10
#define receiver_2_up_second_order_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

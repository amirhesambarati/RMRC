#include "main.h"
/*
volatile uint16_t jack_1_position = 21000;
volatile uint16_t jack_2_position = 21000;
volatile uint16_t motor_1_position = 21000;
volatile uint16_t motor_2_position = 21000;



void setJackPosition(uint8_t id, uint16_t position){
		if(id == 0) jack_1_position = position;
		else if(id == 1) jack_2_position = position;
}

void setMotorPosition(uint8_t id, uint16_t position){
		if(id == 0) motor_1_position = position;
		else if(id == 1) motor_2_position = position;
}

uint16_t getJackPosition(uint8_t id){
		
		if(id == 0) return jack_1_position;
    else if(id == 1) return jack_2_position;
		else return 0;
}
uint16_t getMotorPosition(uint8_t id){
		
		if(id == 0) return motor_1_position;
    else if(id == 1) return motor_2_position;
		else return 0;
}



void encoderCallback(uint16_t GPIO_Pin)
{
		uint8_t B = 0;
	
		switch(GPIO_Pin){
			case jack_1_position_A_Pin:
				
				B = (HAL_GPIO_ReadPin(jack_1_position_B_GPIO_Port, jack_1_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((B == 1) && (jack_1_position < jack[0].open_limit)) {
					jack_1_position += 1;
					
				}
				else if(jack_1_position > jack[0].close_limit) {
					jack_1_position -= 1;
					//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);
				}	
			break;	
			case jack_2_position_A_Pin:
				B = (HAL_GPIO_ReadPin(jack_2_position_B_GPIO_Port, jack_2_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((B == 1) && (jack_2_position < jack[1].open_limit)) jack_2_position += 1;
				else if(jack_2_position > jack[1].close_limit) jack_2_position -= 1;
			break;
			case motor_1_position_A_Pin:
				B = (HAL_GPIO_ReadPin(motor_1_position_B_GPIO_Port, motor_1_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((B == 1) && (motor_1_position < motor[0].open_limit)) motor_1_position += 1;
				else if((motor_1_position > motor[0].close_limit)) motor_1_position -= 1;
			break;
			case motor_2_position_A_Pin:
				B = (HAL_GPIO_ReadPin(motor_2_position_B_GPIO_Port, motor_2_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((B == 1) && (motor_2_position < motor[1].open_limit) && (receiver[1].down_order > 0)) motor_2_position += 1;
				else if((motor_2_position > motor[1].close_limit) && (receiver[1].up_order > 0)) motor_2_position -= 1;
			break;
			default:
			break;	
		}
}
*/

void encoderCallback(uint16_t GPIO_Pin)
{
		uint8_t B = 0;
		uint8_t C = 0;
		uint8_t D = 0;
		uint8_t E = 0;
		
		switch(GPIO_Pin){
			case jack_1_position_A_Pin:
				
				B = (HAL_GPIO_ReadPin(jack_1_position_B_GPIO_Port, jack_1_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((B == 1) && (jack[0].position < jack[0].open_limit)) {
					jack[0].position += 1;
					
				}
				else if(jack[0].position > jack[0].close_limit) {
					jack[0].position -= 1;
					//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);
				}	
			break;	
			case jack_2_position_A_Pin:
				C = (HAL_GPIO_ReadPin(jack_2_position_B_GPIO_Port, jack_2_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((C == 1) && (jack[1].position < jack[1].open_limit)) jack[1].position += 1;
				else if(jack[1].position > jack[1].close_limit) jack[1].position -= 1;
			break;
			case motor_1_position_A_Pin:
				//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);  // test 1
				D = (HAL_GPIO_ReadPin(motor_1_position_B_GPIO_Port, motor_1_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((D == 1) && (motor[0].position < motor[0].open_limit) && (receiver[0].down_order > 0)) {
					motor[0].position += 1;
					//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);  // test 2
				}
				else if((motor[0].position > motor[0].close_limit) && (receiver[0].up_order > 0)) motor[0].position -= 1;
			break;
			case motor_2_position_A_Pin:
				E = (HAL_GPIO_ReadPin(motor_2_position_B_GPIO_Port, motor_2_position_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
				if((E == 1) && (motor[1].position < motor[1].open_limit) && (receiver[1].down_order > 0)) motor[1].position += 1;
				else if((motor[1].position > motor[1].close_limit) && (receiver[1].up_order > 0)) motor[1].position -= 1;
			break;
			default:
			break;	
		}
}
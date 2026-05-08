#include "main.h"

volatile uint16_t jack_1_position = 0;
volatile uint16_t jack_2_position = 0;
volatile uint16_t motor_1_position = 0;
volatile uint16_t motor_2_position = 0;



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
		else return -32000;
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
				B = HAL_GPIO_ReadPin(jack_1_position_B_GPIO_Port, jack_1_position_B_Pin);
				if(B == GPIO_PIN_SET) jack_1_position += 1;
				else if(jack_1_position > 0) jack_1_position -= 1;
			break;	
			case jack_2_position_A_Pin:
				B = HAL_GPIO_ReadPin(jack_2_position_B_GPIO_Port, jack_2_position_B_Pin);
				if(B == GPIO_PIN_SET) jack_2_position += 1;
				else if(jack_2_position > 0) jack_2_position -= 1;
			break;
			case motor_1_position_A_Pin:
				B = HAL_GPIO_ReadPin(motor_1_position_B_GPIO_Port, motor_1_position_B_Pin);
				if(B == GPIO_PIN_SET) motor_1_position += 1;
				else if(motor_1_position > 0) motor_1_position -= 1;
			break;
			case motor_2_position_A_Pin:
				B = HAL_GPIO_ReadPin(motor_2_position_B_GPIO_Port, motor_2_position_B_Pin);
				if(B == GPIO_PIN_SET) motor_2_position += 1;
				else if(motor_2_position > 0) motor_2_position -= 1;
			break;
			default:
			break;	
		}
}
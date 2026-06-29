#include "main.h"


MOTOR_t motor[2];

void motorInit(void)
{
    for(uint8_t i = 0; i < 2; i++){
      motor[i].position = 20000;
			motor[i].previous_position = 0;

			motor[i].open_limit = 50000;	
			motor[i].close_limit = ZERO_POINT;
	
			motor[i].total_length_flag = 0;
			motor[i].total_length_timer = 0;
			
			motor[i].running_flag = 0;
			motor[i].running_timer = 0;
    }
}



void motorRun(){
	for(uint8_t i = 0; i < 2; i++){
		//motor[i].position = getMotorPosition(i);
		
		switch(interface.state[i]){
			case INTERFACE_RUNNING:
				if(setting_state[i] == SETTING_COMPLETED){
					if(receiver[i].down_order > 0){
						if(motor[i].position < motor[i].open_limit - POSITION_HYSTERSIS) motorControl(i, on, open);
						else if(motor[i].position > motor[i].open_limit + POSITION_HYSTERSIS) motorControl(i, on, close);
						else motorControl(i, off, close);
						/*
						if(motor[i].running_flag == 0){
							motor[i].running_flag = 1;
							motor[i].running_timer = hundred_millis;
							motor[i].previous_position = motor[i].position;
						}
						else if((motor[i].running_flag > 0) && (hundred_millis > motor[i].running_timer + 20)){ // 2 sec
							motor[i].running_flag = 0;
							if(motor[i].position == motor[i].previous_position){
								
								setMotorPosition(i, motor[i].open_limit);
							}
						}
						*/
						
					}
					else if((receiver[i].up_order > 0)){
						if((jack[i].position > jack[i].open_limit - POSITION_HYSTERSIS) && (jack[i].position < jack[i].open_limit + POSITION_HYSTERSIS)){
							if(motor[i].position < motor[i].close_limit - POSITION_HYSTERSIS) motorControl(i, on, open);
							else if(motor[i].position > motor[i].close_limit + POSITION_HYSTERSIS) motorControl(i, on, close);
							else motorControl(i, off, close);
							/*
							if(motor[i].running_flag == 0){
								motor[i].running_flag = 1;
								motor[i].running_timer = hundred_millis;
								motor[i].previous_position = motor[i].position;
							}
							else if((motor[i].running_flag > 0) && (hundred_millis > motor[i].running_timer + 20)){ // 2 sec
								motor[i].running_flag = 0;
								if(motor[i].position == motor[i].previous_position){
								
									setMotorPosition(i, motor[i].close_limit);
									
								}
							}
							*/
							
						}
					}
					else {
						motorControl(i, off, close);
						motor[i].running_flag = 0;
						//motor[i].total_length_flag = 0;
					}						
					
					
				}
				else if(setting_state[i] == SETTING_JACK_UP_LIMIT_SET){
					//motor[i].running_flag = 0;
					motor[i].total_length_flag = 0;
					if((jack[i].position > jack[i].open_limit - POSITION_HYSTERSIS) && (jack[i].position < jack[i].open_limit + POSITION_HYSTERSIS)){
						if(receiver[i].down_order > 0) motorControl(i, on, open);
						else if(receiver[i].up_order > 0) motorControl(i, on, close);
						else {
							motorControl(i, off, close);
							//setMotorPosition(i, ZERO_POINT);
							motor[i].position = ZERO_POINT;
							motor[i].close_limit = ZERO_POINT;
							motor[i].open_limit = 50000;
						}	
					}
				}
				else {
					motorControl(i, off, close);
					motor[i].total_length_flag = 0;
					motor[i].running_flag = 0;
				}
			break;
			case INTERFACE_MOTOR_TOTAL_LENGTH_SET:
				
				if(motor[i].total_length_flag == 0){
					motor[i].total_length_flag = 1;
					motor[i].total_length_timer = hundred_millis;
					motor[i].previous_position = motor[i].position;
				}
				else if((motor[i].total_length_flag > 0) && (hundred_millis > motor[i].total_length_timer + 30)){ // 3 sec
					motor[i].total_length_flag = 0;
					if(motor[i].position == motor[i].previous_position){
						motor[i].open_limit = motor[i].position;
						interface.set_value_completed = 1;
					}
				}
				
				if((jack[i].position > jack[i].open_limit - POSITION_HYSTERSIS) && (jack[i].position < jack[i].open_limit + POSITION_HYSTERSIS)){
					if(receiver[i].down_order > 0) {
						motorControl(i, on, open);
						interface.set_value_completed = 0;
					}	
					else if(receiver[i].up_order > 0) {
						motorControl(i, on, close);
						interface.set_value_completed = 0;
					}	
					else motorControl(i, off, close);
				}
				
				
			break;
			case INTERFACE_JACK_ZERO_POINT_SET:
				motorControl(i, off, close);
			break;
			case INTERFACE_JACK_DOWN_LIMIT_SET:
				motorControl(i, off, close);
			break;
			case INTERFACE_JACK_UP_LIMIT_SET:
				motorControl(i, off, close);
			break;
			default:
				interface.state[i] = INTERFACE_RUNNING;
			break;	
		}
		
	}
}









void motorControl(uint8_t id, uint8_t enable, uint8_t direction){
	switch(id){
		case 0:
			if((direction == open) && (enable == on)){
				HAL_GPIO_WritePin(motor_1_move_down_GPIO_Port, motor_1_move_down_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_SET);
				
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_1_move_down_GPIO_Port, motor_1_move_down_Pin, GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_1_move_down_GPIO_Port, motor_1_move_down_Pin, GPIO_PIN_RESET);
			}
		break;
		case 1:
	
			if((direction == open) && (enable == on)){
				HAL_GPIO_WritePin(motor_2_move_down_GPIO_Port, motor_2_move_down_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_2_move_up_GPIO_Port, motor_2_move_up_Pin, GPIO_PIN_SET);
				
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(motor_2_move_up_GPIO_Port, motor_2_move_up_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_2_move_down_GPIO_Port, motor_2_move_down_Pin, GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin(motor_2_move_up_GPIO_Port, motor_2_move_up_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(motor_2_move_down_GPIO_Port, motor_2_move_down_Pin, GPIO_PIN_RESET);
			}
		break;
		default:

			HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_1_move_down_GPIO_Port, motor_1_move_down_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_2_move_up_GPIO_Port, motor_2_move_up_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_2_move_down_GPIO_Port, motor_2_move_down_Pin, GPIO_PIN_RESET);
		break;
	}	
	//return 1;
}
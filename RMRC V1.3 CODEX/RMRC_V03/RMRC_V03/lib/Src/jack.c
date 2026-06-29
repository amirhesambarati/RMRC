#include "main.h"


JACK_t jack[2];

void jackInit(void)
{
    for(uint8_t i = 0; i < 2; i++){
      jack[i].position = 20000;
			jack[i].previous_position = 0;
			
			jack[i].current_feedback = 0;
			jack[i].close_limit = ZERO_POINT;
			jack[i].open_limit = 50000;
			jack[i].zero_point_timer_flag = 0;
			jack[i].zero_point_timer = 0;
			
			jack[i].interface_close_limit = 0;
			jack[i].interface_open_limit = 0;
			
			jack[i].close_limit_set_flag = 0;
			jack[i].close_limit_set_timer = 0;
			jack[i].close_limit_set_counter = 0;
			
			jack[i].open_limit_set_flag = 0;
			jack[i].open_limit_set_timer = 0;
			jack[i].open_limit_set_counter = 0;
			
			
			jack[i].running_flag = 0;
			jack[i].running_timer = 0;
    }
}



void jackRun(){
	for(uint8_t s = 0; s < 2; s++){
	//if(jack_fault_flag == 0){
		//jack[s].position = getJackPosition(s);
		
		switch(interface.state[s]){
			case INTERFACE_RUNNING:
				
			
				if(setting_state[s] == SETTING_COMPLETED){
					if((receiver[s].down_order > 0)){
						if((motor[s].position > motor[s].open_limit - POSITION_HYSTERSIS) && (motor[s].position < motor[s].open_limit + POSITION_HYSTERSIS)){
							if(jack[s].position > jack[s].close_limit + POSITION_HYSTERSIS) jackControl(s, on, close);
							else if(jack[s].position < jack[s].close_limit - POSITION_HYSTERSIS) jackControl(s, on, open);
							else jackControl(s, off, close);
							/*
							if(jack[s].running_flag == 0){
								jack[s].running_flag = 1;
								jack[s].running_timer = hundred_millis;
								jack[s].previous_position = jack[s].position;
							}
							else if((jack[s].running_flag > 0) && (hundred_millis > jack[s].running_timer + 20)){ // 3 sec
								jack[s].running_flag = 0;
								if(jack[s].position == jack[s].previous_position){
									jack[s].position = jack[s].close_limit;
									//setJackPosition(s, jack[s].close_limit);
								}
							}
							*/
							
						}	
						else jackControl(s, off, close);
					}
					else if(receiver[s].up_order > 0){
						if(jack[s].position < jack[s].open_limit - POSITION_HYSTERSIS) jackControl(s, on, open);
						else if(jack[s].position > jack[s].open_limit + POSITION_HYSTERSIS) jackControl(s, on, close);
						else jackControl(s, off, close);
						
							/*
							if(jack[s].running_flag == 0){
								jack[s].running_flag = 1;
								jack[s].running_timer = hundred_millis;
								jack[s].previous_position = jack[s].position;
							}
							else if((jack[s].running_flag > 0) && (hundred_millis > jack[s].running_timer + 20)){ // 3 sec
								jack[s].running_flag = 0;
								if(jack[s].position == jack[s].previous_position){
									jack[s].position = jack[s].open_limit;
									//setJackPosition(s, jack[s].open_limit);
								}
							}
							*/
						
					}
					else {
						jackControl(s, off, close);
						jack[s].running_flag = 0;
					}						
					
				}
				else if(setting_state[s] == SETTING_JACK_UP_LIMIT_SET){
					if(jack[s].position > jack[s].open_limit + POSITION_HYSTERSIS){
						jackControl(s, on, close);
					}
					else if(jack[s].position < jack[s].open_limit - POSITION_HYSTERSIS){
						jackControl(s, on, open);
					}
					else{
						jackControl(s, off, close);
					}
				}
				else{
					jackControl(s, off, close);
					//jack[s].zero_point_timer_flag = 0;
					//jack[s].open_limit_set_flag = 0;
					jack[s].running_flag = 0;
				}
			break;
			case INTERFACE_JACK_ZERO_POINT_SET:
				
				//if(interface.set_value_completed == 0) jackControl(s, on, close);
				if(jack[s].zero_point_timer_flag == 0){
				
					jack[s].zero_point_timer_flag = 1;
					jack[s].zero_point_timer = hundred_millis;
					jack[s].previous_position = jack[s].position;
				
				}
				else if((jack[s].zero_point_timer_flag > 0) && (hundred_millis > jack[s].zero_point_timer + 20)){ // 2 seconds
					jack[s].zero_point_timer_flag = 0;
					if(jack[s].previous_position == jack[s].position){
						jackControl(s , off, close);
						//setJackPosition(s, ZERO_POINT);
						jack[s].position = 10000;
						interface.set_value_completed = 1;
						jack[s].close_limit = ZERO_POINT;  /////// new
						jack[s].open_limit = 50000;
					}
				}
				
			break;
			case INTERFACE_JACK_DOWN_LIMIT_SET:
				jack[s].interface_close_limit = (getInterfaceEncoderPosition() * 10) + jack[s].close_limit;
				
			
				if(jack[s].position > jack[s].interface_close_limit + POSITION_HYSTERSIS){
					jackControl(s, on, close);
					jack[s].close_limit_set_flag = 0;
					jack[s].close_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[s].position < jack[s].interface_close_limit - POSITION_HYSTERSIS){
					jackControl(s, on, open);
					jack[s].close_limit_set_flag = 0;
					jack[s].close_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[s].close_limit_set_flag == 0){
					jackControl(s, off, close);
					jack[s].close_limit_set_flag = 1;
					jack[s].close_limit_set_timer = hundred_millis;
				}
				
				if((jack[s].close_limit_set_flag > 0) && (hundred_millis > jack[s].close_limit_set_timer + 10)){ // 1 second
					jack[s].close_limit_set_flag = 0;
					jack[s].close_limit_set_counter += 1;
				}
				
				if(jack[s].close_limit_set_counter >= 3){
					jack[s].close_limit_set_counter = 0;
					
					jackControl(s, off, close);
					jack[s].close_limit = ZERO_POINT;
					interface.set_value_completed = 1;
					//jack[s].position = ZERO_POINT;  /////// new
					jack[s].open_limit = 50000;
				}
				
				
			break;
			case INTERFACE_JACK_UP_LIMIT_SET:
				jack[s].interface_open_limit = (getInterfaceEncoderPosition() * 10) + jack[s].close_limit;
			
				if(jack[s].position > jack[s].interface_open_limit + POSITION_HYSTERSIS){
					jackControl(s, on, close);
					jack[s].open_limit_set_flag = 0;
					jack[s].open_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[s].position < jack[s].interface_open_limit - POSITION_HYSTERSIS){
					jackControl(s, on, open);
					jack[s].open_limit_set_flag = 0;
					jack[s].open_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[s].open_limit_set_flag == 0){
					jackControl(s, off, close);
					jack[s].open_limit_set_flag = 1;
					jack[s].open_limit_set_timer = hundred_millis;
				}
				
				if((jack[s].open_limit_set_flag > 0) && (hundred_millis > jack[s].open_limit_set_timer + 10)){ // 1 second
					jack[s].open_limit_set_flag = 0;
					jack[s].open_limit_set_counter += 1;
				}
				
				if(jack[s].open_limit_set_counter >= 3){
					jack[s].open_limit_set_counter = 0;
					
					jackControl(s, off, close);
					jack[s].open_limit = jack[s].position;
					interface.set_value_completed = 1;
				}
				
			break;
			case INTERFACE_MOTOR_TOTAL_LENGTH_SET:
				if(jack[s].position > jack[s].open_limit + POSITION_HYSTERSIS){
						jackControl(s, on, close);
				}
				else if(jack[s].position < jack[s].open_limit - POSITION_HYSTERSIS){
						jackControl(s, on, open);
				}
				else{
						jackControl(s, off, close);
				}
			break;
			default:
				interface.state[s] = INTERFACE_RUNNING;
			break;
		}
	//}
	//else{
	//	jackControl(off, close);
		
	//}
	}	
}









void jackControl(uint8_t id, uint8_t enable, uint8_t direction){
	switch(id){
		case 0:
			if((direction == open) && (enable == on)){
				HAL_GPIO_WritePin(jack_1_move_close_GPIO_Port, jack_1_move_close_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_1_move_open_GPIO_Port, jack_1_move_open_Pin, GPIO_PIN_SET);	
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(jack_1_move_open_GPIO_Port, jack_1_move_open_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_1_move_close_GPIO_Port, jack_1_move_close_Pin, GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin(jack_1_move_open_GPIO_Port, jack_1_move_open_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_1_move_close_GPIO_Port, jack_1_move_close_Pin, GPIO_PIN_RESET);
			}
		break;
		case 1:
	
			if((direction == open) && (enable == on)){
				HAL_GPIO_WritePin(jack_2_move_close_GPIO_Port, jack_2_move_close_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_2_move_open_GPIO_Port, jack_2_move_open_Pin, GPIO_PIN_SET);	
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(jack_2_move_open_GPIO_Port, jack_2_move_open_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_2_move_close_GPIO_Port, jack_2_move_close_Pin, GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin(jack_2_move_open_GPIO_Port, jack_2_move_open_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_2_move_close_GPIO_Port, jack_2_move_close_Pin, GPIO_PIN_RESET);
			}
		break;
		default:

			HAL_GPIO_WritePin(jack_1_move_open_GPIO_Port, jack_1_move_open_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_1_move_close_GPIO_Port, jack_1_move_close_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_open_GPIO_Port, jack_2_move_open_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_close_GPIO_Port, jack_2_move_close_Pin, GPIO_PIN_RESET);
		break;
	}	
	//return 1;
}
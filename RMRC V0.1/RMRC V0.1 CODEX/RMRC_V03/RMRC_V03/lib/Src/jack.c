#include "main.h"


JACK_t jack[2];

void jackInit(void)
{
    for(uint8_t i = 0; i < 2; i++){
      jack[i].position = 0;
			jack[i].previous_position = 0;
			
			jack[i].current_feedback = 0;
			jack[i].close_limit = 0;
			jack[i].open_limit = 0;
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
    }
}



void jackRun(){
	for(uint8_t i = 0; i < 2; i++){
	//if(jack_fault_flag == 0){
		jack[i].position = getJackPosition(i);
		
		switch(interface.state[i]){
			case INTERFACE_RUNNING:
				
			
				if(setting_state[i] == SETTING_COMPLETED){
					if((receiver[i].down_order > 0)){
						if((motor[i].position > motor[i].open_limit - POSITION_HYSTERSIS) && (motor[i].position < motor[i].open_limit + POSITION_HYSTERSIS)){
							if(jack[i].position > jack[i].close_limit + POSITION_HYSTERSIS) jackControl(i, on, close);
							else if(jack[i].position < jack[i].close_limit - POSITION_HYSTERSIS) jackControl(i, on, open);
							else jackControl(i, off, close);
						}	
						else jackControl(i, off, close);
					}
					else if(receiver[i].up_order > 0){
						if(jack[i].position < jack[i].open_limit - POSITION_HYSTERSIS) jackControl(i, on, open);
						else if(jack[i].position > jack[i].open_limit + POSITION_HYSTERSIS) jackControl(i, on, close);
						else jackControl(i, off, close);
					}
					else jackControl(i, off, close);
				}
				else if(setting_state[i] == SETTING_JACK_UP_LIMIT_SET){
					if(jack[i].position > jack[i].open_limit + POSITION_HYSTERSIS){
						jackControl(i, on, close);
					}
					else if(jack[i].position < jack[i].open_limit - POSITION_HYSTERSIS){
						jackControl(i, on, open);
					}
					else{
						jackControl(i, off, close);
					}
				}
				else{
					jackControl(i, off, close);
				}
			break;
			case INTERFACE_JACK_ZERO_POINT_SET:
				if(interface.set_value_completed == 0) jackControl(i, on, close);
				if(jack[i].zero_point_timer_flag == 0){
					jack[i].zero_point_timer_flag = 1;
					jack[i].zero_point_timer = hundred_millis;
					jack[i].previous_position = jack[i].position;
				}
				else if((jack[i].zero_point_timer_flag > 0) && (hundred_millis > jack[i].zero_point_timer + 20)){ // 2 seconds
					jack[i].zero_point_timer_flag = 0;
					if(jack[i].previous_position == jack[i].position){
						jackControl(i , off, close);
						setJackPosition(i, ZERO_POINT);
						//setting_state[i] = SETTING_JACK_ZERO_POINT_SET;
						interface.set_value_completed = 1;
					}
				}
				
			break;
			case INTERFACE_JACK_DOWN_LIMIT_SET:
				jack[i].interface_close_limit = getInterfaceEncoderPosition() * 10;
				
			
				if(jack[i].position > jack[i].interface_close_limit + POSITION_HYSTERSIS){
					jackControl(i, on, close);
					jack[i].close_limit_set_flag = 0;
					jack[i].close_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[i].position < jack[i].interface_close_limit - POSITION_HYSTERSIS){
					jackControl(i, on, open);
					jack[i].close_limit_set_flag = 0;
					jack[i].close_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[i].close_limit_set_flag == 0){
					jackControl(i, off, close);
					jack[i].close_limit_set_flag = 1;
					jack[i].close_limit_set_timer = hundred_millis;
				}
				
				if((jack[i].close_limit_set_flag > 0) && (hundred_millis > jack[i].close_limit_set_timer + 10)){ // 1 second
					jack[i].close_limit_set_flag = 0;
					jack[i].close_limit_set_counter += 1;
				}
				
				if(jack[i].close_limit_set_counter >= 3){
					jack[i].close_limit_set_counter = 0;
					
					jackControl(i, off, close);
					jack[i].close_limit = jack[i].position;
					interface.set_value_completed = 1;
				}
				
				
			break;
			case INTERFACE_JACK_UP_LIMIT_SET:
				jack[i].interface_open_limit = (getInterfaceEncoderPosition() * 10) + jack[i].close_limit;
			
				if(jack[i].position > jack[i].interface_open_limit + POSITION_HYSTERSIS){
					jackControl(i, on, close);
					jack[i].open_limit_set_flag = 0;
					jack[i].open_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[i].position < jack[i].interface_open_limit - POSITION_HYSTERSIS){
					jackControl(i, on, open);
					jack[i].open_limit_set_flag = 0;
					jack[i].open_limit_set_counter = 0;
					interface.set_value_completed = 0;
				}
				else if(jack[i].open_limit_set_flag == 0){
					jackControl(i, off, close);
					jack[i].open_limit_set_flag = 1;
					jack[i].open_limit_set_timer = hundred_millis;
				}
				
				if((jack[i].open_limit_set_flag > 0) && (hundred_millis > jack[i].open_limit_set_timer + 10)){ // 1 second
					jack[i].open_limit_set_flag = 0;
					jack[i].open_limit_set_counter += 1;
				}
				
				if(jack[i].open_limit_set_counter >= 3){
					jack[i].open_limit_set_counter = 0;
					
					jackControl(i, off, close);
					jack[i].open_limit = jack[i].position;
					interface.set_value_completed = 1;
				}
				
			break;
			case INTERFACE_MOTOR_TOTAL_LENGTH_SET:
				if(jack[i].position > jack[i].open_limit + POSITION_HYSTERSIS){
						jackControl(i, on, close);
				}
				else if(jack[i].position < jack[i].open_limit - POSITION_HYSTERSIS){
						jackControl(i, on, open);
				}
				else{
						jackControl(i, off, close);
				}
			break;
			default:
				interface.state[i] = INTERFACE_RUNNING;
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
				HAL_GPIO_WritePin(jack_1_move_enable_GPIO_Port, jack_1_move_enable_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(jack_1_move_direction_GPIO_Port, jack_1_move_direction_Pin, GPIO_PIN_SET);
				
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(jack_1_move_enable_GPIO_Port, jack_1_move_enable_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(jack_1_move_direction_GPIO_Port, jack_1_move_direction_Pin, GPIO_PIN_RESET);
			}
			else{
				HAL_GPIO_WritePin(jack_1_move_enable_GPIO_Port, jack_1_move_enable_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_1_move_direction_GPIO_Port, jack_1_move_direction_Pin, GPIO_PIN_RESET);
			}
		break;
		case 1:
	
			if((direction == open) && (enable == on)){
				HAL_GPIO_WritePin(jack_2_move_enable_GPIO_Port, jack_2_move_enable_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(jack_2_move_direction_GPIO_Port, jack_2_move_direction_Pin, GPIO_PIN_SET);
				
			}
			else if((direction == close) && (enable == on)){
				HAL_GPIO_WritePin(jack_2_move_enable_GPIO_Port, jack_2_move_enable_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(jack_2_move_direction_GPIO_Port, jack_2_move_direction_Pin, GPIO_PIN_RESET);
			}
			else{
				HAL_GPIO_WritePin(jack_2_move_enable_GPIO_Port, jack_2_move_enable_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(jack_2_move_direction_GPIO_Port, jack_2_move_direction_Pin, GPIO_PIN_RESET);
			}
		break;
		default:

			HAL_GPIO_WritePin(jack_1_move_enable_GPIO_Port, jack_1_move_enable_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_1_move_direction_GPIO_Port, jack_1_move_direction_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_enable_GPIO_Port, jack_2_move_enable_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_direction_GPIO_Port, jack_2_move_direction_Pin, GPIO_PIN_RESET);
		break;
	}	
	//return 1;
}
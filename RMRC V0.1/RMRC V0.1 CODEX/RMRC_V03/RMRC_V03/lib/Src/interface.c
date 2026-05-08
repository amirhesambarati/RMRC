#include "main.h"



interface_t interface;

volatile uint8_t setting_state[2] = {0, 0};

void interfaceInit(void)
{
    for(uint8_t i = 0; i < 8; i++){
        interface.green_led[i] = 0;
        interface.red_led[i] = 0;
				
				
    }
		interface.setting_changed = 1;
    interface.encoder_btn_clicked = 0;
    interface.encoder_position = 0;
    interface.encoder_btn_clicked_timer = 0;
    interface.state[0] = 0;
		interface.state[1] = 0;
    interface.menu_number = 0;
		interface.set_value_completed = 0;
		
}

void interfaceRun(){
		if(interface.encoder_btn_clicked == 2){
				interface.encoder_btn_clicked = 0;
				interface.setting_changed = 1;
				if(interface.menu_number == 0) interface.menu_number = 1;
				else if(interface.menu_number == 1){
					if(interface.encoder_position < 4){
						if(interface.encoder_position == setting_state[0]){
							interface.state[0] = setting_state[0] + 1;
							interface.menu_number = 2;
							for(uint8_t i = 0; i < 8; i++){
								interface.green_led[i] = 0;
								interface.red_led[i] = 0;
							}
						}
					}
					else{
						if(interface.encoder_position == setting_state[1] + 4){
							interface.state[1] = setting_state[1] + 1;
							interface.menu_number = 2;
							for(uint8_t i = 0; i < 8; i++){
								interface.green_led[i] = 0;
								interface.red_led[i] = 0;
							}
						}
					}
				}
				else if((interface.menu_number == 2) && (interface.set_value_completed == 1)){
					interface.set_value_completed = 0;
					interface.menu_number = 0;
					if(interface.state[0] > 0){
						setting_state[0] = interface.state[0];
						
						interface.state[0] = INTERFACE_RUNNING;
					}
					else if(interface.state[1] > 0){
						setting_state[1] = interface.state[1];
						
						interface.state[1] = INTERFACE_RUNNING;
					}
				}
				
		}
		else if((interface.encoder_btn_clicked == 1) && (hundred_millis > interface.encoder_btn_clicked_timer + 30)){
			interface.encoder_btn_clicked = 3;
			interface.encoder_btn_clicked_timer = hundred_millis;
			if(interface.menu_number == 1){
				
				if(interface.encoder_position < setting_state[0]){
					setting_state[0] = interface.encoder_position;
					interface.setting_changed = 1;
					interface.menu_number = 0;
					interface.state[0] = INTERFACE_RUNNING;
					interface.state[1] = INTERFACE_RUNNING;
				}
				else if((interface.encoder_position > 3) && (interface.encoder_position < setting_state[1] + 4)){
					setting_state[1] = interface.encoder_position - 4;
					interface.setting_changed = 1;
					interface.menu_number = 0;
					interface.state[0] = INTERFACE_RUNNING;
					interface.state[1] = INTERFACE_RUNNING;
				}
			}
			else if(interface.menu_number > 1){
				if(interface.state[0] > 1){
					interface.menu_number = 0;
					setting_state[0] = interface.state[0] - 2;
					interface.state[0] = INTERFACE_RUNNING;
				}
				else if(interface.state[0] == 1){
					interface.menu_number = 0;
					setting_state[0] = 0;
					interface.state[0] = INTERFACE_RUNNING;
				}
				
				if(interface.state[1] > 1){
					interface.menu_number = 0;
					setting_state[1] = interface.state[1] - 2;
					interface.state[1] = INTERFACE_RUNNING;
				}
				else if(interface.state[1] == 1){
					interface.menu_number = 0;
					setting_state[1] = 0;
					interface.state[1] = INTERFACE_RUNNING;
				}
			
			}
		}
		else if(interface.encoder_btn_clicked > 3) interface.encoder_btn_clicked = 0;
		
		switch(interface.menu_number){
			case 0:
					mainMenu();		
			break;
			case 1:				
					settingMenu();
			break;
			case 2:
					setValueMenu();
			break;
			default:
			break;
		}
		
		if(interface.setting_changed > 0){
			interface.setting_changed = 0;
			ledControl(interface.green_led, interface.red_led);
		}
}

void ledControl(uint8_t green_led[8], uint8_t red_led[8]){
	
	if(green_led[0] > 0) HAL_GPIO_WritePin(jack_1_zero_point_green_led_GPIO_Port, jack_1_zero_point_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_zero_point_green_led_GPIO_Port, jack_1_zero_point_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[0] > 0) HAL_GPIO_WritePin(jack_1_zero_point_red_led_GPIO_Port, jack_1_zero_point_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_zero_point_red_led_GPIO_Port, jack_1_zero_point_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[1] > 0) HAL_GPIO_WritePin(jack_1_down_limit_green_led_GPIO_Port, jack_1_down_limit_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_down_limit_green_led_GPIO_Port, jack_1_down_limit_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[1] > 0) HAL_GPIO_WritePin(jack_1_down_limit_red_led_GPIO_Port, jack_1_down_limit_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_down_limit_red_led_GPIO_Port, jack_1_down_limit_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[2] > 0) HAL_GPIO_WritePin(jack_1_up_limit_green_led_GPIO_Port, jack_1_up_limit_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_up_limit_green_led_GPIO_Port, jack_1_up_limit_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[2] > 0) HAL_GPIO_WritePin(jack_1_up_limit_red_led_GPIO_Port, jack_1_up_limit_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_1_up_limit_red_led_GPIO_Port, jack_1_up_limit_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[3] > 0) HAL_GPIO_WritePin(motor_1_green_led_GPIO_Port, motor_1_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(motor_1_green_led_GPIO_Port, motor_1_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[3] > 0) HAL_GPIO_WritePin(motor_1_red_led_GPIO_Port, motor_1_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(motor_1_red_led_GPIO_Port, motor_1_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[4] > 0) HAL_GPIO_WritePin(jack_2_zero_point_green_led_GPIO_Port, jack_2_zero_point_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_zero_point_green_led_GPIO_Port, jack_2_zero_point_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[4] > 0) HAL_GPIO_WritePin(jack_2_zero_point_red_led_GPIO_Port, jack_2_zero_point_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_zero_point_red_led_GPIO_Port, jack_2_zero_point_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[5] > 0) HAL_GPIO_WritePin(jack_2_down_limit_green_led_GPIO_Port, jack_2_down_limit_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_down_limit_green_led_GPIO_Port, jack_2_down_limit_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[5] > 0) HAL_GPIO_WritePin(jack_2_down_limit_red_led_GPIO_Port, jack_2_down_limit_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_down_limit_red_led_GPIO_Port, jack_2_down_limit_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[6] > 0) HAL_GPIO_WritePin(jack_2_up_limit_green_led_GPIO_Port, jack_2_up_limit_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_up_limit_green_led_GPIO_Port, jack_2_up_limit_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[6] > 0) HAL_GPIO_WritePin(jack_2_up_limit_red_led_GPIO_Port, jack_2_up_limit_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(jack_2_up_limit_red_led_GPIO_Port, jack_2_up_limit_red_led_Pin, GPIO_PIN_RESET);
	
	if(green_led[7] > 0) HAL_GPIO_WritePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin, GPIO_PIN_RESET);
	
	if(red_led[7] > 0) HAL_GPIO_WritePin(motor_2_red_led_GPIO_Port, motor_2_red_led_Pin, GPIO_PIN_SET);
	else HAL_GPIO_WritePin(motor_2_red_led_GPIO_Port, motor_2_red_led_Pin, GPIO_PIN_RESET);
	
}


int16_t getInterfaceEncoderPosition(){
		return interface.encoder_position;
}

void setValueMenu(){
	if(interface.state[0] > 0){
		interface.green_led[interface.state[0] - 1] = 1;
		interface.red_led[interface.state[0] - 1] = 1;
	}
	else if(interface.state[1] > 0){
		interface.green_led[interface.state[0] + 3] = 1;
		interface.red_led[interface.state[0] + 3] = 1;
	}
}

void mainMenu(){

					if(setting_state[0] == SETTING_NOTHING){
					
						interface.green_led[0] = 0;
						interface.red_led[0] = 1;
						interface.green_led[1] = 0;
						interface.red_led[1] = 1;
						interface.green_led[2] = 0;
						interface.red_led[2] = 1;
						interface.green_led[3] = 0;
						interface.red_led[3] = 1;
					}
					else if(setting_state[0] == SETTING_JACK_ZERO_POINT_SET){
						interface.green_led[0] = 1;
						interface.red_led[0] = 0;
						interface.green_led[1] = 0;
						interface.red_led[1] = 1;
						interface.green_led[2] = 0;
						interface.red_led[2] = 1;
						interface.green_led[3] = 0;
						interface.red_led[3] = 1;
					
					}
					else if(setting_state[0] == SETTING_JACK_DOWN_LIMIT_SET){
						interface.green_led[0] = 1;
						interface.red_led[0] = 0;
						interface.green_led[1] = 1;
						interface.red_led[1] = 0;
						interface.green_led[2] = 0;
						interface.red_led[2] = 1;
						interface.green_led[3] = 0;
						interface.red_led[3] = 1;
					}
					else if(setting_state[0] == SETTING_JACK_UP_LIMIT_SET){
						interface.green_led[0] = 1;
						interface.red_led[0] = 0;
						interface.green_led[1] = 1;
						interface.red_led[1] = 0;
						interface.green_led[2] = 1;
						interface.red_led[2] = 0;
						interface.green_led[3] = 0;
						interface.red_led[3] = 1;
					}
					else{
						interface.green_led[0] = 1;
						interface.red_led[0] = 0;
						interface.green_led[1] = 1;
						interface.red_led[1] = 0;
						interface.green_led[2] = 1;
						interface.red_led[2] = 0;
						interface.green_led[3] = 1;
						interface.red_led[3] = 0;
					}
					
					if(setting_state[1] == SETTING_NOTHING){
					
						interface.green_led[4] = 0;
						interface.red_led[4] = 1;
						interface.green_led[5] = 0;
						interface.red_led[5] = 1;
						interface.green_led[6] = 0;
						interface.red_led[6] = 1;
						interface.green_led[7] = 0;
						interface.red_led[7] = 1;
					}
					else if(setting_state[1] == SETTING_JACK_ZERO_POINT_SET){
						interface.green_led[4] = 1;
						interface.red_led[4] = 0;
						interface.green_led[5] = 0;
						interface.red_led[5] = 1;
						interface.green_led[6] = 0;
						interface.red_led[6] = 1;
						interface.green_led[7] = 0;
						interface.red_led[7] = 1;
					
					}
					else if(setting_state[1] == SETTING_JACK_DOWN_LIMIT_SET){
						interface.green_led[4] = 1;
						interface.red_led[4] = 0;
						interface.green_led[5] = 1;
						interface.red_led[5] = 0;
						interface.green_led[6] = 0;
						interface.red_led[6] = 1;
						interface.green_led[7] = 0;
						interface.red_led[7] = 1;
					}
					else if(setting_state[1] == SETTING_JACK_UP_LIMIT_SET){
						interface.green_led[4] = 1;
						interface.red_led[4] = 0;
						interface.green_led[5] = 1;
						interface.red_led[5] = 0;
						interface.green_led[6] = 1;
						interface.red_led[6] = 0;
						interface.green_led[7] = 0;
						interface.red_led[7] = 1;
					}
					else{
						interface.green_led[4] = 1;
						interface.red_led[4] = 0;
						interface.green_led[5] = 1;
						interface.red_led[5] = 0;
						interface.green_led[6] = 1;
						interface.red_led[6] = 0;
						interface.green_led[7] = 1;
						interface.red_led[7] = 0;
					}

}

void settingMenu(){
					switch(setting_state[0]){
						case SETTING_NOTHING:
					
							if(interface.encoder_position < 4) interface.red_led[0] = 1;
							else interface.red_led[0] = 0;
							interface.red_led[1] = 0;
							interface.red_led[2] = 0;
							interface.red_led[3] = 0;
							
							interface.green_led[0] = 0;
							interface.green_led[1] = 0;
							interface.green_led[2] = 0;
							interface.green_led[3] = 0;
					
						break;
						case SETTING_JACK_ZERO_POINT_SET:
					
							if(interface.encoder_position == 0) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.green_led[0] = 1;
								interface.green_led[1] = 0;
							
							}	
							else if(interface.encoder_position < 4) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 1;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
							}
							else{
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
							}
						
							interface.red_led[2] = 0;
							interface.red_led[3] = 0;
							interface.green_led[2] = 0;
							interface.green_led[3] = 0;
					
						break;
						case SETTING_JACK_DOWN_LIMIT_SET:
					
							if(interface.encoder_position == 0) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.green_led[0] = 1;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
							}
							else if(interface.encoder_position == 1) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 1;
								interface.green_led[2] = 0;
							}
							else if(interface.encoder_position < 4) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 1;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
							}
							else{
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
							}
						
							interface.red_led[3] = 0;
							interface.green_led[3] = 0;
								
					
						break;
						case SETTING_JACK_UP_LIMIT_SET:
							if(interface.encoder_position == 0) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 1;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 1) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 1;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 2) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 1;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 3) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 1;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
							else{
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
						break;	
						default:
							if(interface.encoder_position == 0) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 1;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 1) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 1;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 2) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 1;
								interface.green_led[3] = 0;
							}
							else if(interface.encoder_position == 3) {
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 1;
							}
							else{
								interface.red_led[0] = 0;
								interface.red_led[1] = 0;
								interface.red_led[2] = 0;
								interface.red_led[3] = 0;
								interface.green_led[0] = 0;
								interface.green_led[1] = 0;
								interface.green_led[2] = 0;
								interface.green_led[3] = 0;
							}
						
							
							
						break;
					}
					
					switch(setting_state[1]){
						case SETTING_NOTHING:
					
							if(interface.encoder_position > 3) interface.red_led[4] = 1;
							else interface.red_led[4] = 0;
							interface.red_led[5] = 0;
							interface.red_led[6] = 0;
							interface.red_led[7] = 0;
					
						break;
						case SETTING_JACK_ZERO_POINT_SET:
					
							if(interface.encoder_position == 4) {
								interface.red_led[4] = 1;
								interface.red_led[5] = 0;
							}	
							else if(interface.encoder_position > 4) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 1;
							}
							else{
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
							}
						
							interface.red_led[6] = 0;
							interface.red_led[7] = 0;
					
						break;
						case SETTING_JACK_DOWN_LIMIT_SET:
					
							if(interface.encoder_position == 4) {
								interface.red_led[4] = 1;
								interface.red_led[5] = 0;
								interface.red_led[6] = 0;
							}
							else if(interface.encoder_position == 5) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 1;
								interface.red_led[6] = 0;
							}
							else if(interface.encoder_position > 5) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
								interface.red_led[6] = 1;
							}
							else{
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
								interface.red_led[6] = 0;
							}
						
							interface.red_led[7] = 0;
					
						break;
						default:
							if(interface.encoder_position == 4) {
								interface.red_led[4] = 1;
								interface.red_led[5] = 0;
								interface.red_led[6] = 0;
								interface.red_led[7] = 0;
							}
							else if(interface.encoder_position == 5) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 1;
								interface.red_led[6] = 0;
								interface.red_led[7] = 0;
							}
							else if(interface.encoder_position == 6) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
								interface.red_led[6] = 1;
								interface.red_led[7] = 0;
							}
							else if(interface.encoder_position == 7) {
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
								interface.red_led[6] = 0;
								interface.red_led[7] = 1;
							}
							else{
								interface.red_led[4] = 0;
								interface.red_led[5] = 0;
								interface.red_led[6] = 0;
								interface.red_led[7] = 0;
							}
						
							
							
						break;
					}

}


void interfaceCallback(uint16_t GPIO_Pin)
{
		uint8_t B = 0;
	
		switch(GPIO_Pin){
			case interface_encoder_A_Pin:
				
				B = HAL_GPIO_ReadPin(interface_encoder_B_GPIO_Port, interface_encoder_B_Pin);
			
				switch(interface.menu_number){
					case 0:
						
						interface.encoder_position = 0;
					break;
					case 1:
						if(B == GPIO_PIN_SET) interface.encoder_position += 1;
						else interface.encoder_position -= 1;
						
						if(interface.encoder_position > 7) interface.encoder_position = 7;
						else if(interface.encoder_position < 0) interface.encoder_position = 0;
					break;
					case 2:
						if(B == GPIO_PIN_SET) interface.encoder_position += 1;
						else interface.encoder_position -= 1;
						
						if(interface.encoder_position > 1000) interface.encoder_position = 1000;
						else if(interface.encoder_position < 0) interface.encoder_position = 0;
					break;
				}
				
			break;	
			case interface_button_Pin:
				interface.encoder_btn_clicked += 1;
				interface.encoder_btn_clicked_timer = hundred_millis;
			break;
			
			default:
			break;	
		}
}
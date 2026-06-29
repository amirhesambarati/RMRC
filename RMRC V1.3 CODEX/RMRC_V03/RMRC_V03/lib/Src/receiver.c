#include "main.h"


uint32_t receiver_run_timer = 0;
uint32_t millis = 0;
uint32_t receiver_read_timer = 0;
// up1  down1  up2  down2
uint8_t  receiver_curr_order[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t  receiver_prev_order[8] = {0, 0, 0, 0, 0, 0, 0, 0};

RECEIVER_t receiver[2];

void receiverInit(void)
{
    for(uint8_t i = 0; i < 2; i++){
        receiver[i].up_order = 0;
        receiver[i].down_order = 0;
				receiver[i].up_order_counter = 0;
        receiver[i].down_order_counter = 0;
    }
		
}



void receiverRun(){
	millis = HAL_GetTick();
		
	if(millis < receiver_read_timer) receiver_read_timer = millis;
	
	if(millis > receiver_read_timer){
		receiver_read_timer = millis;
		
		receiver_curr_order[0] = (HAL_GPIO_ReadPin(receiver_1_up_first_order_GPIO_Port, receiver_1_up_first_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		receiver_curr_order[1] = (HAL_GPIO_ReadPin(receiver_1_up_second_order_GPIO_Port, receiver_1_up_second_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		
		if((receiver_curr_order[0] != receiver_prev_order[0]) || (receiver_curr_order[1] != receiver_prev_order[1])){
			receiver_prev_order[0] = receiver_curr_order[0];
			receiver_prev_order[1] = receiver_curr_order[1];
			receiver[0].up_order_counter += 1;
		}
		
		receiver_curr_order[2] = (HAL_GPIO_ReadPin(receiver_1_down_first_order_GPIO_Port, receiver_1_down_first_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		receiver_curr_order[3] = (HAL_GPIO_ReadPin(receiver_1_down_second_order_GPIO_Port, receiver_1_down_second_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		
		if((receiver_curr_order[2] != receiver_prev_order[2]) || (receiver_curr_order[3] != receiver_prev_order[3])){
			receiver_prev_order[2] = receiver_curr_order[2];
			receiver_prev_order[3] = receiver_curr_order[3];
			receiver[0].down_order_counter += 1;
		}
		
		receiver_curr_order[4] = (HAL_GPIO_ReadPin(receiver_2_up_first_order_GPIO_Port, receiver_2_up_first_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		receiver_curr_order[5] = (HAL_GPIO_ReadPin(receiver_2_up_second_order_GPIO_Port, receiver_2_up_second_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		
		if((receiver_curr_order[4] != receiver_prev_order[4]) || (receiver_curr_order[5] != receiver_prev_order[5])){
			receiver_prev_order[4] = receiver_curr_order[4];
			receiver_prev_order[5] = receiver_curr_order[5];
			receiver[1].up_order_counter += 1;
		}
		
		receiver_curr_order[6] = (HAL_GPIO_ReadPin(receiver_2_down_first_order_GPIO_Port, receiver_2_down_first_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		receiver_curr_order[7] = (HAL_GPIO_ReadPin(receiver_2_down_second_order_GPIO_Port, receiver_2_down_second_order_Pin) == GPIO_PIN_SET) ? 1 : 0;
		
		if((receiver_curr_order[6] != receiver_prev_order[6]) || (receiver_curr_order[7] != receiver_prev_order[7])){
			receiver_prev_order[6] = receiver_curr_order[6];
			receiver_prev_order[7] = receiver_curr_order[7];
			receiver[1].down_order_counter += 1;
		}
	}	
	
	if(hundred_millis > receiver_run_timer){
		receiver_run_timer = hundred_millis;
		
		for(uint8_t i = 0; i < 2; i++){
			if(receiver[i].up_order_counter > 4) receiver[i].up_order = 1;
			else receiver[i].up_order = 0;
			
			if(receiver[i].down_order_counter > 4) receiver[i].down_order = 1;
			else receiver[i].down_order = 0;
			
			receiver[i].up_order_counter = 0;
			receiver[i].down_order_counter = 0;
    }
		
		
	}	
	
	//if(receiver[0].down_order > 0) HAL_GPIO_WritePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin, GPIO_PIN_SET);
	//else HAL_GPIO_WritePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin, GPIO_PIN_RESET);
	
	//if(receiver[1].down_order > 0) HAL_GPIO_WritePin(motor_2_red_led_GPIO_Port, motor_2_red_led_Pin, GPIO_PIN_SET);
	//else HAL_GPIO_WritePin(motor_2_red_led_GPIO_Port, motor_2_red_led_Pin, GPIO_PIN_RESET);
}
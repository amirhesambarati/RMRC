#include "main.h"


uint32_t receiver_run_timer = 0;
uint8_t  receiver_order[2] = {0, 0};
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
	if(hundred_millis > receiver_run_timer){
		receiver_run_timer = hundred_millis;
		
		receiver_order[0] = HAL_GPIO_ReadPin(receiver_1_up_first_order_GPIO_Port, receiver_1_up_first_order_Pin);
		receiver_order[1] = HAL_GPIO_ReadPin(receiver_1_up_second_order_GPIO_Port, receiver_1_up_second_order_Pin);
		
		if((receiver_order[0] == GPIO_PIN_SET) || (receiver_order[1] == GPIO_PIN_SET)){
			if(receiver[0].up_order_counter < 10) receiver[0].up_order_counter += 1;
			else receiver[0].up_order_counter = 10;
		}
		
		receiver_order[0] = HAL_GPIO_ReadPin(receiver_1_down_first_order_GPIO_Port, receiver_1_down_first_order_Pin);
		receiver_order[1] = HAL_GPIO_ReadPin(receiver_1_down_second_order_GPIO_Port, receiver_1_down_second_order_Pin);
		
		if((receiver_order[0] == GPIO_PIN_SET) || (receiver_order[1] == GPIO_PIN_SET)){
			if(receiver[0].down_order_counter < 10) receiver[0].down_order_counter += 1;
			else receiver[0].down_order_counter = 10;
		}
		
		receiver_order[0] = HAL_GPIO_ReadPin(receiver_2_up_first_order_GPIO_Port, receiver_2_up_first_order_Pin);
		receiver_order[1] = HAL_GPIO_ReadPin(receiver_2_up_second_order_GPIO_Port, receiver_2_up_second_order_Pin);
		
		if((receiver_order[0] == GPIO_PIN_SET) || (receiver_order[1] == GPIO_PIN_SET)){
			if(receiver[1].up_order_counter < 10) receiver[1].up_order_counter += 1;
			else receiver[1].up_order_counter = 10;
		}
		
		receiver_order[0] = HAL_GPIO_ReadPin(receiver_2_down_first_order_GPIO_Port, receiver_2_down_first_order_Pin);
		receiver_order[1] = HAL_GPIO_ReadPin(receiver_2_down_second_order_GPIO_Port, receiver_2_down_second_order_Pin);
		
		if((receiver_order[0] == GPIO_PIN_SET) || (receiver_order[1] == GPIO_PIN_SET)){
			if(receiver[1].down_order_counter < 10) receiver[1].down_order_counter += 1;
			else receiver[1].down_order_counter = 10;
		}
		
		for(uint8_t i = 0; i < 2; i++){
			if(receiver[i].up_order_counter > 4) receiver[i].up_order = 1;
			else receiver[i].up_order = 0;
			
			if(receiver[i].down_order_counter > 4) receiver[i].down_order = 1;
			else receiver[i].down_order = 0;
    }
		
		
	}

}
#include "main.h"


eeprom24_t AT24_Data;

//I2C_HandleTypeDef *i2c;
extern I2C_HandleTypeDef hi2c2;
uint8_t tx_buf[16];
uint8_t tx_buf_2[8];
uint8_t rx_buf[EEPROM_SIZE];

// 0 = idle
// 1 = writing
// 2 = waiting ACK
// 3 = reading
volatile uint8_t AT24_State = 0;
uint32_t AT24_write_timer = 0;
volatile uint32_t AT24_page_write_timer = 0;
volatile uint8_t AT24_page_write_counter = 0;
volatile uint8_t AT24_write_flag = 0;
volatile uint8_t AT24_first_read_flag = 1;

uint8_t supply_feedback = 0;


void AT24Init(I2C_HandleTypeDef *hi2c_in)
{
    //i2c = hi2c_in;
    AT24_State = 0;
	
		for(uint8_t j = 0; j < 12; j++) AT24_Data.vars[j] = 0;
	
		for(uint8_t k = 0; k < 2; k++){
			AT24_Data.jack_prev_position[k] = 0;
			AT24_Data.jack_prev_close_limit[k] = 0;
			AT24_Data.jack_prev_open_limit[k] = 0;
			AT24_Data.motor_prev_position[k] = 0;
			AT24_Data.motor_prev_open_limit[k] = 0;
			AT24_Data.setting_prev_state[k] = 0;
		}	
}




// ------------------ WRITE ALL IT ------------------
HAL_StatusTypeDef AT24_WriteAll_IT(void)
{


    for(uint8_t f = 0; f < 8; f++)
    {
        tx_buf[(f*2)]   = (AT24_Data.vars[f] >> 8) & 0xFF;
        tx_buf[(f*2)+1] =  AT24_Data.vars[f]       & 0xFF;
    }

    AT24_State = 1;
		
		
		AT24_page_write_counter = 0;

    return HAL_I2C_Mem_Write_IT(&hi2c2,
                                AT24_ADDR,
                                0x00,
                                I2C_MEMADD_SIZE_8BIT,
                                tx_buf,
                                16);
}

// ------------------ READ ALL IT -------------------
HAL_StatusTypeDef AT24_ReadAll_IT(void)
{
    if(AT24_State != 0) return HAL_BUSY;

    AT24_State = 3;

    return HAL_I2C_Mem_Read_IT(&hi2c2,
                               AT24_ADDR,
                               0x00,
                               I2C_MEMADD_SIZE_8BIT,
                               rx_buf,
                               24);
}

// ------------------ TASK  ----------------------------------
void AT24Run(void)
{
		if(AT24_State == 4){
			if(HAL_GetTick() < AT24_page_write_timer) AT24_page_write_timer = HAL_GetTick();
			if(HAL_GetTick() > AT24_page_write_timer + 5){ // 5
				AT24_page_write_timer = HAL_GetTick();
				if(AT24_page_write_counter > 0){
					for (uint8_t t = 0; t < 4; t++)
					{
						tx_buf_2[(t*2)]   = (AT24_Data.vars[t+8] >> 8) & 0xFF;
						tx_buf_2[(t*2)+1] =  AT24_Data.vars[t+8]       & 0xFF;
					}
					HAL_I2C_Mem_Write_IT(&hi2c2,
                                AT24_ADDR,
                                0x10,
                                I2C_MEMADD_SIZE_8BIT,
                                tx_buf_2,
                                8);
				}
			
				
				
			}
			
			
		
		}
    else if (AT24_State == 2){
			if(HAL_GetTick() < AT24_page_write_timer) AT24_page_write_timer = HAL_GetTick();
			if(HAL_GetTick() > AT24_page_write_timer + 5){
        if (HAL_I2C_IsDeviceReady(&hi2c2, AT24_ADDR, 1, 1) == HAL_OK)
        {
						
						
						AT24_page_write_counter = 0;
						AT24_State = 0;
						AT24_ReadAll_IT();
						
						
				}
			}		
    }
		
		
		
		if(hundred_millis > AT24_write_timer + 1200){ // 2 min
			AT24_write_timer = hundred_millis;
			
			if((receiver[0].up_order == 0) && (receiver[1].up_order == 0) && (receiver[0].down_order == 0) && (receiver[1].down_order == 0)){
				if((interface.state[0] == INTERFACE_RUNNING) && (interface.state[1] == INTERFACE_RUNNING)){
					
					for(uint8_t g = 0; g < 2; g++){
						if(jack[g].close_limit != AT24_Data.jack_prev_close_limit[g]){
							AT24_Data.jack_prev_close_limit[g] = jack[g].close_limit;
							AT24_write_flag = 1;
						}
						if(jack[g].position != AT24_Data.jack_prev_position[g]){
							AT24_Data.jack_prev_position[g] = jack[g].position;
							AT24_write_flag = 1;
						}
						if(jack[g].open_limit != AT24_Data.jack_prev_open_limit[g]){
							AT24_Data.jack_prev_open_limit[g] = jack[g].open_limit;
							AT24_write_flag = 1;
						}
						
						if(motor[g].position != AT24_Data.motor_prev_position[g]){
							AT24_Data.motor_prev_position[g] = motor[g].position;
							AT24_write_flag = 1;
						}
						if(motor[g].open_limit != AT24_Data.motor_prev_open_limit[g]){
							AT24_Data.motor_prev_open_limit[g] = motor[g].open_limit;
							AT24_write_flag = 1;
						}
						
						if(setting_state[g] != AT24_Data.setting_prev_state[g]){
							AT24_Data.setting_prev_state[g] = setting_state[g];//// g
							AT24_write_flag = 1;
						}
						
					}
					if((AT24_write_flag == 1) && (AT24_State == 0)){
						AT24_write_flag = 0;
				
						AT24_Data.vars[0] = setting_state[0];
						AT24_Data.vars[1] = jack[0].close_limit;
						AT24_Data.vars[2] = jack[0].position;
						AT24_Data.vars[3] = jack[0].open_limit;
						AT24_Data.vars[4] = motor[0].position;
						AT24_Data.vars[5] = motor[0].open_limit;
				
						AT24_Data.vars[6] = setting_state[1];
						AT24_Data.vars[7] = jack[1].close_limit;
						AT24_Data.vars[8] = jack[1].position;
						AT24_Data.vars[9] = jack[1].open_limit;
						AT24_Data.vars[10] = motor[1].position;
						AT24_Data.vars[11] = motor[1].open_limit;
				
				
						AT24_WriteAll_IT();
					}
					
				
				}
			
			}
			
		}
		
		
		supply_feedback = (HAL_GPIO_ReadPin(supply_feedback_GPIO_Port, supply_feedback_Pin) == GPIO_PIN_SET) ? 1 : 0;
		
		if(supply_feedback == 0){
			
			turnOffDevice();
			
						AT24_Data.vars[0] = setting_state[0];
						AT24_Data.vars[1] = jack[0].close_limit;
						AT24_Data.vars[2] = jack[0].position;
						AT24_Data.vars[3] = jack[0].open_limit;
						AT24_Data.vars[4] = motor[0].position;
						AT24_Data.vars[5] = motor[0].open_limit;
				
						AT24_Data.vars[6] = setting_state[1];
						AT24_Data.vars[7] = jack[1].close_limit;
						AT24_Data.vars[8] = jack[1].position;
						AT24_Data.vars[9] = jack[1].open_limit;
						AT24_Data.vars[10] = motor[1].position;
						AT24_Data.vars[11] = motor[1].open_limit;
			
						if(AT24_State == 0) AT24_WriteAll_IT();
		}
		
		
		//if(AT24_Data.vars[0] == 1) HAL_GPIO_WritePin(jack_2_up_limit_red_led_GPIO_Port, jack_2_up_limit_red_led_Pin, GPIO_PIN_SET);
		//else HAL_GPIO_WritePin(jack_2_up_limit_red_led_GPIO_Port, jack_2_up_limit_red_led_Pin, GPIO_PIN_RESET);
}


void turnOffDevice(){
	// jack
			HAL_GPIO_WritePin(jack_1_move_open_GPIO_Port, jack_1_move_open_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_1_move_close_GPIO_Port, jack_1_move_close_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_open_GPIO_Port, jack_2_move_open_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(jack_2_move_close_GPIO_Port, jack_2_move_close_Pin, GPIO_PIN_RESET);
	// motor
			HAL_GPIO_WritePin(motor_1_move_up_GPIO_Port, motor_1_move_up_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_1_move_down_GPIO_Port, motor_1_move_down_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_2_move_up_GPIO_Port, motor_2_move_up_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(motor_2_move_down_GPIO_Port, motor_2_move_down_Pin, GPIO_PIN_RESET);
	// interface		
			offMenu();
			ledControl(interface.green_led, interface.red_led);
}

// ------------------ TX COMPLETE CALLBACK --------------------------------
void AT24_TxComplete(void)
{
		AT24_page_write_counter += 1;
	
		if(AT24_page_write_counter < 2) AT24_State = 4;
		else AT24_State = 2;
		//AT24_State = 2;
		AT24_page_write_timer = HAL_GetTick();
		//HAL_GPIO_TogglePin(motor_2_red_led_GPIO_Port, motor_2_red_led_Pin);
}

// ------------------ RX COMPLETE CALLBACK --------------------------------
void AT24_RxComplete(void)
{
		
		
		if((rx_buf[0] == 0xFF) && (rx_buf[1] == 0xFF)){
			AT24_Data.vars[0] = setting_state[0];
			AT24_Data.vars[1] = jack[0].close_limit;
			AT24_Data.vars[2] = jack[0].position;
			AT24_Data.vars[3] = jack[0].open_limit;
			AT24_Data.vars[4] = motor[0].position;
			AT24_Data.vars[5] = motor[0].open_limit;
				
			AT24_Data.vars[6] = setting_state[1];
			AT24_Data.vars[7] = jack[1].close_limit;
			AT24_Data.vars[8] = jack[1].position;
			AT24_Data.vars[9] = jack[1].open_limit;
			AT24_Data.vars[10] = motor[1].position;
			AT24_Data.vars[11] = motor[1].open_limit;
			AT24_WriteAll_IT();
			
			//AT24_write_flag = 1;
			//AT24_State = 4;	
		}
		else{
			AT24_State = 0;
			for(uint8_t m = 0; m < 12; m++){ // 12
					AT24_Data.vars[m] = (((uint16_t)rx_buf[(m*2)]) << 8) | ((uint16_t)rx_buf[(m*2)+1]);
			}
			/*
			if((AT24_Data.vars[0] != setting_state[0] || AT24_Data.vars[1] != jack[0].close_limit) ||
					 (AT24_Data.vars[2] != jack[0].position  || AT24_Data.vars[3] != jack[0].open_limit) ||
					 (AT24_Data.vars[4] != motor[0].position || AT24_Data.vars[5] != motor[0].open_limit)||
					 (AT24_Data.vars[6] != setting_state[1] || AT24_Data.vars[7] != jack[1].close_limit) ||
					 (AT24_Data.vars[8] != jack[1].position || AT24_Data.vars[9] != jack[1].open_limit)  ||
					 (AT24_Data.vars[10] != motor[1].position || AT24_Data.vars[11] != motor[1].open_limit)) 
				{*/
			
			if(AT24_first_read_flag == 0){
				if((AT24_Data.vars[0] != setting_state[0] || AT24_Data.vars[1] != jack[0].close_limit) ||
					 (AT24_Data.vars[2] != jack[0].position  || AT24_Data.vars[3] != jack[0].open_limit) ||
					 (AT24_Data.vars[4] != motor[0].position || AT24_Data.vars[5] != motor[0].open_limit)||
					 (AT24_Data.vars[6] != setting_state[1] || AT24_Data.vars[7] != jack[1].close_limit) ||
					 (AT24_Data.vars[8] != jack[1].position || AT24_Data.vars[9] != jack[1].open_limit)  ||
					 (AT24_Data.vars[10] != motor[1].position || AT24_Data.vars[11] != motor[1].open_limit)){
							//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);
							AT24_first_read_flag = 2;  // 2
							AT24_write_flag = 1;
							
				}
			}

			
		
			if(AT24_first_read_flag < 2){
				//HAL_GPIO_TogglePin(motor_2_green_led_GPIO_Port, motor_2_green_led_Pin);
				setting_state[0] = AT24_Data.vars[0];
				
				switch(setting_state[0]){
				case SETTING_COMPLETED:
				
					jack[0].close_limit = AT24_Data.vars[1];
					jack[0].position = AT24_Data.vars[2];
					jack[0].open_limit = AT24_Data.vars[3];
					motor[0].position = AT24_Data.vars[4];
					motor[0].open_limit = AT24_Data.vars[5];
				break;	
				case SETTING_JACK_UP_LIMIT_SET:
				
					jack[0].close_limit = AT24_Data.vars[1];
					jack[0].position = AT24_Data.vars[2];
					jack[0].open_limit = AT24_Data.vars[3];
					motor[0].position = 10000;
					motor[0].open_limit = 50000;
				break;	
				case SETTING_JACK_DOWN_LIMIT_SET:
					jack[0].close_limit = AT24_Data.vars[1];
					jack[0].position = AT24_Data.vars[2];
					jack[0].open_limit = 50000;
					motor[0].position = 10000;
					motor[0].open_limit = 50000;
				break;	
				case SETTING_JACK_ZERO_POINT_SET:
					jack[0].close_limit = ZERO_POINT;
					jack[0].position = AT24_Data.vars[2];
					jack[0].open_limit = 50000;
					motor[0].position = 10000;
					motor[0].open_limit = 50000;
				break;	
				default:
					jack[0].close_limit = ZERO_POINT;
					jack[0].position = 10000;
					jack[0].open_limit = 50000;
					motor[0].position = 10000;
					motor[0].open_limit = 50000;
				break;	
			
				}
				
				
				
		
				setting_state[1] = AT24_Data.vars[6];
		
				switch(setting_state[1]){
				case SETTING_COMPLETED:
				
					jack[1].close_limit = AT24_Data.vars[7];
					jack[1].position = AT24_Data.vars[8];
					jack[1].open_limit = AT24_Data.vars[9];
					motor[1].position = AT24_Data.vars[10];
					motor[1].open_limit = AT24_Data.vars[11];
				break;	
				case SETTING_JACK_UP_LIMIT_SET:
				
					jack[1].close_limit = AT24_Data.vars[7];
					jack[1].position = AT24_Data.vars[8];
					jack[1].open_limit = AT24_Data.vars[9];
					motor[1].position = 10000;
					motor[1].open_limit = 50000;
				break;	
				case SETTING_JACK_DOWN_LIMIT_SET:
					jack[1].close_limit = AT24_Data.vars[7];
					jack[1].position = AT24_Data.vars[8];
					jack[1].open_limit = 50000;
					motor[1].position = 10000;
					motor[1].open_limit = 50000;
				break;	
				case SETTING_JACK_ZERO_POINT_SET:
					jack[1].close_limit = ZERO_POINT;
					jack[1].position = AT24_Data.vars[8];
					jack[1].open_limit = 50000;
					motor[1].position = 10000;
					motor[1].open_limit = 50000;
				break;	
				default:
					jack[1].close_limit = ZERO_POINT;
					jack[1].position = 10000;
					jack[1].open_limit = 50000;
					motor[1].position = 10000;
					motor[1].open_limit = 50000;
				break;	
			
				}
				
				//setJackPosition(0, jack[0].position);
				//setJackPosition(1, jack[1].position);
				//setMotorPosition(0, motor[0].position);
				//setMotorPosition(1, motor[1].position);
		
				AT24_Data.jack_prev_position[0] = jack[0].position;
				AT24_Data.jack_prev_close_limit[0] = jack[0].close_limit;
				AT24_Data.jack_prev_open_limit[0] = jack[0].open_limit ;
		
				AT24_Data.motor_prev_position[0] = motor[0].position;
				AT24_Data.motor_prev_open_limit[0] = motor[0].open_limit;
		
				AT24_Data.jack_prev_position[1] = jack[1].position;
				AT24_Data.jack_prev_close_limit[1] = jack[1].close_limit;
				AT24_Data.jack_prev_open_limit[1] = jack[1].open_limit ;
		
				AT24_Data.motor_prev_position[1] = motor[1].position;
				AT24_Data.motor_prev_open_limit[1] = motor[1].open_limit;
		
				AT24_Data.setting_prev_state[0] = setting_state[0];
				AT24_Data.setting_prev_state[1] = setting_state[1];
			}
		
			AT24_first_read_flag = 0;
	}
}




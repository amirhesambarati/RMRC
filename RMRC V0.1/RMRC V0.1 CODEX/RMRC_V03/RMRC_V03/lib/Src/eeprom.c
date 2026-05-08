#include "main.h"


eeprom24_t AT24_Data;

I2C_HandleTypeDef *i2c;
uint8_t tx_buf[EEPROM_SIZE];
uint8_t rx_buf[EEPROM_SIZE];

// 0 = idle
// 1 = writing
// 2 = waiting ACK
// 3 = reading
volatile uint8_t AT24_State = 0;
uint32_t AT24_write_timer = 0;
uint8_t AT24_write_flag = 0;

void AT24Init(I2C_HandleTypeDef *hi2c_in)
{
    i2c = hi2c_in;
    AT24_State = 0;
	
		for(uint8_t i = 0; i < 12; i++) AT24_Data.vars[i] = 0;
	
		for(uint8_t i = 0; i < 2; i++){
			AT24_Data.jack_prev_position[i] = 0;
			AT24_Data.jack_prev_close_limit[i] = 0;
			AT24_Data.jack_prev_open_limit[i] = 0;
			AT24_Data.motor_prev_position[i] = 0;
			AT24_Data.motor_prev_open_limit[i] = 0;
			AT24_Data.setting_prev_state[i] = 0;
		}	
}


uint8_t getAT24State(void)
{
    return AT24_State;
}

// ------------------ WRITE ALL IT ------------------
HAL_StatusTypeDef AT24_WriteAll_IT(void)
{
    if(AT24_State != 0)
        return HAL_BUSY;


    for (int i = 0; i < 12; i++)
    {
        tx_buf[i*2]   = (AT24_Data.vars[i] >> 8) & 0xFF;
        tx_buf[i*2+1] =  AT24_Data.vars[i]       & 0xFF;
    }

    AT24_State = 1;

    return HAL_I2C_Mem_Write_IT(i2c,
                                AT24_ADDR,
                                0,
                                I2C_MEMADD_SIZE_8BIT,
                                tx_buf,
                                EEPROM_SIZE);
}

// ------------------ READ ALL IT -------------------
HAL_StatusTypeDef AT24_ReadAll_IT(void)
{
    if(AT24_State != 0) return HAL_BUSY;

    AT24_State = 3;

    return HAL_I2C_Mem_Read_IT(i2c,
                               AT24_ADDR,
                               0,
                               I2C_MEMADD_SIZE_8BIT,
                               rx_buf,
                               EEPROM_SIZE);
}

// ------------------ TASK  ----------------------------------
void AT24Run(void)
{
    if (AT24_State == 2){
        if (HAL_I2C_IsDeviceReady(i2c, AT24_ADDR, 1, 1) == HAL_OK)
        {
            AT24_State = 0; 
        }
    }
		
		if(hundred_millis > AT24_write_timer + 600){ // 60 sec
			AT24_write_timer = hundred_millis;
			
			if((receiver[0].up_order == 0) && (receiver[1].up_order == 0) && (receiver[0].down_order == 0) && (receiver[1].down_order == 0)){
				if((interface.state[0] == INTERFACE_RUNNING) && (interface.state[1] == INTERFACE_RUNNING)){
					for(uint8_t i = 0; i < 2; i++){
						if(jack[i].close_limit != AT24_Data.jack_prev_close_limit[i]){
							AT24_Data.jack_prev_close_limit[i] = jack[i].close_limit;
							AT24_write_flag = 1;
						}
						if(jack[i].position != AT24_Data.jack_prev_position[i]){
							AT24_Data.jack_prev_position[i] = jack[i].position;
							AT24_write_flag = 1;
						}
						if(jack[i].open_limit != AT24_Data.jack_prev_open_limit[i]){
							AT24_Data.jack_prev_open_limit[i] = jack[i].open_limit;
							AT24_write_flag = 1;
						}
						
						if(motor[i].position != AT24_Data.motor_prev_position[i]){
							AT24_Data.motor_prev_position[i] = motor[i].position;
							AT24_write_flag = 1;
						}
						if(motor[i].open_limit != AT24_Data.motor_prev_open_limit[i]){
							AT24_Data.motor_prev_open_limit[i] = motor[i].open_limit;
							AT24_write_flag = 1;
						}
						
						if(setting_state[i] != AT24_Data.setting_prev_state[i]){
							AT24_Data.setting_prev_state[i] = setting_state[i];
							AT24_write_flag = 1;
						}
						
						
						
					}	
				
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

// ------------------ TX COMPLETE CALLBACK --------------------------------
void AT24_TxComplete(void)
{

    AT24_State = 2;
}

// ------------------ RX COMPLETE CALLBACK --------------------------------
void AT24_RxComplete(void)
{
  
    for(uint8_t i = 0; i < 12; i++){
        AT24_Data.vars[i] = ((uint16_t)rx_buf[i*2] << 8) | (uint16_t)rx_buf[i*2+1];
    }

    AT24_State = 0;
		
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
				motor[0].position = 0;
				motor[0].open_limit = 0;
			break;	
			case SETTING_JACK_DOWN_LIMIT_SET:
				jack[0].close_limit = AT24_Data.vars[1];
				jack[0].position = AT24_Data.vars[2];
				jack[0].open_limit = 0;
				motor[0].position = 0;
				motor[0].open_limit = 0;
			break;	
			case SETTING_JACK_ZERO_POINT_SET:
				jack[0].close_limit = 0;
				jack[0].position = AT24_Data.vars[2];
				jack[0].open_limit = 0;
				motor[0].position = 0;
				motor[0].open_limit = 0;
			break;	
			default:
				jack[0].close_limit = 0;
				jack[0].position = 0;
				jack[0].open_limit = 0;
				motor[0].position = 0;
				motor[0].open_limit = 0;
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
				motor[1].position = 0;
				motor[1].open_limit = 0;
			break;	
			case SETTING_JACK_DOWN_LIMIT_SET:
				jack[1].close_limit = AT24_Data.vars[7];
				jack[1].position = AT24_Data.vars[8];
				jack[1].open_limit = 0;
				motor[1].position = 0;
				motor[1].open_limit = 0;
			break;	
			case SETTING_JACK_ZERO_POINT_SET:
				jack[1].close_limit = 0;
				jack[1].position = AT24_Data.vars[8];
				jack[1].open_limit = 0;
				motor[1].position = 0;
				motor[1].open_limit = 0;
			break;	
			default:
				jack[1].close_limit = 0;
				jack[1].position = 0;
				jack[1].open_limit = 0;
				motor[1].position = 0;
				motor[1].open_limit = 0;
			break;	
			
		}
		
		
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




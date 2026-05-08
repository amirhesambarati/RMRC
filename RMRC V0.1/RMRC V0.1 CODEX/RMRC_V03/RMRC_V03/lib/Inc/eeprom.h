#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#include "stm32g0xx_hal.h"
#include "main.h"

#define AT24_ADDR     (0x50 << 1)
#define EEPROM_SIZE   24

typedef struct {
    volatile uint16_t vars[12];   // 24 bytes
		volatile uint16_t jack_prev_position[2];
		volatile uint16_t jack_prev_close_limit[2];
		volatile uint16_t jack_prev_open_limit[2];
		volatile uint16_t motor_prev_position[2];
		volatile uint16_t motor_prev_open_limit[2];
		
		uint8_t setting_prev_state[2];
		
} eeprom24_t;




extern eeprom24_t AT24_Data;

extern uint32_t AT24_write_timer;
extern uint8_t AT24_write_flag;

extern volatile uint8_t AT24_State;

extern void AT24Init(I2C_HandleTypeDef *hi2c);


HAL_StatusTypeDef AT24_WriteAll_IT(void);
HAL_StatusTypeDef AT24_ReadAll_IT(void);


void AT24Run(void);


void AT24_TxComplete(void);
void AT24_RxComplete(void);


uint8_t AT24_GetState(void);

#endif

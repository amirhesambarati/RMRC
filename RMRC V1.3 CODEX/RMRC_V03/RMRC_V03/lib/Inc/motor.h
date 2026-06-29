#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32g0xx_hal.h"
#include "main.h"

typedef struct {
    volatile uint16_t position;
		uint16_t previous_position;
		
		volatile uint16_t open_limit;
		uint16_t close_limit;
	
		uint8_t total_length_flag;
		uint32_t total_length_timer;
} MOTOR_t;

extern MOTOR_t motor[2];

extern void motorRun(void);
extern void motorInit(void);

extern void motorControl(uint8_t id, uint8_t enable, uint8_t direction);


#endif

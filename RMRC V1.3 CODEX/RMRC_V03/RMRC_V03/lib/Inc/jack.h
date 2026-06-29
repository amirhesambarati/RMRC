#ifndef INC_JACK_H_
#define INC_JACK_H_

#include "stm32g0xx_hal.h"
#include "main.h"

typedef struct {
		volatile uint16_t position;
		uint16_t previous_position;
		uint16_t current_feedback;
		volatile uint16_t close_limit;
		volatile uint16_t open_limit;
		uint8_t zero_point_timer_flag;
		uint32_t zero_point_timer;
	
	
		uint16_t interface_close_limit;
		uint16_t interface_open_limit;
	
		uint8_t close_limit_set_flag;
		uint32_t close_limit_set_timer;
		uint8_t close_limit_set_counter;
		
		uint8_t open_limit_set_flag;
		uint32_t open_limit_set_timer;
		uint8_t open_limit_set_counter;
	
} JACK_t;

extern JACK_t jack[2];



extern void jackRun(void);
extern void jackInit(void);

extern void jackControl(uint8_t id, uint8_t enable, uint8_t direction);

#endif

#ifndef INC_RECEIVER_H_
#define INC_RECEIVER_H_

#include "stm32g0xx_hal.h"
#include "main.h"

typedef struct {
    
		uint8_t up_order;
		uint8_t down_order;
		uint8_t up_order_counter;
		uint8_t down_order_counter;
	
} RECEIVER_t;

extern RECEIVER_t receiver[2];

extern uint32_t receiver_run_timer;
extern uint8_t  receiver_order[2];

extern void receiverRun(void);
extern void receiverInit(void);
#endif

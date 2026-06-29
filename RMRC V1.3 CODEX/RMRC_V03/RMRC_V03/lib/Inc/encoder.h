#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "stm32g0xx_hal.h"
#include "main.h"

extern volatile uint16_t jack_1_position;
extern volatile uint16_t jack_2_position;
extern volatile uint16_t motor_1_position;
extern volatile uint16_t motor_2_position;

extern void setMotorPosition(uint8_t id, uint16_t position);
extern void setJackPosition(uint8_t id, uint16_t position);
extern uint16_t getJackPosition(uint8_t id);
extern uint16_t getMotorPosition(uint8_t id);
extern void encoderCallback(uint16_t GPIO_Pin);

#endif

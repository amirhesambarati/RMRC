#ifndef __READ_ENC_H__
#define __READ_ENC_H__

#include "main.h"
#include <stdint.h>

#define MOTOR_ENC_COUNT    2
#define JACK_ENC_COUNT     2

#define MOTOR_ENCODER_PPR  200
#define JACK_ENCODER_PPR   200

void AllEncoders_Init(void);
void AllEncoders_Update(void);

/* Motor encoder functions */
int32_t MotorEncoder_GetCount(uint8_t id);
int32_t MotorEncoder_GetSpeed(uint8_t id);
int32_t MotorEncoder_GetRPM(uint8_t id);
void    MotorEncoder_ResetCount(uint8_t id);

/* Jack encoder functions */
int32_t JackEncoder_GetCount(uint8_t id);
int32_t JackEncoder_GetSpeed(uint8_t id);
int32_t JackEncoder_GetRPM(uint8_t id);
void    JackEncoder_ResetCount(uint8_t id);

#endif

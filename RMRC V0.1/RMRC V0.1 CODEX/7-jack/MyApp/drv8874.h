#ifndef __DRV8874_H__
#define __DRV8874_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdio.h>
#include <stdint.h>

/* -------------------- Types -------------------- */
typedef enum
{
    DRV8874_MOTOR_1 = 0,
    DRV8874_MOTOR_2
} DRV8874_Motor_t;

typedef enum
{
    DRV8874_DIR_FORWARD = 0,
    DRV8874_DIR_REVERSE
} DRV8874_Direction_t;

typedef enum
{
    DRV8874_OK = 0,
    DRV8874_FAULT
} DRV8874_FaultStatus_t;

/* -------------------- Public API -------------------- */
void DRV8874_Init(void);
void DRV8874_Task(void);

void DRV8874_Wakeup(DRV8874_Motor_t motor);
void DRV8874_Sleep(DRV8874_Motor_t motor);

void DRV8874_SetDirection(DRV8874_Motor_t motor, DRV8874_Direction_t dir);

void DRV8874_Enable(DRV8874_Motor_t motor);
void DRV8874_Disable(DRV8874_Motor_t motor);

void DRV8874_Run(DRV8874_Motor_t motor, DRV8874_Direction_t dir);
void DRV8874_Stop(DRV8874_Motor_t motor);

DRV8874_FaultStatus_t DRV8874_GetFault(DRV8874_Motor_t motor);
void DRV8874_PrintStatus(DRV8874_Motor_t motor);

const char* DRV8874_MotorName(DRV8874_Motor_t motor);
const char* DRV8874_DirName(DRV8874_Direction_t dir);

#ifdef __cplusplus
}
#endif

#endif /* __DRV8874_H__ */

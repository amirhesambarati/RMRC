#ifndef STM32_PRINTF_UART_H
#define STM32_PRINTF_UART_H

#include "main.h"
#include <stdint.h>

void UART_TX_Task(void);
void Log(const char *log);

uint32_t UART_GetDropCount(void);
void UART_ClearDropCount(void);

#endif

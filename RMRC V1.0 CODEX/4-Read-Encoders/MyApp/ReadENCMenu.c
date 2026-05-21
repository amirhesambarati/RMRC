#include "ReadENCMenu.h"

#define Menu_ENCODER_MS  1

static uint32_t last_enc_menu_time = 0;

volatile int32_t encoder_count = 0;

static uint8_t prev_state = 0;
static int8_t accum = 0;

static uint8_t Encoder_ReadAB(void)
{
    uint8_t a = (HAL_GPIO_ReadPin(ENC_MENU_A_GPIO_Port, ENC_MENU_A_Pin) == GPIO_PIN_SET) ? 1 : 0;
    uint8_t b = (HAL_GPIO_ReadPin(ENC_MENU_B_GPIO_Port, ENC_MENU_B_Pin) == GPIO_PIN_SET) ? 1 : 0;
    return (a << 1) | b;
}

void MenuEncoder_Init(void)
{
    prev_state = Encoder_ReadAB();
    accum = 0;
    encoder_count = 0;
}

/*
Call every 1ms from SysTick or timer interrupt
Valid quadrature sequence:
00 -> 01 -> 11 -> 10 -> 00  (one direction)
00 -> 10 -> 11 -> 01 -> 00  (other direction)
*/
void MenuEncoder_Task(void)
{
	    uint32_t now = HAL_GetTick();
	        if ((now - last_enc_menu_time) < Menu_ENCODER_MS)
        {
            return;
        }
				last_enc_menu_time = now;

    static const int8_t trans_table[16] = {
         0, -1, +1,  0,
        +1,  0,  0, -1,
        -1,  0,  0, +1,
         0, +1, -1,  0
    };

    uint8_t curr_state = Encoder_ReadAB();
    uint8_t idx = (prev_state << 2) | curr_state;

    int8_t movement = trans_table[idx];

    if (movement != 0)
    {
        accum += movement;

				if (accum >= 2)
				{
						encoder_count++;
						accum = 0;
				}
				else if (accum <= -2)
				{
						encoder_count--;
						accum = 0;
				}

    }

    prev_state = curr_state;
}

int32_t MenuEncoder_GetCount(void)
{
    int32_t cnt;
    __disable_irq();
    cnt = encoder_count;
    __enable_irq();
    return cnt;
}
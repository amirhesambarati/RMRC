#include "time.h"

//extern TIM_HandleTypeDef htim2;

//uint32_t micros()
//{
//    return __HAL_TIM_GET_COUNTER(&htim2);
//}

uint32_t millis(void)
{
    return HAL_GetTick();   
}

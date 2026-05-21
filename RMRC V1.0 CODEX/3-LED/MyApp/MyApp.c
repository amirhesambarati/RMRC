
#include "MyApp.h"
#include "STM32PrintfUART.h"

extern UART_HandleTypeDef huart1;

void test1(){
	
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		HAL_GPIO_TogglePin(LED6_GPIO_Port, LED6_Pin);
		HAL_GPIO_TogglePin(LED7_GPIO_Port, LED7_Pin);
		HAL_GPIO_TogglePin(LED8_GPIO_Port, LED8_Pin);
		HAL_GPIO_TogglePin(LED9_GPIO_Port, LED9_Pin);
		HAL_GPIO_TogglePin(LED10_GPIO_Port, LED10_Pin);
		HAL_GPIO_TogglePin(LED11_GPIO_Port, LED11_Pin);
		HAL_GPIO_TogglePin(LED12_GPIO_Port, LED12_Pin);
		HAL_GPIO_TogglePin(LED13_GPIO_Port, LED13_Pin);
		HAL_GPIO_TogglePin(LED14_GPIO_Port, LED14_Pin);
		HAL_GPIO_TogglePin(LED15_GPIO_Port, LED15_Pin);
		HAL_GPIO_TogglePin(LED16_GPIO_Port, LED16_Pin);
		printf("end test1 .\r\n");
}

void My_App(){

	test1();
	HAL_Delay(1000);
	printf("end MyApp.\r\n");	
}

void My_App_init(){

	printf("System Started!\r\n");
	HAL_Delay(100);

}
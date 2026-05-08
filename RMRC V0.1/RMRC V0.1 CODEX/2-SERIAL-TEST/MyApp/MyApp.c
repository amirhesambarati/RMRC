
#include "MyApp.h"
#include "STM32PrintfUART.h"

extern UART_HandleTypeDef huart1;

void test1(){

		//HAL_UART_Transmit(&huart1,(uint8_t*)"Test SERIAL TX 1",16,100);
}

void test2(){

	Log("TEST Serial TX2!\r\n");

}

void test3(){

	printf("TEST Serial TX3!\r\n");

}

//void My_App()
//{
//    char msg[] = "UART1 blocking test\r\n";
//    HAL_GPIO_TogglePin(LED10_GPIO_Port, LED10_Pin);
//    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
//    HAL_Delay(1000);
//}


void My_App(){

	test1();
	HAL_Delay(1000);
	test2() ;
	HAL_Delay(1000);
	test3() ;
	HAL_Delay(1000);
	printf("System Started!\r\n");
	HAL_Delay(1);
	
}
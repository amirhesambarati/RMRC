
#include "STM32PrintfUART.h"

#define UART_BUFFER_SIZE 512

extern UART_HandleTypeDef huart1;

static volatile uint8_t tx_busy = 0;

static uint8_t dma_buf[256];  

static uint8_t uart_buffer[UART_BUFFER_SIZE];
static volatile uint16_t head = 0;
static volatile uint16_t tail = 0;

void RingBuffer_Write(uint8_t data){
    __disable_irq();
    uint16_t next = (head + 1) % UART_BUFFER_SIZE;
    if(next != tail)
    {
        uart_buffer[head] = data;
        head = next;
    }
    __enable_irq();
}

int RingBuffer_Read(uint8_t *data)
{
    __disable_irq();

    if(head == tail)
    {
        __enable_irq();
        return 0;
    }

    *data = uart_buffer[tail];
    tail = (tail + 1) % UART_BUFFER_SIZE;

    __enable_irq();
    return 1;
}

void Log(const char* log)
{
    size_t len = strlen(log);
    
    for(uint16_t i = 0; i < len; i++)
    {
        RingBuffer_Write((uint8_t)log[i]);
    }
}

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        RingBuffer_Write(ptr[i]);
    }
    return len;
}

int fputc(int ch, FILE *f)
{
    RingBuffer_Write((uint8_t)ch);
    return ch;
}

int __io_putchar(int ch)
{
    RingBuffer_Write((uint8_t)ch);
    return ch;
}


void UART_TX_Task(void)
{

		__disable_irq();
		if(tx_busy) { __enable_irq(); return; }
		if(head == tail) { __enable_irq(); return; }
		tx_busy = 1;
		__enable_irq();

    uint16_t count = 0;

    __disable_irq();
    uint16_t temp_tail = tail;
    uint16_t local_head = head;
		__enable_irq();
    while(count < sizeof(dma_buf) && temp_tail != local_head)
    {
        dma_buf[count++] = uart_buffer[temp_tail];
        temp_tail = (temp_tail + 1) % UART_BUFFER_SIZE;
    }


    if(count == 0) return;

		if(HAL_UART_Transmit_DMA(&huart1, dma_buf, count) == HAL_OK)
		{
				__disable_irq();
				tail = temp_tail;
				__enable_irq();
		}
		else
		{
				tx_busy = 0;
		}
		}

		void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
		{
				if(huart->Instance == USART1)
				{
						tx_busy = 0;
						UART_TX_Task(); 
				}
		}

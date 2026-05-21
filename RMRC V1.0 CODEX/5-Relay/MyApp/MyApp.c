#include "MyApp.h"
#include "STM32PrintfUART.h"
#include "ReadENC.h"
#include "ReadENCMenu.h"
#include "time.h"

static uint32_t last_enc_print = 0;
static uint32_t last_relay_toggle = 0;

void My_App_init(void)
{
    MenuEncoder_Init();
    AllEncoders_Init();

    printf("System Started!\r\n");

    HAL_Delay(100);
}

void testENC(void)
{
    MenuEncoder_Task();
    int32_t menu_count = MenuEncoder_GetCount();

    AllEncoders_Update();

    int32_t m1_count = MotorEncoder_GetCount(0);
    int32_t m1_rpm   = MotorEncoder_GetRPM(0);
    int32_t m1_speed = MotorEncoder_GetSpeed(0);

    int32_t m2_count = MotorEncoder_GetCount(1);
    int32_t m2_rpm   = MotorEncoder_GetRPM(1);
    int32_t m2_speed = MotorEncoder_GetSpeed(1);

    int32_t j1_count = JackEncoder_GetCount(0);
    int32_t j1_rpm   = JackEncoder_GetRPM(0);
    int32_t j1_speed = JackEncoder_GetSpeed(0);

    int32_t j2_count = JackEncoder_GetCount(1);
    int32_t j2_rpm   = JackEncoder_GetRPM(1);
    int32_t j2_speed = JackEncoder_GetSpeed(1);

    uint32_t now = millis();

    if (now - last_enc_print >= 1000)
    {
        last_enc_print = now;

        printf("Menu=%ld | "
               "M1: cnt=%ld rpm=%ld pps=%ld | "
               "M2: cnt=%ld rpm=%ld pps=%ld | "
               "J1: cnt=%ld rpm=%ld pps=%ld | "
               "J2: cnt=%ld rpm=%ld pps=%ld | "
               "time=%lu\r\n",
               (long)menu_count,

               (long)m1_count,
               (long)m1_rpm,
               (long)m1_speed,

               (long)m2_count,
               (long)m2_rpm,
               (long)m2_speed,

               (long)j1_count,
               (long)j1_rpm,
               (long)j1_speed,

               (long)j2_count,
               (long)j2_rpm,
               (long)j2_speed,

               (unsigned long)last_enc_print);
    }
}

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
} Relay_t;

Relay_t relays[] =
{
    {MOTOR_CLOSE_OUTPUT_1_GPIO_Port, MOTOR_CLOSE_OUTPUT_1_Pin},
    {MOTOR_OPEN_OUTPUT_1_GPIO_Port,  MOTOR_OPEN_OUTPUT_1_Pin},
    {MOTOR_CLOSE_OUTPUT_2_GPIO_Port, MOTOR_CLOSE_OUTPUT_2_Pin},
    {MOTOR_OPEN_OUTPUT_2_GPIO_Port,  MOTOR_OPEN_OUTPUT_2_Pin},
    {jack_close_1_GPIO_Port, jack_close_1_Pin},
    {jack_close_2_GPIO_Port, jack_close_2_Pin},
    {jack_open_1_GPIO_Port,  jack_open_1_Pin},
    {jack_open_2_GPIO_Port,  jack_open_2_Pin}
};

#define RELAY_COUNT (sizeof(relays)/sizeof(relays[0]))

void testRelay(void)
{
    static uint8_t relay_index = 0;
    uint32_t now = millis();

    if (now - last_relay_toggle >= 1000)
    {
        last_relay_toggle = now;

        HAL_GPIO_TogglePin(relays[relay_index].port,
                           relays[relay_index].pin);

        relay_index++;

        if (relay_index >= RELAY_COUNT)
            relay_index = 0;
    }
}


void My_App(void)
{
    testENC();
    testRelay();
}

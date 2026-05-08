#include "MyApp.h"
#include "STM32PrintfUART.h"
#include "ReadENC.h"
#include "ReadENCMenu.h"
#include "time.h"
#include "drv8874.h"

static uint32_t last_enc_print = 0;
static uint32_t last_relay_toggle = 0;

void My_App_init(void)
{
    MenuEncoder_Init();
    AllEncoders_Init();
		DRV8874_Init();
    printf("\r\n==== SYSTEM START ====\r\n");

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

void testRelay(void)
{
    uint32_t now = millis();

    if (now - last_relay_toggle >= 1000)
    {
        last_relay_toggle = now;

        HAL_GPIO_TogglePin(MOTOR_CLOSE_OUTPUT_1_GPIO_Port, MOTOR_CLOSE_OUTPUT_1_Pin);
        HAL_GPIO_TogglePin(MOTOR_OPEN_OUTPUT_1_GPIO_Port,  MOTOR_OPEN_OUTPUT_1_Pin);
        HAL_GPIO_TogglePin(MOTOR_CLOSE_OUTPUT_2_GPIO_Port, MOTOR_CLOSE_OUTPUT_2_Pin);
        HAL_GPIO_TogglePin(MOTOR_OPEN_OUTPUT_2_GPIO_Port,  MOTOR_OPEN_OUTPUT_2_Pin);
    }
}


//extern ADC_HandleTypeDef hadc1;

//static uint32_t last_adc_print = 0;

//static uint32_t ADC_Read_Channel(ADC_HandleTypeDef *hadc, uint32_t channel)
//{
//    ADC_ChannelConfTypeDef sConfig = {0};

//    sConfig.Channel = channel;
//    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;   // ???? ?? MCU ???? ??? ??? ???

//    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
//    {
//        return 0;
//    }

//    if (HAL_ADC_Start(hadc) != HAL_OK)
//    {
//        return 0;
//    }

//    if (HAL_ADC_PollForConversion(hadc, 10) != HAL_OK)
//    {
//        HAL_ADC_Stop(hadc);
//        return 0;
//    }

//    uint32_t value = HAL_ADC_GetValue(hadc);

//    HAL_ADC_Stop(hadc);

//    return value;
//}

//static float ADC_To_Voltage(uint32_t adc_value)
//{
//    return ((float)adc_value * 3.3f) / 4095.0f;
//}


//void testInputs(void)
//{
//    uint32_t now = millis();

//    if (now - last_adc_print >= 1000)
//    {
//        last_adc_print = now;

//        /* --------- ADC values ---------
//           ??? ????? ???????? ???? ?? MCU ????? ?? ????? ???? ????
//        */
//        uint32_t ipropi1_adc = ADC_Read_Channel(&hadc1, ADC_CHANNEL_x);   // ??????? ???
//        uint32_t ipropi2_adc = ADC_Read_Channel(&hadc1, ADC_CHANNEL_y);   // ??????? ???
//        uint32_t psfb_adc    = ADC_Read_Channel(&hadc1, ADC_CHANNEL_z);   // ??????? ???

//        float ipropi1_v = ADC_To_Voltage(ipropi1_adc);
//        float ipropi2_v = ADC_To_Voltage(ipropi2_adc);
//        float psfb_v    = ADC_To_Voltage(psfb_adc);

//        /* --------- Digital GPIO values --------- */
//        GPIO_PinState jack_fault_1 = HAL_GPIO_ReadPin(JACK_FUALT_1_GPIO_Port, JACK_FUALT_1_Pin);
//        GPIO_PinState jack_fault_2 = HAL_GPIO_ReadPin(JACK_FUALT_2_GPIO_Port, JACK_FUALT_2_Pin);

//        GPIO_PinState close_rx_3 = HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_3_GPIO_Port, CLOSE_RECEIVER_INPUT_3_Pin);
//        GPIO_PinState open_rx_3  = HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_3_GPIO_Port, OPEN_RECEIVER_INPUT_3_Pin);
//        GPIO_PinState close_rx_4 = HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_4_GPIO_Port, CLOSE_RECEIVER_INPUT_4_Pin);
//        GPIO_PinState open_rx_4  = HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_4_GPIO_Port, OPEN_RECEIVER_INPUT_4_Pin);
//        GPIO_PinState open_rx_2  = HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_2_GPIO_Port, OPEN_RECEIVER_INPUT_2_Pin);
//        GPIO_PinState close_rx_1 = HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_1_GPIO_Port, CLOSE_RECEIVER_INPUT_1_Pin);
//        GPIO_PinState open_rx_1  = HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_1_GPIO_Port, OPEN_RECEIVER_INPUT_1_Pin);
//        GPIO_PinState close_rx_2 = HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_2_GPIO_Port, CLOSE_RECEIVER_INPUT_2_Pin);

//        printf("ADC | IPROPI1=%lu (%.3fV) | IPROPI2=%lu (%.3fV) | PSFB=%lu (%.3fV)\r\n",
//               (unsigned long)ipropi1_adc, (double)ipropi1_v,
//               (unsigned long)ipropi2_adc, (double)ipropi2_v,
//               (unsigned long)psfb_adc,    (double)psfb_v);

//        printf("DIG | JF1=%d | JF2=%d | C3=%d | O3=%d | C4=%d | O4=%d | O2=%d | C1=%d | O1=%d | C2=%d\r\n",
//               (int)jack_fault_1,
//               (int)jack_fault_2,
//               (int)close_rx_3,
//               (int)open_rx_3,
//               (int)close_rx_4,
//               (int)open_rx_4,
//               (int)open_rx_2,
//               (int)close_rx_1,
//               (int)open_rx_1,
//               (int)close_rx_2);
//    }
//}

void testGPIOInputs(void)
{
    static uint32_t last_print = 0;
    uint32_t now = millis();

    if (now - last_print >= 1000)
    {
        last_print = now;

        printf("JF2=%d | IPROPI2=%d | PSFB=%d | IPROPI1=%d | JF1=%d | C3=%d | O3=%d | C4=%d | O4=%d | O2=%d | C1=%d | O1=%d | C2=%d\r\n",
               (int)HAL_GPIO_ReadPin(JACK_FUALT_2_GPIO_Port, JACK_FUALT_2_Pin),
               (int)HAL_GPIO_ReadPin(IPROPI2_GPIO_Port, IPROPI2_Pin),
               (int)HAL_GPIO_ReadPin(POWER_SUPPLY_FEEDBACK_GPIO_Port, POWER_SUPPLY_FEEDBACK_Pin),
               (int)HAL_GPIO_ReadPin(IPROPI1_GPIO_Port, IPROPI1_Pin),
               (int)HAL_GPIO_ReadPin(JACK_FUALT_1_GPIO_Port, JACK_FUALT_1_Pin),
               (int)HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_3_GPIO_Port, CLOSE_RECEIVER_INPUT_3_Pin),
               (int)HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_3_GPIO_Port, OPEN_RECEIVER_INPUT_3_Pin),
               (int)HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_4_GPIO_Port, CLOSE_RECEIVER_INPUT_4_Pin),
               (int)HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_4_GPIO_Port, OPEN_RECEIVER_INPUT_4_Pin),
               (int)HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_2_GPIO_Port, OPEN_RECEIVER_INPUT_2_Pin),
               (int)HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_1_GPIO_Port, CLOSE_RECEIVER_INPUT_1_Pin),
               (int)HAL_GPIO_ReadPin(OPEN_RECEIVER_INPUT_1_GPIO_Port, OPEN_RECEIVER_INPUT_1_Pin),
               (int)HAL_GPIO_ReadPin(CLOSE_RECEIVER_INPUT_2_GPIO_Port, CLOSE_RECEIVER_INPUT_2_Pin));
    }
}

void testDRV8874(void)
{
    typedef enum
    {
        TEST_IDLE = 0,
        TEST_M2_FWD_START,
        TEST_M2_FWD_WAIT,
        TEST_M2_STOP1,
        TEST_M2_STOP1_WAIT,
        TEST_M2_REV_START,
        TEST_M2_REV_WAIT,
        TEST_M2_STOP2,
        TEST_M2_STOP2_WAIT,
        TEST_DONE
    } TestState_t;

    static TestState_t state = TEST_IDLE;
    static uint32_t state_tick = 0;

    switch (state)
    {
        case TEST_IDLE:
            printf("\r\n--- MOTOR2 FORWARD TEST ---\r\n");
            DRV8874_Run(DRV8874_MOTOR_2, DRV8874_DIR_FORWARD);
            state_tick = HAL_GetTick();
            state = TEST_M2_FWD_WAIT;
            break;

        case TEST_M2_FWD_WAIT:
            if ((HAL_GetTick() - state_tick) >= 3000U)
            {
                DRV8874_Stop(DRV8874_MOTOR_2);
                state_tick = HAL_GetTick();
                state = TEST_M2_STOP1_WAIT;
            }
            break;

        case TEST_M2_STOP1_WAIT:
            if ((HAL_GetTick() - state_tick) >= 1000U)
            {
                printf("\r\n--- MOTOR2 REVERSE TEST ---\r\n");
                DRV8874_Run(DRV8874_MOTOR_2, DRV8874_DIR_REVERSE);
                state_tick = HAL_GetTick();
                state = TEST_M2_REV_WAIT;
            }
            break;

        case TEST_M2_REV_WAIT:
            if ((HAL_GetTick() - state_tick) >= 3000U)
            {
                DRV8874_Stop(DRV8874_MOTOR_2);
                state_tick = HAL_GetTick();
                state = TEST_M2_STOP2_WAIT;
            }
            break;

        case TEST_M2_STOP2_WAIT:
            if ((HAL_GetTick() - state_tick) >= 1000U)
            {
                printf("\r\n--- MOTOR2 TEST DONE ---\r\n");
                state = TEST_DONE;
            }
            break;

        case TEST_DONE:
        default:
            break;
    }
}


void My_App(void)
{
	
//    testENC();
//    testRelay();
//		testInputs();
//		testGPIOInputs();

    DRV8874_Task();
    testDRV8874();


	
}

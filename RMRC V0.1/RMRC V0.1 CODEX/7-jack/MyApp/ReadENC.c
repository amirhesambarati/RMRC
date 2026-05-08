#include "ReadENC.h"

typedef struct
{
    volatile int32_t pulse_counter;

    int32_t last_pulse_count;
    int32_t pulse_speed_pps;
    int32_t rpm;

    uint32_t last_speed_time;
} Encoder_t;

static Encoder_t motor_enc[MOTOR_ENC_COUNT];
static Encoder_t jack_enc[JACK_ENC_COUNT];

/* 
invert_dir = 0  -> normal direction
invert_dir = 1  -> reverse direction
*/
static void Encoder_ProcessEdge(Encoder_t *enc, GPIO_TypeDef *B_Port, uint16_t B_Pin, uint8_t invert_dir)
{
    GPIO_PinState b_state = HAL_GPIO_ReadPin(B_Port, B_Pin);

    if (invert_dir == 0)
    {
        if (b_state == GPIO_PIN_SET)
            enc->pulse_counter--;
        else
            enc->pulse_counter++;
    }
    else
    {
        if (b_state == GPIO_PIN_SET)
            enc->pulse_counter++;
        else
            enc->pulse_counter--;
    }
}

void AllEncoders_Init(void)
{
    uint32_t now = HAL_GetTick();
    uint8_t i;

    for (i = 0; i < MOTOR_ENC_COUNT; i++)
    {
        motor_enc[i].pulse_counter = 0;
        motor_enc[i].last_pulse_count = 0;
        motor_enc[i].pulse_speed_pps = 0;
        motor_enc[i].rpm = 0;
        motor_enc[i].last_speed_time = now;
    }

    for (i = 0; i < JACK_ENC_COUNT; i++)
    {
        jack_enc[i].pulse_counter = 0;
        jack_enc[i].last_pulse_count = 0;
        jack_enc[i].pulse_speed_pps = 0;
        jack_enc[i].rpm = 0;
        jack_enc[i].last_speed_time = now;
    }
}

/*
Use this callback if your HAL/project supports rising-edge specific EXTI callback.
If your project uses HAL_GPIO_EXTI_Callback(), move the same logic there.
*/
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    /* Motor 1 */
    if (GPIO_Pin == ENC_MOTOR1_A_Pin)
    {
        Encoder_ProcessEdge(&motor_enc[0], ENC_MOTOR1_B_GPIO_Port, ENC_MOTOR1_B_Pin, 0);
    }
    /* Motor 2 */
    else if (GPIO_Pin == ENC_MOTOR2_A_Pin)
    {
        Encoder_ProcessEdge(&motor_enc[1], ENC_MOTOR2_B_GPIO_Port, ENC_MOTOR2_B_Pin, 0);
    }
    /* Jack 1 */
    else if (GPIO_Pin == ENC_JACK1_A_Pin)
    {
        Encoder_ProcessEdge(&jack_enc[0], ENC_JACK1_B_GPIO_Port, ENC_JACK1_B_Pin, 0);
    }
    /* Jack 2 */
    else if (GPIO_Pin == ENC_JACK2_A_Pin)
    {
        Encoder_ProcessEdge(&jack_enc[1], ENC_JACK2_B_GPIO_Port, ENC_JACK2_B_Pin, 0);
    }
}

void AllEncoders_Update(void)
{
    uint32_t now = HAL_GetTick();
    uint32_t dt;
    int32_t pulse_now;
    int32_t dp;
    uint8_t i;

    /* Update motor encoders */
    for (i = 0; i < MOTOR_ENC_COUNT; i++)
    {
        dt = now - motor_enc[i].last_speed_time;

        if (dt >= 100)
        {
            __disable_irq();
            pulse_now = motor_enc[i].pulse_counter;
            __enable_irq();

            dp = pulse_now - motor_enc[i].last_pulse_count;

            motor_enc[i].pulse_speed_pps = (int32_t)((int64_t)dp * 1000 / dt);
            motor_enc[i].rpm = (motor_enc[i].pulse_speed_pps * 60) / MOTOR_ENCODER_PPR;

            motor_enc[i].last_pulse_count = pulse_now;
            motor_enc[i].last_speed_time = now;
        }
    }

    /* Update jack encoders */
    for (i = 0; i < JACK_ENC_COUNT; i++)
    {
        dt = now - jack_enc[i].last_speed_time;

        if (dt >= 100)
        {
            __disable_irq();
            pulse_now = jack_enc[i].pulse_counter;
            __enable_irq();

            dp = pulse_now - jack_enc[i].last_pulse_count;

            jack_enc[i].pulse_speed_pps = (int32_t)((int64_t)dp * 1000 / dt);
            jack_enc[i].rpm = (jack_enc[i].pulse_speed_pps * 60) / JACK_ENCODER_PPR;

            jack_enc[i].last_pulse_count = pulse_now;
            jack_enc[i].last_speed_time = now;
        }
    }
}

/* ===================== MOTOR FUNCTIONS ===================== */

int32_t MotorEncoder_GetCount(uint8_t id)
{
    int32_t count = 0;

    if (id >= MOTOR_ENC_COUNT)
        return 0;

    __disable_irq();
    count = motor_enc[id].pulse_counter;
    __enable_irq();

    return count;
}

int32_t MotorEncoder_GetSpeed(uint8_t id)
{
    if (id >= MOTOR_ENC_COUNT)
        return 0;

    return motor_enc[id].pulse_speed_pps;
}

int32_t MotorEncoder_GetRPM(uint8_t id)
{
    if (id >= MOTOR_ENC_COUNT)
        return 0;

    return motor_enc[id].rpm;
}

void MotorEncoder_ResetCount(uint8_t id)
{
    if (id >= MOTOR_ENC_COUNT)
        return;

    __disable_irq();
    motor_enc[id].pulse_counter = 0;
    motor_enc[id].last_pulse_count = 0;
    __enable_irq();

    motor_enc[id].pulse_speed_pps = 0;
    motor_enc[id].rpm = 0;
    motor_enc[id].last_speed_time = HAL_GetTick();
}

/* ===================== JACK FUNCTIONS ===================== */

int32_t JackEncoder_GetCount(uint8_t id)
{
    int32_t count = 0;

    if (id >= JACK_ENC_COUNT)
        return 0;

    __disable_irq();
    count = jack_enc[id].pulse_counter;
    __enable_irq();

    return count;
}

int32_t JackEncoder_GetSpeed(uint8_t id)
{
    if (id >= JACK_ENC_COUNT)
        return 0;

    return jack_enc[id].pulse_speed_pps;
}

int32_t JackEncoder_GetRPM(uint8_t id)
{
    if (id >= JACK_ENC_COUNT)
        return 0;

    return jack_enc[id].rpm;
}

void JackEncoder_ResetCount(uint8_t id)
{
    if (id >= JACK_ENC_COUNT)
        return;

    __disable_irq();
    jack_enc[id].pulse_counter = 0;
    jack_enc[id].last_pulse_count = 0;
    __enable_irq();

    jack_enc[id].pulse_speed_pps = 0;
    jack_enc[id].rpm = 0;
    jack_enc[id].last_speed_time = HAL_GetTick();
}

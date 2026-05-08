#include "drv8874.h"

/* -------------------- Pin Alias Mapping -------------------- */
#define DRV8874_M1_EN_Pin            JACK_EN_1_Pin
#define DRV8874_M1_EN_GPIO_Port      JACK_EN_1_GPIO_Port
#define DRV8874_M1_DIR_Pin           JACK_DIR_1_Pin
#define DRV8874_M1_DIR_GPIO_Port     JACK_DIR_1_GPIO_Port
#define DRV8874_M1_NSLEEP_Pin        JACK_nSLEEP_1_Pin
#define DRV8874_M1_NSLEEP_GPIO_Port  JACK_nSLEEP_1_GPIO_Port
#define DRV8874_M1_NFAULT_Pin        JACK_FUALT_1_Pin
#define DRV8874_M1_NFAULT_GPIO_Port  JACK_FUALT_1_GPIO_Port

#define DRV8874_M2_EN_Pin            JACK_EN_2_Pin
#define DRV8874_M2_EN_GPIO_Port      JACK_EN_2_GPIO_Port
#define DRV8874_M2_DIR_Pin           JACK_DIR_2_Pin
#define DRV8874_M2_DIR_GPIO_Port     JACK_DIR_2_GPIO_Port
#define DRV8874_M2_NSLEEP_Pin        JACK_nSLEEP_2_Pin
#define DRV8874_M2_NSLEEP_GPIO_Port  JACK_nSLEEP_2_GPIO_Port
#define DRV8874_M2_NFAULT_Pin        JACK_FUALT_2_Pin
#define DRV8874_M2_NFAULT_GPIO_Port  JACK_FUALT_2_GPIO_Port

/* -------------------- Configuration -------------------- */
#define DRV8874_PWM_PERIOD_MS     10U
#define DRV8874_START_DUTY        50U
#define DRV8874_TARGET_DUTY       90U
#define DRV8874_RAMP_TIME_MS      2000U

/* -------------------- Local Types -------------------- */
typedef struct
{
    uint8_t running;
    uint8_t pwm_enabled;
    uint8_t duty_percent;
    uint8_t start_duty;
    uint8_t target_duty;
    uint32_t ramp_ms;
    uint32_t pwm_period_ms;
    uint32_t start_tick;
    DRV8874_Direction_t dir;
} DRV8874_Runtime_t;

/* -------------------- Local Variables -------------------- */
static DRV8874_Runtime_t drv_runtime[2];

/* -------------------- Local Function Prototypes -------------------- */
static uint8_t DRV8874_GetIndex(DRV8874_Motor_t motor);

static GPIO_TypeDef* DRV8874_GetEnPort(DRV8874_Motor_t motor);
static uint16_t DRV8874_GetEnPin(DRV8874_Motor_t motor);

static GPIO_TypeDef* DRV8874_GetDirPort(DRV8874_Motor_t motor);
static uint16_t DRV8874_GetDirPin(DRV8874_Motor_t motor);

static GPIO_TypeDef* DRV8874_GetSleepPort(DRV8874_Motor_t motor);
static uint16_t DRV8874_GetSleepPin(DRV8874_Motor_t motor);

static GPIO_TypeDef* DRV8874_GetFaultPort(DRV8874_Motor_t motor);
static uint16_t DRV8874_GetFaultPin(DRV8874_Motor_t motor);

static const char* pin_state_str(GPIO_PinState s);
static DRV8874_FaultStatus_t DRV8874_GetFaultRaw(DRV8874_Motor_t motor);

/* -------------------- Public Helper Functions -------------------- */
const char* DRV8874_MotorName(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? "MOTOR_1" : "MOTOR_2";
}

const char* DRV8874_DirName(DRV8874_Direction_t dir)
{
    return (dir == DRV8874_DIR_FORWARD) ? "FORWARD" : "REVERSE";
}

/* -------------------- Public Functions -------------------- */
void DRV8874_Init(void)
{
    printf("\r\n[DRV8874] Init start\r\n");

    for (uint8_t i = 0; i < 2; i++)
    {
        drv_runtime[i].running       = 0U;
        drv_runtime[i].pwm_enabled   = 0U;
        drv_runtime[i].duty_percent  = 0U;
        drv_runtime[i].start_duty    = DRV8874_START_DUTY;
        drv_runtime[i].target_duty   = DRV8874_TARGET_DUTY;
        drv_runtime[i].ramp_ms       = DRV8874_RAMP_TIME_MS;
        drv_runtime[i].pwm_period_ms = DRV8874_PWM_PERIOD_MS;
        drv_runtime[i].start_tick    = 0U;
        drv_runtime[i].dir           = DRV8874_DIR_FORWARD;
    }

    DRV8874_Disable(DRV8874_MOTOR_1);
    DRV8874_Disable(DRV8874_MOTOR_2);

    printf("[DRV8874] Motors disabled\r\n");

    DRV8874_Wakeup(DRV8874_MOTOR_1);
    DRV8874_Wakeup(DRV8874_MOTOR_2);

    printf("[DRV8874] Wakeup command sent to both motors\r\n");

    HAL_Delay(5);

    DRV8874_PrintStatus(DRV8874_MOTOR_1);
    DRV8874_PrintStatus(DRV8874_MOTOR_2);

    printf("[DRV8874] Init done\r\n");
}

void DRV8874_Task(void)
{
    uint32_t now = HAL_GetTick();

    for (uint8_t idx = 0U; idx < 2U; idx++)
    {
        DRV8874_Motor_t motor = (idx == 0U) ? DRV8874_MOTOR_1 : DRV8874_MOTOR_2;

        if ((drv_runtime[idx].running == 0U) || (drv_runtime[idx].pwm_enabled == 0U))
        {
            continue;
        }

        if (DRV8874_GetFaultRaw(motor) == DRV8874_FAULT)
        {
            printf("[DRV8874] %s FAULT detected during run, stopping motor\r\n",
                   DRV8874_MotorName(motor));

            drv_runtime[idx].running = 0U;
            drv_runtime[idx].pwm_enabled = 0U;
            drv_runtime[idx].duty_percent = 0U;

            HAL_GPIO_WritePin(DRV8874_GetEnPort(motor),
                              DRV8874_GetEnPin(motor),
                              GPIO_PIN_RESET);
            continue;
        }

        {
            uint32_t elapsed = now - drv_runtime[idx].start_tick;

            if (elapsed >= drv_runtime[idx].ramp_ms)
            {
                drv_runtime[idx].duty_percent = drv_runtime[idx].target_duty;
            }
            else
            {
                uint32_t delta = (uint32_t)drv_runtime[idx].target_duty - (uint32_t)drv_runtime[idx].start_duty;

                drv_runtime[idx].duty_percent =
                    (uint8_t)(drv_runtime[idx].start_duty +
                    ((elapsed * delta) / drv_runtime[idx].ramp_ms));
            }
        }

        {
            uint32_t period  = drv_runtime[idx].pwm_period_ms;
            uint32_t on_time = (period * drv_runtime[idx].duty_percent) / 100U;
            uint32_t phase   = now % period;

            HAL_GPIO_WritePin(DRV8874_GetEnPort(motor),
                              DRV8874_GetEnPin(motor),
                              (phase < on_time) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        }
    }
}

void DRV8874_Wakeup(DRV8874_Motor_t motor)
{
    HAL_GPIO_WritePin(DRV8874_GetSleepPort(motor),
                      DRV8874_GetSleepPin(motor),
                      GPIO_PIN_SET);

    printf("[DRV8874] %s Wakeup -> nSLEEP=HIGH\r\n", DRV8874_MotorName(motor));
    DRV8874_PrintStatus(motor);
}

void DRV8874_Sleep(DRV8874_Motor_t motor)
{
    uint8_t idx = DRV8874_GetIndex(motor);

    drv_runtime[idx].running = 0U;
    drv_runtime[idx].pwm_enabled = 0U;
    drv_runtime[idx].duty_percent = 0U;

    HAL_GPIO_WritePin(DRV8874_GetEnPort(motor),
                      DRV8874_GetEnPin(motor),
                      GPIO_PIN_RESET);

    HAL_GPIO_WritePin(DRV8874_GetSleepPort(motor),
                      DRV8874_GetSleepPin(motor),
                      GPIO_PIN_RESET);

    printf("[DRV8874] %s Sleep -> nSLEEP=LOW\r\n", DRV8874_MotorName(motor));
    DRV8874_PrintStatus(motor);
}

void DRV8874_SetDirection(DRV8874_Motor_t motor, DRV8874_Direction_t dir)
{
    uint8_t idx = DRV8874_GetIndex(motor);

    drv_runtime[idx].dir = dir;

    if (dir == DRV8874_DIR_FORWARD)
    {
        HAL_GPIO_WritePin(DRV8874_GetDirPort(motor),
                          DRV8874_GetDirPin(motor),
                          GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(DRV8874_GetDirPort(motor),
                          DRV8874_GetDirPin(motor),
                          GPIO_PIN_RESET);
    }

    printf("[DRV8874] %s Direction set -> %s\r\n",
           DRV8874_MotorName(motor),
           DRV8874_DirName(dir));
    DRV8874_PrintStatus(motor);
}

void DRV8874_Enable(DRV8874_Motor_t motor)
{
    uint8_t idx = DRV8874_GetIndex(motor);

    drv_runtime[idx].running = 1U;
    drv_runtime[idx].pwm_enabled = 0U;
    drv_runtime[idx].duty_percent = 100U;

    HAL_GPIO_WritePin(DRV8874_GetEnPort(motor),
                      DRV8874_GetEnPin(motor),
                      GPIO_PIN_SET);

    printf("[DRV8874] %s Enable -> EN=HIGH\r\n", DRV8874_MotorName(motor));
    DRV8874_PrintStatus(motor);
}

void DRV8874_Disable(DRV8874_Motor_t motor)
{
    uint8_t idx = DRV8874_GetIndex(motor);

    drv_runtime[idx].running = 0U;
    drv_runtime[idx].pwm_enabled = 0U;
    drv_runtime[idx].duty_percent = 0U;

    HAL_GPIO_WritePin(DRV8874_GetEnPort(motor),
                      DRV8874_GetEnPin(motor),
                      GPIO_PIN_RESET);

    printf("[DRV8874] %s Disable -> EN=LOW\r\n", DRV8874_MotorName(motor));
    DRV8874_PrintStatus(motor);
}

void DRV8874_Run(DRV8874_Motor_t motor, DRV8874_Direction_t dir)
{
    uint8_t idx = DRV8874_GetIndex(motor);

    printf("[DRV8874] %s Run request -> dir=%s\r\n",
           DRV8874_MotorName(motor),
           DRV8874_DirName(dir));

    if (DRV8874_GetFaultRaw(motor) == DRV8874_FAULT)
    {
        printf("[DRV8874] %s FAULT detected before run, motor disabled\r\n",
               DRV8874_MotorName(motor));
        DRV8874_Disable(motor);
        return;
    }

    DRV8874_SetDirection(motor, dir);
    HAL_Delay(1);

    drv_runtime[idx].running       = 1U;
    drv_runtime[idx].pwm_enabled   = 1U;
    drv_runtime[idx].start_duty    = DRV8874_START_DUTY;
    drv_runtime[idx].target_duty   = DRV8874_TARGET_DUTY;
    drv_runtime[idx].duty_percent  = DRV8874_START_DUTY;
    drv_runtime[idx].ramp_ms       = DRV8874_RAMP_TIME_MS;
    drv_runtime[idx].pwm_period_ms = DRV8874_PWM_PERIOD_MS;
    drv_runtime[idx].start_tick    = HAL_GetTick();
    drv_runtime[idx].dir           = dir;

    printf("[DRV8874] %s Soft PWM started at %u%% duty, target=%u%%, ramp=%ums\r\n",
           DRV8874_MotorName(motor),
           drv_runtime[idx].start_duty,
           drv_runtime[idx].target_duty,
           drv_runtime[idx].ramp_ms);
}

void DRV8874_Stop(DRV8874_Motor_t motor)
{
    printf("[DRV8874] %s Stop request\r\n", DRV8874_MotorName(motor));
    DRV8874_Disable(motor);
}

DRV8874_FaultStatus_t DRV8874_GetFault(DRV8874_Motor_t motor)
{
    GPIO_PinState state;

    state = HAL_GPIO_ReadPin(DRV8874_GetFaultPort(motor),
                             DRV8874_GetFaultPin(motor));

    printf("[DRV8874] %s Fault pin read = %s\r\n",
           DRV8874_MotorName(motor),
           pin_state_str(state));

    if (state == GPIO_PIN_RESET)
    {
        return DRV8874_FAULT;
    }

    return DRV8874_OK;
}

void DRV8874_PrintStatus(DRV8874_Motor_t motor)
{
    GPIO_PinState sleep_state;
    GPIO_PinState dir_state;
    GPIO_PinState en_state;
    GPIO_PinState fault_state;

    sleep_state = HAL_GPIO_ReadPin(DRV8874_GetSleepPort(motor), DRV8874_GetSleepPin(motor));
    dir_state   = HAL_GPIO_ReadPin(DRV8874_GetDirPort(motor),   DRV8874_GetDirPin(motor));
    en_state    = HAL_GPIO_ReadPin(DRV8874_GetEnPort(motor),    DRV8874_GetEnPin(motor));
    fault_state = HAL_GPIO_ReadPin(DRV8874_GetFaultPort(motor), DRV8874_GetFaultPin(motor));

    printf("[DRV8874] %s STATUS | nSLEEP=%s | DIR=%s | EN=%s | nFAULT=%s\r\n",
           DRV8874_MotorName(motor),
           pin_state_str(sleep_state),
           pin_state_str(dir_state),
           pin_state_str(en_state),
           pin_state_str(fault_state));
}

/* -------------------- Local Helper Functions -------------------- */
static uint8_t DRV8874_GetIndex(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? 0U : 1U;
}

static const char* pin_state_str(GPIO_PinState s)
{
    return (s == GPIO_PIN_SET) ? "HIGH" : "LOW";
}

static DRV8874_FaultStatus_t DRV8874_GetFaultRaw(DRV8874_Motor_t motor)
{
    GPIO_PinState state;

    state = HAL_GPIO_ReadPin(DRV8874_GetFaultPort(motor),
                             DRV8874_GetFaultPin(motor));

    return (state == GPIO_PIN_RESET) ? DRV8874_FAULT : DRV8874_OK;
}

/* -------------------- Local Mapping Functions -------------------- */
static GPIO_TypeDef* DRV8874_GetEnPort(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_EN_GPIO_Port : DRV8874_M2_EN_GPIO_Port;
}

static uint16_t DRV8874_GetEnPin(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_EN_Pin : DRV8874_M2_EN_Pin;
}

static GPIO_TypeDef* DRV8874_GetDirPort(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_DIR_GPIO_Port : DRV8874_M2_DIR_GPIO_Port;
}

static uint16_t DRV8874_GetDirPin(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_DIR_Pin : DRV8874_M2_DIR_Pin;
}

static GPIO_TypeDef* DRV8874_GetSleepPort(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_NSLEEP_GPIO_Port : DRV8874_M2_NSLEEP_GPIO_Port;
}

static uint16_t DRV8874_GetSleepPin(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_NSLEEP_Pin : DRV8874_M2_NSLEEP_Pin;
}

static GPIO_TypeDef* DRV8874_GetFaultPort(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_NFAULT_GPIO_Port : DRV8874_M2_NFAULT_GPIO_Port;
}

static uint16_t DRV8874_GetFaultPin(DRV8874_Motor_t motor)
{
    return (motor == DRV8874_MOTOR_1) ? DRV8874_M1_NFAULT_Pin : DRV8874_M2_NFAULT_Pin;
}

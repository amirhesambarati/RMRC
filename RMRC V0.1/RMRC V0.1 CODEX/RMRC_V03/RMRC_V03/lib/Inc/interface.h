#ifndef INC_INTERFACE_H_
#define INC_INTERFACE_H_

#include "stm32g0xx_hal.h"
#include "main.h"

#define INTERFACE_RUNNING                 0
#define INTERFACE_JACK_ZERO_POINT_SET     1
#define INTERFACE_JACK_DOWN_LIMIT_SET     2
#define INTERFACE_JACK_UP_LIMIT_SET       3
#define INTERFACE_MOTOR_TOTAL_LENGTH_SET  4

#define SETTING_NOTHING                0
#define SETTING_JACK_ZERO_POINT_SET    1
#define SETTING_JACK_DOWN_LIMIT_SET    2
#define SETTING_JACK_UP_LIMIT_SET      3
#define SETTING_COMPLETED              4

typedef struct {
	
		// leds = zero point set - down limit - up limit - motor total length
		uint8_t green_led[8];
		uint8_t red_led[8];
		uint8_t setting_changed;
		volatile uint8_t encoder_btn_clicked;
		volatile int16_t encoder_position;
		volatile uint32_t encoder_btn_clicked_timer;
		uint8_t state[2];
		uint8_t menu_number;
		uint8_t set_value_completed;
			
} interface_t;

extern volatile uint8_t setting_state[2];


extern interface_t interface;



extern void mainMenu(void);
extern void settingMenu(void);
extern void setValueMenu(void);

extern int16_t getInterfaceEncoderPosition(void);
extern void interfaceInit(void);

extern void interfaceRun(void);
extern void ledControl(uint8_t green_led[8], uint8_t red_led[8]);

extern void interfaceCallback(uint16_t GPIO_Pin);
#endif

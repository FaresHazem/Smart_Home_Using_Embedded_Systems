/*
 * LED_Interface.h
 *
 * Created: 9/25/2023 7:12:44 PM
 *  Author: Beast
 */ 


#ifndef LED_INTERFACE_H_
#define LED_INTERFACE_H_

ES LED_enuInit(void);

ES LED_enuON(u8 copy_u8_led_id);

ES LED_enuOFF(u8 copy_u8_led_id);

ES LED_enuToggle(u8 copy_u8_led_id);

// Led Id's
#define LED_0  0
#define LED_1  1
#define LED_2  2
#define LED_3  3
#define LED_4  4


#endif /* LED_INTERFACE_H_ */
/*
 * EXTI_Config.h
 *
 * Created: 10/8/2023 9:25:10 PM
 *  Author: Beast
 */ 


#ifndef EXTI_CONFIG_H_
#define EXTI_CONFIG_H_

#define EXTI_MAX_PIN_NO				3

typedef struct{
	
	u8 interrupt_pin;
	u8 interrupt_mode;
	u8 interrupt_sense_level;
	
}EXTI_Str;


#endif /* EXTI_CONFIG_H_ */
/*
 * EXTI_Interface.h
 *
 * Created: 10/8/2023 9:24:35 PM
 *  Author: Beast
 */ 


#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

ES EXTI_enuInit(void);	// Initializes all EXTI in our controller

ES EXTI_enuSetInterruptSenseLevel(u8 copy_u8_interrupt_pin, u8 copy_u8_sense_level);	// Set sense level for an interrupt

ES EXTI_enuSetInterruptMode(u8 copy_u8_interrupt_pin, u8 copy_u8_mode);		// Set interrupt mode

ES EXTI_enuEnableInterrupt(u8 copy_u8_interrupt_pin);		// interrupt on

ES EXTI_enuDisableInterrupt(u8 copy_u8_interrupt_pin);		// interrupt off

ES EXTI_enuCallBack(u8 copy_u8_interrupt_pin, void(*copy_pfun_ptr)(void));						// Call back function to implement ISR without violation of SW design

#define EXTI_INTERRUPT_PIN0					0
#define EXTI_INTERRUPT_PIN1					1
#define EXTI_INTERRUPT_PIN2					2

#define EXTI_LOW_LEVEL						10
#define EXTI_ANY_LOGICAL_CHANGE				20
#define EXTI_FALLING_EDGE					30
#define EXTI_RISING_EDGE					40

#define EXTI_INTERRUPT_MODE					111
#define EXTI_POLLING_MODE					222

#endif /* EXTI_INTERFACE_H_ */
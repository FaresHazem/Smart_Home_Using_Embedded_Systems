/*
 * EXTI_Priv.h
 *
 * Created: 10/8/2023 9:24:20 PM
 *  Author: Beast
 */ 


#ifndef EXTI_PRIV_H_
#define EXTI_PRIV_H_

// Registers
#define MCUCR								*((volatile u8 *)0x55)				// Microcontroller Unit Control Register (Used to set detection type for Interrupt 0 and Interrupt 1)
#define MCUCSR								*((volatile u8 *)0x54)				// Microcontroller Unit Control and States Register (Used to set detection type for Interrupt 2)
#define GICR								*((volatile u8 *)0x5B)				// General Interrupt Control Register (Used to Enable and Disable the interrupts)
#define GIFR								*((volatile u8 *)0x5A)				// General Interrupt Flag Register	(Has Interrupt flags that are controlled by hardware when an interrupt finishes or by software)

#define EXTI_INTERRUPT_PIN0					0 
#define EXTI_INTERRUPT_PIN1					1
#define EXTI_INTERRUPT_PIN2					2

#define EXTI_LOW_LEVEL						10
#define EXTI_ANY_LOGICAL_CHANGE				20
#define EXTI_FALLING_EDGE					30
#define EXTI_RISING_EDGE					40

#define EXTI_INTERRUPT_MODE					111
#define EXTI_POLLING_MODE					222

ES enuSetIntSenseLevel(u8 copy_u8_interrupt_pin, u8 copy_u8_sense_level);
ES enuSetIntMode(u8 copy_u8_interrupt_pin, u8 copy_u8_mode);

#endif /* EXTI_PRIV_H_ */
/*
 * Timer0_Priv.h
 *
 * Created: 1/24/2024 8:34:23 PM
 *  Author: Beast
 */ 

#ifndef TIMER0_PRIV_H_
#define TIMER0_PRIV_H_

// Timer Registers
#define TCCR0		*((volatile u8*)0x53)	//Timer/Counter Control Register
#define TCNT0		*((volatile u8*)0x52)	//Timer/Counter Register (incremented by one every clock cycle)
#define OCR0		*((volatile u8*)0x5C)	//Output Compare Register (has a saved desired value that is constantly compared with tcnt0 register every clock cycle)
#define TIMSK		*((volatile u8*)0x59)	//Timer/Counter Interrupt Mask Register (Interrupt enablers)
#define TIFR		*((volatile u8*)0x58)	//Timer/Counter Interrupt Flag Register (flags)

// Timer Prescalers CSXX bits
#define NO_CLOCK_SOURECE			0b000
#define SYSTEM_FREQUENCY			0b001
#define TIMER0_PRESC_8				0b010
#define TIMER0_PRESC_64				0b011
#define TIMER0_PRESC_256			0b100
#define TIMER0_PRESC_1024			0b101
#define TIMER0_PRESC_FALLING		0b110
#define TIMER0_PRESC_RISING			0b111

// Timer Modes WGM-bits
#define TIMER0_OVF_MODE				0
#define TIMER0_CTC_MODE				1
#define TIMER0_FAST_PWM				2
#define TIMER0_PHASECORRECT_PWM		3

// Timer Compare Output Mode OC0-Pin
#define TIMER0_OC0_DISCONNECTED			10
#define TIMER0_OC0_TOGGLE				20
#define TIMER0_OC0_SET					30
#define TIMER0_OC0_CLEAR				40

// Timer PWM OC0-pin Modes
#define TIMER0_OC0_RESERVED				50
#define TIMER0_OC0_INVERTED				60
#define TIMER0_OC0_NON_INVERTED			70

// Timer Mode
#define TIMER0_POLLING_MODE				666
#define TIMER0_INTERRUPT_MODE			777

#define TIMER0_SYSTEM_FREQUENCY				16000000.0

#define TIMER0_TOP		256UL

static const f32 TIMER0_PRESCALERS[] = {1.0, 8.0, 64.0, 256.0, 1024.0};
 
static ES enu_Calculate_OVF_Mode(f32 copy_f32_prescaler, f32 copy_f32_time, f32 *copy_pf32_number_of_ovfs, u16 *copy_pu16_preload);
static ES enu_Calculate_CTC_Mode(f32 copy_f32_prescaler, f32 copy_f32_time, u8 *copy_pu8_OCR_value, u16 *copy_pu16_CTC_interrupts);
 
#endif /* TIMER0_PRIV_H_ */
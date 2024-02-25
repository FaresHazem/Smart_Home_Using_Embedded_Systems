/*
 * Timer2_Priv.h
 *
 * Created: 2/25/2024 4:19:17 PM
 *  Author: Beast
 */ 


#ifndef TIMER2_PRIV_H_
#define TIMER2_PRIV_H_

// Timer Registers
#define TCCR2		*((volatile u8*)0x45)	//Timer/Counter Control Register
#define TCNT2		*((volatile u8*)0x44)	//Timer/Counter Register (incremented by one every clock cycle)
#define OCR2		*((volatile u8*)0x43)	//Output Compare Register (has a saved desired value that is constantly compared with tcnt0 register every clock cycle)
#define TIMSK		*((volatile u8*)0x59)	//Timer/Counter Interrupt Mask Register (Interrupt enablers)
#define TIFR		*((volatile u8*)0x58)	//Timer/Counter Interrupt Flag Register (flags)

// Timer Prescalers CSXX bits
#define TIMER2_NO_CLOCK_SOURECE			0b000
#define TIMER2_SYSTEM_FREQUENCY			0b001
#define TIMER2_PRESC_8					0b010
#define TIMER2_PRESC_32					0b011
#define TIMER2_PRESC_64					0b100
#define TIMER2_PRESC_128				0b101
#define TIMER2_PRESC_256				0b110
#define TIMER2_PRESC_1024				0b111

// Timer Modes WGM-bits
#define TIMER2_OVF_MODE				0
#define TIMER2_CTC_MODE				1
#define TIMER2_FAST_PWM				2
#define TIMER2_PHASECORRECT_PWM		3

#define TIMER2_OC2_DISCONNECTED			10
#define TIMER2_OC2_TOGGLE				20
#define TIMER2_OC2_SET					30
#define TIMER2_OC2_CLEAR				40

// Timer PWM OC0-pin Modes
#define TIMER2_OC2_RESERVED				50
#define TIMER2_OC2_INVERTED				60
#define TIMER2_OC2_NON_INVERTED			70

// Timer Mode
#define TIMER2_POLLING_MODE				666
#define TIMER2_INTERRUPT_MODE			777

//#define TIMER2_SYSTEM_FREQUENCY				16000000.0

#define TIMER2_TOP		256UL

static const f32 TIMER2PRESCALERS[] = {1.0, 8.0, 32.0, 64.0, 128.0, 265.0, 1024.0};

#endif /* TIMER2_PRIV_H_ */
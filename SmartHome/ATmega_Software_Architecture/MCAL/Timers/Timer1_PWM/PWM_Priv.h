/*
 * PWM_Priv.h
 *
 * Created: 2/5/2024 6:52:32 PM
 *  Author: Beast
 */ 


#ifndef PWM_PRIV_H_
#define PWM_PRIV_H_

//PWM Registers
//Control Registers
#define TCCR1A		*((volatile u8*)0X4F)
#define TCCR1B		*((volatile u8*)0X4E)
//Counter Registers
#define TCNT1H		*((volatile u8*)0X4D)
#define TCNT1L		*((volatile u8*)0X4C)
//Compare Registers
#define OCR1AH		*((volatile u8*)0X4B)
#define OCR1AL		*((volatile u8*)0X4A)
#define OCR1BH		*((volatile u8*)0X49)
#define OCR1BL		*((volatile u8*)0X48)
//Input Capture Registers
#define ICR1H		*((volatile u8*)0X47)
#define ICR1L		*((volatile u8*)0X46)

//PWM Prescalers
#define PWM_NO_CLOCKSOURCE			0b000
#define PWM_SYSTEM_FREQ				0b001
#define PWM_8_PRESC					0b010
#define PWM_64_PRESC				0b011
#define PWM_256_PRESC				0b100
#define PWM_1024_PRESC				0b101
#define PWM_EXTERNAL_CLK_FALLING	0b110
#define PWM_EXTERNAL_CLK_RISING		0b111

//PWM Modes
//Generates PWM on OCR1A and OCR1B with constant Frequencies (Value of Top is Constant)
#define PWM_PHASE_CORRECT_8BIT_2X          0b0001    //Phase correct PWM, 8-bit, TOP is constant
#define PWM_PHASE_CORRECT_9BIT_2X          0b0010    //Phase correct PWM, 9-bit, TOP is constant
#define PWM_PHASE_CORRECT_10BIT_2X         0b0011    //Phase correct PWM, 10-bit, TOP is constant
#define PWM_FAST_8BIT_2X                   0b0101    //Fast PWM, 8-bit, TOP is constant
#define PWM_FAST_9BIT_2X                   0b0110    //Fast PWM, 9-bit, TOP is constant
#define PWM_FAST_10BIT_2X                  0b0111    //Fast PWM, 10-bit, TOP is constant

//Generates PWM on OCR1A and OCR1B with adjustable Frequencies (Value of Top can be modified)
#define PWM_PHASE_CORRECT_2X_BOTTOM_UPDATE 0b1000    //Phase correct PWM, 2X, TOP is ICR1, update at bottom
#define PWM_PHASE_CORRECT_1X_BOTTOM_UPDATE 0b1001    //Phase correct PWM, 1X, TOP is OCR1A, update at bottom
#define PWM_PHASE_CORRECT_2X_TOP_UPDATE    0b1010    //Phase correct PWM, 2X, TOP is ICR1, update at top
#define PWM_PHASE_CORRECT_1X_TOP_UPDATE    0b1011    //Phase correct PWM, 1X, TOP is OCR1A, update at top
#define PWM_FAST_2X_TOP_UPDATE             0b1110    //Fast PWM, 2X, TOP is ICR1, update at top
#define PWM_FAST_1X_TOP_UPDATE             0b1111    //Fast PWM, 1X, TOP is OCR1A, update at top

//Output Mode on OC1A and OC1B
//Non-Inverted: Clear OC1X-PIN on compare and Set on Overflow of counter
//Inverted: Set OC1X-PIN on compare and Clear on Overflow of counter
#define PWM_OC1A_NORMAL							00
#define PWM_OC1B_NORMAL							00
#define PWM_OC1A_INVERTED						11
#define PWM_OC1A_NON_INVERTED					10
#define PWM_OC1B_INVERTED						11
#define PWM_OC1B_NON_INVERTED					10

//Top values of TCNT1A and TCNT1B combined together
#define PWM_TOP_256								256
#define PWM_TOP_512								512
#define PWM_TOP_1024							1024

#define F_CPU 16000000UL

static const u16 PWM_PRESCALERS[] = {1, 8, 64, 256, 1024};

static ES enu_SetPwm(u8 copy_u8_channel, u8 copy_u8_dutycycle, u16 copy_u16_top);

//This function will be used to set the PWM duty cycle at given channel
//Channel: OC1A or OC1B
//DutyCycle: duty of PWM percentage of high in the cycle
//Top: Top value of TCNT1 where we compare below this value

#endif /* PWM_PRIV_H_ */
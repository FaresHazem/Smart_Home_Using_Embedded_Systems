/*
 * TIMER0_priv.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Omar Gamal El-Kady
 */

#ifndef MCAL_TIMERS_TIMER0_TIMER0_PRIV_H_
#define MCAL_TIMERS_TIMER0_TIMER0_PRIV_H_

// Timer 0 Register
#define TIMER0_TCCR0          *((volatile u8 *)0x53)
#define TIMER0_TCNT0          *((volatile u8 *)0x52)
#define TIMER0_OCR0           *((volatile u8 *)0x5C)

#define TIMER0_TIMSK           *((volatile u8 *)0x59)
#define TIMER0_TIFR            *((volatile u8 *)0x58)

#define TIMER0_WG_MODE_NORMAL           0
#define TIMER0_WG_MODE_PWM              1
#define TIMER0_WG_MODE_CTC              2
#define TIMER0_WG_MODE_FASTPWM          3

#define TIMER0_COM_MODE_NON_PWM_NORMAL  0
#define TIMER0_COM_MODE_NON_PWM_TOGGLE  1
#define TIMER0_COM_MODE_NON_PWM_CLEAR   2
#define TIMER0_COM_MODE_NON_PWM_SET     3

#define TIMER0_COM_MODE_PWM_NORMAL      0
#define TIMER0_COM_MODE_PWM_RESERVED    1
#define TIMER0_COM_MODE_PWM_CLEAR       2
#define TIMER0_COM_MODE_PWM_SET         3

#define TIMER0_COM_MODE_FASTPWM_NORMAL      0
#define TIMER0_COM_MODE_FASTPWM_RESERVED    1
#define TIMER0_COM_MODE_FASTPWM_CLEAR       2
#define TIMER0_COM_MODE_FASTPWM_SET         3

#define TIMER0_NO_CLK_SOURCE           0
#define TIMER0_PRESCALER_1             1
#define TIMER0_PRESCALER_8             2
#define TIMER0_PRESCALER_64            3
#define TIMER0_PRESCALER_256           4
#define TIMER0_PRESCALER_1024          5
#define TIMER0_EXT_CLK_FALLING         6
#define TIMER0_EXT_CLK_RISING          7

#define TIMER0_POLLING                 90
#define TIMER0_OVFINT                  91
#define TIMER0_COMINT                  92

static ES_t enuSetWaveGenMode(u8 Copy_u8WaveGenMode);
static ES_t enuSetPrescaler(u8 Copy_u8Prescaler);
static ES_t enuSetCompNonPWMMode(u8 Copy_u8COMNonPWMMode);
static ES_t enuSetInterruptMode(u8 Copy_u8InterruptMode);
static ES_t enuCalculateOVFNOCR(u16 Copy_u16DelayTime);

static u16 LOC_au16PrescalerValues[] = {1 , 8 , 64 , 256 , 1024};

#endif /* MCAL_TIMERS_TIMER0_TIMER0_PRIV_H_ */

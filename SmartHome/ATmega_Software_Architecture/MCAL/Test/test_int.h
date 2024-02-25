/*
 * TIMER0_int.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Omar Gamal El-Kady
 */

#ifndef MCAL_TIMERS_TIMER0_TIMER0_INT_H_
#define MCAL_TIMERS_TIMER0_TIMER0_INT_H_

ES_t TIEMR0_enuInit(void);

ES_t TIMER0_enuSetWaveGenMode(u8 Copy_u8WaveGenMode);

ES_t TIMER0_enuSetPrescaler(u8 Copy_u8Prescaler);

ES_t TIMER0_enuSetCompNonPWMMode(u8 Copy_u8COMNonPWMMode);

ES_t TIMER0_SetOCRValue(u8 Copy_u8Cycles);

ES_t TIMER0_enuGetTimerCounts(u8 * Copy_pu8Counts);

ES_t TIMER0_enuOVFInterruptEnable(void);

ES_t TIMER0_enuOVFInterruptDisable(void);

ES_t TIMER0_enuCOMInterruptEnable(void);

ES_t TIMER0_enuCOMInterruptDisable(void);

ES_t TIMER0_enuInterruptDisable(void);

ES_t TIMER0_enuSyncDelayms(u16 Copy_u16DelayTime);

ES_t TIMER0_enuAsyncDelayms(u16 Copy_u16DelayTime);

ES_t TIMER0_enuSetCallBack(void (* Copy_pfuncAppFunc)(void*),void * Copy_pvidParameter);

#endif /* MCAL_TIMERS_TIMER0_TIMER0_INT_H_ */

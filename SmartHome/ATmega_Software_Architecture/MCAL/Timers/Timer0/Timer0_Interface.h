/*
 * Timer0_Interface.h
 *
 * Created: 1/24/2024 8:34:41 PM
 *  Author: Beast
 */ 


#ifndef TIMER0_INTERFACE_H_
#define TIMER0_INTERFACE_H_

ES Timer0_enuInit(void);

ES Timer0_enuPolling_SetTimer(f32 copy_f32_time);

ES Timer0_enuInterrupt_SetTimer(f32 copy_f32_time);

ES Timer0_enuPolling_GeneratePWM(u8 copy_u8_dutycycle, u8 copy_u8_groupId, u8 copy_u8_pinId);               // this function will generate PWM on any pin at polling mode

ES Timer0_enuInterrupt_GeneratePWM(u8 copy_u8_dutycycle);           // pwm on OC0-pin

ES TIMER0_enuSetCallBack(void *(ptr_to_function)(void*), void * copy_pvid_parameter);

ES Timer0_enuCounter(u8 copy_u8_counterPin);


#endif /* TIMER0_INTERFACE_H_ */
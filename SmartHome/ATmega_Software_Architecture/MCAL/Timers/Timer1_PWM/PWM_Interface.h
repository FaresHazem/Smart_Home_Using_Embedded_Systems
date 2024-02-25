/*
 * PWM_Interface.h
 *
 * Created: 2/5/2024 6:52:02 PM
 *  Author: Beast
 */ 


#ifndef PWM_INTERFACE_H_
#define PWM_INTERFACE_H_

ES PWM_enuInit();

ES PWM_enuGeneratePWM_FixedFreq(u8 copy_u8_dutycycle_A, u8 copy_u8_dutycycle_B);  //generates PWM on both channels

ES PWM_enuGeneratePWM_AdjustableFreq_1X(u8 copy_u8_dutycycle, u8 copy_u8_frequency);  //generates PWM on channel B with adjustable frequency TOP-->OCR1A

ES PWM_enuGeneratePWM_AdjustableFreq_2X(u8 copy_u8_dutycycle_A, u8 copy_u8_dutycycle_B, u8 copy_u8_frequency);  //generates PWM on both channels with adjustable frequency TOP-->ICR1

ES PWM_enuStopPWM();
#endif /* PWM_INTERFACE_H_ */
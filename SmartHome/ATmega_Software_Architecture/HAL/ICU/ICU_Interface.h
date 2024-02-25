/*
 * ICU_Interface.h
 *
 * Created: 2/17/2024 11:51:21 PM
 *  Author: Beast
 */ 


#ifndef ICU_INTERFACE_H_
#define ICU_INTERFACE_H_

ES ICU_enuInit();													//Initialize ICU

ES ICU_enuGetDutyCycle(u8 *copy_pu8_dutycycle);                     //To get the duty of the input PWM

ES ICU_enuGetFrequency(u16 *copy_pu16_frequency);					//To get the frequency of the input PWM

#endif /* ICU_INTERFACE_H_ */
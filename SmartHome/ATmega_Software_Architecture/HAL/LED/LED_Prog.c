/*
 * LED_Prog.c
 *
 * Created: 9/25/2023 7:11:43 PM
 *  Author: Beast
 */ 

#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#include "../../MCAL/DIO/DIO_INT.h"

#include "LED_Config.h"
#include "LED_Priv.h"

extern u8 local_u8_led_max;
extern LED local_astr_led_config[];                      // getting the array of the led configuration to use it in our program

ES LED_enuInit(){
	
	ES local_enu_error_state = ES_NOK;
	u8 local_u8_counter;
	
	for(local_u8_counter = 0; local_u8_counter < local_u8_led_max; local_u8_counter++){
		local_enu_error_state = DIO_enuSetPinMode(local_astr_led_config[local_u8_counter].led_grp, local_astr_led_config[local_u8_counter].led_pin, DIO_OUTPUT);
		if(local_enu_error_state != ES_OK){
			break;
		}
	}
	return local_enu_error_state;
}

ES LED_enuON(u8 copy_u8_led_id){
	
	ES local_enu_error_state = ES_NOK;
	if(copy_u8_led_id < local_u8_led_max){
		
		local_enu_error_state =  DIO_enuSetPinValue(local_astr_led_config[copy_u8_led_id].led_grp, local_astr_led_config[copy_u8_led_id].led_pin, DIO_HIGH);
		
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	return local_enu_error_state;
}

ES LED_enuOFF(u8 copy_u8_led_id){
	
	ES local_enu_error_state = ES_NOK;
	if(copy_u8_led_id < local_u8_led_max){
		
		local_enu_error_state =  DIO_enuSetPinValue(local_astr_led_config[copy_u8_led_id].led_grp, local_astr_led_config[copy_u8_led_id].led_pin, DIO_LOW);
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	return local_enu_error_state;
}

ES LED_enuToggle(u8 copy_u8_led_id){
	
	ES local_enu_error_state = ES_NOK;
	if(copy_u8_led_id < local_u8_led_max){
		
		local_enu_error_state =  DIO_enuToggleBit(local_astr_led_config[copy_u8_led_id].led_grp, local_astr_led_config[copy_u8_led_id].led_pin);
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	return local_enu_error_state;
	
}
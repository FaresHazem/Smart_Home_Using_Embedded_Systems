/*
 * Switch_Porg.c
 *
 * Created: 9/24/2023 4:57:51 PM
 *  Author: Beast
 */ 

// Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

// Lower Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"

// Switch Private Libraries
#include "Switch_Config.h"
#include "Switch_Priv.h"

extern u8 local_u8_switch_max;
extern Switch Switch_astr_switch_config[];

ES Switch_enu_Init(){
	
	ES local_enu_error_state = ES_NOK;
		
		for(int i = 0; i < local_u8_switch_max; i++){
			DIO_enuSetPinMode(Switch_astr_switch_config[i].sw_grp, Switch_astr_switch_config[i].sw_pin, DIO_INPUT);
			DIO_enuSetPinValue(Switch_astr_switch_config[i].sw_grp, Switch_astr_switch_config[i].sw_pin, Switch_astr_switch_config[i].sw_state);
		}
		
	return local_enu_error_state;
}

ES Switch_enu_GetSwitchState(u8 copy_u8_switch_id, u8 *copy_pbool_pressed){
	
	ES local_enu_error_state = ES_NOK;
	
	// Checks
	if(copy_pbool_pressed == NULL){
		local_enu_error_state = ES_NULL_POINTER;
	}else{

		if(copy_u8_switch_id < local_u8_switch_max){
			
			// Body
			local_enu_error_state = DIO_enuReadBit(Switch_astr_switch_config[copy_u8_switch_id].sw_grp, Switch_astr_switch_config[copy_u8_switch_id].sw_pin, copy_pbool_pressed);
			if(local_enu_error_state != ES_OK){
				return local_enu_error_state;
			}
			
			local_enu_error_state = ES_OK;
		}else{
			local_enu_error_state = ES_OUT_OF_RANGE;
		}
	}
	return local_enu_error_state;
}
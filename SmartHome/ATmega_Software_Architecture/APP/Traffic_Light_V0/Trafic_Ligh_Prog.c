/*
 * Trafic_Ligh_Prog.c
 *
 * Created: 9/26/2023 3:41:34 PM
 *  Author: Beast
 */ 
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#define F_CPU 16000000
#include <util/delay.h>

#include "../../MCAL/DIO/DIO_INT.h"
#include "../../HAL/HexDecoder/HexDec_Interface.h"
#include "../../HAL/LED/LED_Interface.h"
#include "../../HAL/Switch/Switch_INT.h"

#include "Traffic_Light_Priv.h"

u8 TRAFFIC_COUNTER = 15;

u8 local_u8_button_state1;
u8 local_u8_button_state2;
boolean local_bool_flag1 = False;
boolean local_bool_flag2 = False;


ES TraficLight_enu_Init(){
	
	ES local_Aenu_error_state[3];
	ES local_enu_error_state = ES_NOK;
	
	local_Aenu_error_state[0] = HexDec_enu_Init();
	local_Aenu_error_state[1] = Switch_enu_Init();
	local_Aenu_error_state[2] = LED_enuInit();
	
	for(u8 i = 0; i < 3; i++){
		if(local_Aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_Aenu_error_state[i];
			break;
		}
	}	
	return local_enu_error_state;
}

void POV(u8 number){
	
	if(number < 100){
		HexDec_enu_DisableSegment(Segment_1);
		HexDec_enu_DisableSegment(Segment_2);
		
		HexDec_enu_Display(number % 10);
		HexDec_enu_EnableSegment(Segment_1);
		_delay_ms(10);
		HexDec_enu_DisableSegment(Segment_1);
		
		HexDec_enu_Display(number / 10);
		HexDec_enu_EnableSegment(Segment_2);
		_delay_ms(10);
		HexDec_enu_DisableSegment(Segment_2);
	}
}

ES officer(u8 *copy_pu8_number){
	
	ES local_enu_error_state = ES_NOK;
	
	if(copy_pu8_number == NULL){
		local_enu_error_state = ES_NULL_POINTER;
	}else{
	
		if(TRAFFIC_COUNTER > 0 && TRAFFIC_COUNTER < 100){
			
			Switch_enu_GetSwitchState(SWITCH_BUTTON_0, &local_u8_button_state1);
			if(local_u8_button_state1 == DIO_HIGH && local_bool_flag1 == False){
				(*copy_pu8_number)++;
				local_bool_flag1 = True;
			}else if(local_u8_button_state1 == DIO_LOW && local_bool_flag1 == True){
				local_bool_flag1 = False;
			}else{}
				
			Switch_enu_GetSwitchState(SWITCH_BUTTON_1, &local_u8_button_state2);
			if(local_u8_button_state2 == DIO_HIGH && local_bool_flag2 == False){
				(*copy_pu8_number)--;
				local_bool_flag2 = True;
			}else if(local_u8_button_state2 == DIO_LOW && local_bool_flag2 == True){
				local_bool_flag2 = False;
			}else{}
					
		}else{
			local_enu_error_state = ES_OUT_OF_RANGE;
		}
	}
	return local_enu_error_state;
}

void TraficLight_enu_ON(){
	
	u8 local_u8_number = 0;
	u8 local_u8_saver = TRAFFIC_COUNTER;             // To not lose the actual value of the counter
	
	for(TRAFFIC_COUNTER; TRAFFIC_COUNTER > 0; TRAFFIC_COUNTER--){
		if(TRAFFIC_COUNTER > 3){
			LED_enuON(LED_0);                                                     // Red
			for(u8 wait = 0; wait < 50; wait++){
				POV(TRAFFIC_COUNTER);
				officer(&local_u8_number);
			}
		}else{
			LED_enuOFF(LED_0);
			LED_enuON(LED_1);                                                     // Yellow in last 3 seconds of red
			for(u8 wait = 0; wait < 50; wait++){
				POV(TRAFFIC_COUNTER);
				officer(&local_u8_number);
			}
		}
	}
	LED_enuOFF(LED_1);
	// TO RESET THE COUNTER TO ITS FORM
	TRAFFIC_COUNTER = local_u8_saver;
	
	for(TRAFFIC_COUNTER; TRAFFIC_COUNTER > 0; TRAFFIC_COUNTER--){
		LED_enuON(LED_2);                                                     // Green
		for(u8 wait = 0; wait < 50; wait++){
			POV(TRAFFIC_COUNTER);
			officer(&local_u8_number);
		}
	}
	LED_enuOFF(LED_2);
	// TO UPDATE THE COUNTER AFTER FULL SUCCESSFULL TRAFIC RED YELLOW AND GREEN
	TRAFFIC_COUNTER = local_u8_saver;
	TRAFFIC_COUNTER += local_u8_number;
}
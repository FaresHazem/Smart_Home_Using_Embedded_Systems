/*
 * TrafficLight_V1_Porg.c
 *
 * Created: 12/3/2023 9:08:22 PM
 *  Author: Beast
 */ 

#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#define F_CPU 16000000
#include <util/delay.h>

#include "../../MCAL/DIO/DIO_INT.h"
#include "../../MCAL/EXTI/EXTI_Interface.h"
#include "../../HAL/HexDecoder/HexDec_Interface.h"
#include "../../HAL/LED/LED_Interface.h"
#include "../../HAL/Switch/Switch_INT.h"

u8 TRAFFIC_COUNTER_V1 = 15;
u8 Incremented = 0;

ES enu_increment_counter(){
	
	Incremented++;
	
	return ES_OK;
}

ES TraficLightV1_enuInit(){
	
	ES local_Aenu_error_state[5];
	ES local_enu_error_state = ES_NOK;
	
	local_Aenu_error_state[0] = HexDec_enu_Init();
	local_Aenu_error_state[1] = Switch_enu_Init();
	local_Aenu_error_state[2] = LED_enuInit();
	local_Aenu_error_state[2] = EXTI_enuInit();
	
	EXTI_enuCallBack(EXTI_INTERRUPT_PIN0, enu_increment_counter);
	
	for(u8 i = 0; i < 3; i++){
		if(local_Aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_Aenu_error_state[i];
			break;
		}
	}
	return local_enu_error_state;
}


ES enu_POV(u8 number){
	
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
	return ES_OK;
}


ES TraficLightV1_enuStart(){
			
	//u8 local_u8_number = 0;
	u8 local_u8_saver = TRAFFIC_COUNTER_V1;             // To not lose the actual value of the counter
			
	for(TRAFFIC_COUNTER_V1; TRAFFIC_COUNTER_V1 > 0; TRAFFIC_COUNTER_V1--){
		if(TRAFFIC_COUNTER_V1 > 3){
			LED_enuON(LED_0);                                                     // Red
			for(u8 wait = 0; wait < 50; wait++){
				enu_POV(TRAFFIC_COUNTER_V1);
				//officer(&local_u8_number);
			}
		}else{
			LED_enuOFF(LED_0);
			LED_enuON(LED_1);                                                // Yellow in last 3 seconds of red
			for(u8 wait = 0; wait < 50; wait++){
				enu_POV(TRAFFIC_COUNTER_V1);
				//officer(&local_u8_number);
			}
		}
	}
	LED_enuOFF(LED_1);
	// TO RESET THE COUNTER TO ITS FORM
	TRAFFIC_COUNTER_V1 = local_u8_saver;
			
	for(TRAFFIC_COUNTER_V1; TRAFFIC_COUNTER_V1 > 0; TRAFFIC_COUNTER_V1--){
		LED_enuON(LED_2);                                                     // Green
		for(u8 wait = 0; wait < 50; wait++){
			enu_POV(TRAFFIC_COUNTER_V1);
			//officer(&local_u8_number);
		}
	}
	LED_enuOFF(LED_2);
	// TO UPDATE THE COUNTER AFTER FULL SUCCESSFULL TRAFIC RED YELLOW AND GREEN
	TRAFFIC_COUNTER_V1 = local_u8_saver;
	if(Incremented != 0){
		TRAFFIC_COUNTER_V1 += Incremented;
		Incremented = 0;
	}
	return ES_OK;
}
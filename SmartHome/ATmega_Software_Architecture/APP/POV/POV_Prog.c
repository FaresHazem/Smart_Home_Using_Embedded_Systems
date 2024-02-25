/*
 * POV_Prog.c
 *
 * Created: 11/27/2023 10:26:44 PM
 *  Author: Beast
 */ 

// Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"
//Delay
#define F_CPU 16000000
#include <util/delay.h>

// Lower Level Libraries
#include "../../HAL/HexDecoder/HexDec_Interface.h"

// Private Libraries


ES POV_enuInit(){
	
	return HexDec_enu_Init();
	
}

ES POV_enuDisplayNum(u8 copy_u8_number){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_u8_number >= 0 && copy_u8_number < 100){
		
		for(u8 i = 0; i < copy_u8_number; i++){
			
			for(u8 wait = 0; wait < 20; wait++){
				HexDec_enu_DisableSegment(Segment_1);
				HexDec_enu_DisableSegment(Segment_2);
				
				HexDec_enu_Display(i % 10);
				HexDec_enu_EnableSegment(Segment_2);
				_delay_ms(10);
				HexDec_enu_DisableSegment(Segment_2);

				HexDec_enu_Display(i / 10);
				HexDec_enu_EnableSegment(Segment_1);
				_delay_ms(10);
				HexDec_enu_DisableSegment(Segment_1);
			}
			
		}
		
		local_enu_errorstate = ES_OK;
	}else{
		local_enu_errorstate = ES_NOK;
	}
	
	return local_enu_errorstate;
}

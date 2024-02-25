/*
 * HexDec_Prog.c
 *
 * Created: 9/23/2023 9:45:32 PM
 *  Author: Beast
 */ 


// Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

// Lower Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"

// HexDecoder Private Libraries
#include "HexDec_Priv.h"
#include "HexDec_Config.h"

ES HexDec_enu_Init(void){
	
	ES local_enu_error_state = ES_NOK;
	
	// Checking of all pins and groups are in range
	#if HexDec_PIN_A <= DIO_PIN_7 && HexDec_PIN_B <= DIO_PIN_7 && HexDec_PIN_C <= DIO_PIN_7 && HexDec_PIN_D <= DIO_PIN_7 && HexDec_EN1_PIN <= DIO_PIN_7 && HexDec_EN2_PIN <= DIO_PIN_7
		#if HexDec_A_GRP <= DIO_GROUP_D && HexDec_B_GRP <= DIO_GROUP_D && HexDec_C_GRP <= DIO_GROUP_D && HexDec_D_GRP <= DIO_GROUP_D && HexDec_EN1_GRP <= DIO_GROUP_D && HexDec_EN2_GRP <= DIO_GROUP_D
		
			ES local_aenu[6] = {0};
			// Initializing pin modes
			local_aenu[0] = DIO_enuSetPinMode(HexDec_A_GRP, HexDec_PIN_A, DIO_OUTPUT);
			local_aenu[1] = DIO_enuSetPinMode(HexDec_B_GRP, HexDec_PIN_B, DIO_OUTPUT);
			local_aenu[2] = DIO_enuSetPinMode(HexDec_C_GRP, HexDec_PIN_C, DIO_OUTPUT);
			local_aenu[3] = DIO_enuSetPinMode(HexDec_D_GRP, HexDec_PIN_D, DIO_OUTPUT);
			
			local_aenu[4] = DIO_enuSetPinMode(HexDec_EN1_GRP, HexDec_EN1_PIN, DIO_OUTPUT);
			local_aenu[5] = DIO_enuSetPinMode(HexDec_EN2_GRP, HexDec_EN2_PIN, DIO_OUTPUT);
			
			// Checking if all initializations are passed
			for(u8 i = 0; i < 6; i++){
				if(local_aenu[i] != ES_OK){
					local_enu_error_state = local_aenu[i];
					break;
				}
			}
			
		#else
			#error "Hex Group Config Out Of Range"
			#endif
	#else
		#error "Hex Pin Config Out Of Range"
		#endif
	
	return local_enu_error_state;
	
}

ES HexDec_enu_Display(u8 copy_u8_number){
	
	ES local_enu_error_state = ES_NOK;
	
	if(copy_u8_number <= SegmentLimit){
		
		// Masking
		DIO_enuSetPinValue(HexDec_A_GRP, HexDec_PIN_A, DIO_LOW);
		DIO_enuSetPinValue(HexDec_B_GRP, HexDec_PIN_B, DIO_LOW);
		DIO_enuSetPinValue(HexDec_C_GRP, HexDec_PIN_C, DIO_LOW);
		DIO_enuSetPinValue(HexDec_D_GRP, HexDec_PIN_D, DIO_LOW);
		
		// Initialization
		DIO_enuSetPinValue(HexDec_A_GRP, HexDec_PIN_A, GetBit(copy_u8_number, 0));
		DIO_enuSetPinValue(HexDec_B_GRP, HexDec_PIN_B, GetBit(copy_u8_number, 1));
		DIO_enuSetPinValue(HexDec_C_GRP, HexDec_PIN_C, GetBit(copy_u8_number, 2));
		DIO_enuSetPinValue(HexDec_D_GRP, HexDec_PIN_D, GetBit(copy_u8_number, 3));
		
		local_enu_error_state = ES_OK;
		
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	
	return local_enu_error_state;
}

ES HexDec_enu_EnableSegment(u8 copy_u8_segment_id){

	ES local_enu_error_state = ES_NOK;
	
	if(copy_u8_segment_id <= Segment_2){
		
		switch(copy_u8_segment_id){
			case Segment_1:                        // 0
				DIO_enuSetPinValue(HexDec_EN1_GRP, HexDec_EN1_PIN, DIO_HIGH);
				break;
			case Segment_2:                        // 1
				DIO_enuSetPinValue(HexDec_EN2_GRP, HexDec_EN2_PIN, DIO_HIGH);
				break;
		}
		local_enu_error_state = ES_OK;
		
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	
	return local_enu_error_state;
}

ES HexDec_enu_DisableSegment(u8 copy_u8_segment_id){

	ES local_enu_error_state = ES_NOK;
	
	if(copy_u8_segment_id <= Segment_2){
		
		switch(copy_u8_segment_id){
			case Segment_1:                        // 0
				DIO_enuSetPinValue(HexDec_EN1_GRP, HexDec_EN1_PIN, DIO_LOW);
				break;
			case Segment_2:                        // 1
				DIO_enuSetPinValue(HexDec_EN2_GRP, HexDec_EN2_PIN, DIO_LOW);
				break;
		}
		local_enu_error_state = ES_OK;
		
		}else{
			local_enu_error_state = ES_OUT_OF_RANGE;
	}
	
	return local_enu_error_state;
}

ES HexDec_enu_StopDisplay(){
	HexDec_enu_DisableSegment(Segment_1);
	HexDec_enu_DisableSegment(Segment_2);
	
	return ES_OK;
}
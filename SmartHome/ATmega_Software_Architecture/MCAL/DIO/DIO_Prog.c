/*
 * DIO_Prog.c
 *
 * Created: 9/22/2023 6:34:49 PM
 *  Author: Beast
 */ 

//------------------------------------------------------------------------
//Public Libraries
#include "stdTypes.h"
#include "BitMath.h"
#include "ErrorState.h"

//Lower Layers Libraries
//This is the lowest file so there is no inclusion

//Private Libraries
#include "DIO_Config.h"
#include "DIO_Priv.h"
//------------------------------------------------------------------------



ES DIO_enuInit(void);															

ES DIO_enuSetPinMode(u8 copy_u8_groupID, u8 copy_u8_bitNo, u8 copy_u8_bitMode){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_u8_groupID <= DIO_GROUP_D && copy_u8_bitNo <= DIO_PIN_7 && copy_u8_bitMode <= DIO_OUTPUT){
		
		switch(copy_u8_groupID){
			
			case DIO_GROUP_A:               
				InitializeBit(DDRA_, copy_u8_bitNo, copy_u8_bitMode);
			break;
			
			case DIO_GROUP_B:
				InitializeBit(DDRB_, copy_u8_bitNo, copy_u8_bitMode);
			break;
			
			case DIO_GROUP_C:
				InitializeBit(DDRC_, copy_u8_bitNo, copy_u8_bitMode);
			break;
			
			case DIO_GROUP_D:
				InitializeBit(DDRD_, copy_u8_bitNo, copy_u8_bitMode);
			break;
			
		}
		local_enu_errorstate = ES_OK;
		
	}else{
		local_enu_errorstate = ES_OUT_OF_RANGE;
	}
	
	return local_enu_errorstate;
}

ES DIO_enuSetPinValue(u8 copy_u8_groupID, u8 copy_u8_bitNo, u8 copy_u8_bitvalue){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_u8_groupID <= DIO_GROUP_D && copy_u8_bitNo <= DIO_PIN_7 && copy_u8_bitvalue <= DIO_HIGH){
		
		switch(copy_u8_groupID){
			
			case DIO_GROUP_A:
				InitializeBit(PORTA_, copy_u8_bitNo, copy_u8_bitvalue);
			break;
			
			case DIO_GROUP_B:
				InitializeBit(PORTB_, copy_u8_bitNo, copy_u8_bitvalue);
			break;
			
			case DIO_GROUP_C:
				InitializeBit(PORTC_, copy_u8_bitNo, copy_u8_bitvalue);
			break;
			
			case DIO_GROUP_D:
				InitializeBit(PORTD_, copy_u8_bitNo, copy_u8_bitvalue);
			break;
		}
		local_enu_errorstate = ES_OK;
	}else{
		local_enu_errorstate = ES_OUT_OF_RANGE;
	}
	
	return local_enu_errorstate;
}

ES DIO_enuToggleBit(u8 copy_u8_groupID, u8 copy_u8_bitNo){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_u8_groupID <= DIO_GROUP_D && copy_u8_bitNo <= DIO_PIN_7){
		
		switch(copy_u8_groupID){
			
			case DIO_GROUP_A:
				ToggleBit(PORTA_, copy_u8_bitNo);
			break;
			case DIO_GROUP_B:
				ToggleBit(PORTB_, copy_u8_bitNo);
			break;
			case DIO_GROUP_C:
				ToggleBit(PORTC_, copy_u8_bitNo);
			break;
			case DIO_GROUP_D:
				ToggleBit(PORTD_, copy_u8_bitNo);
			break;	
		}
		local_enu_errorstate = ES_OK;
	}else{
		local_enu_errorstate = ES_OUT_OF_RANGE;
	}
	
	return local_enu_errorstate;
	
}                

ES DIO_enuGetBit(u8 copy_u8_groupID, u8 copy_u8_bitNo, u8 * copy_pu8_value){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_pu8_value != NULL){
		
		if(copy_u8_groupID <= DIO_GROUP_D && copy_u8_bitNo <= DIO_PIN_7){
			
			switch(copy_u8_groupID){
				
				case DIO_GROUP_A:
				*copy_pu8_value = GetBit(PORTA_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_B:
				*copy_pu8_value = GetBit(PORTB_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_C:
				*copy_pu8_value = GetBit(PORTC_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_D:
				*copy_pu8_value = GetBit(PORTD_, copy_u8_bitNo);
				break;
				
			}
			local_enu_errorstate = ES_OK;
		}else{
			local_enu_errorstate = ES_OUT_OF_RANGE;
		}
	}else{
		local_enu_errorstate = ES_NULL_POINTER;
	}
	
	return local_enu_errorstate;
	
}

ES DIO_enuReadBit(u8 copy_u8_groupID, u8 copy_u8_bitNo, u8 * copy_pu8_value){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_pu8_value != NULL){
		
		if(copy_u8_groupID <= DIO_GROUP_D && copy_u8_bitNo <= DIO_PIN_7){
			
			switch(copy_u8_groupID){
				
				case DIO_GROUP_A:
				*copy_pu8_value = GetBit(PINA_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_B:
				*copy_pu8_value = GetBit(PINB_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_C:
				*copy_pu8_value = GetBit(PINC_, copy_u8_bitNo);
				break;
				
				case DIO_GROUP_D:
				*copy_pu8_value = GetBit(PIND_, copy_u8_bitNo);
				break;
				
			}
			local_enu_errorstate = ES_OK;
			}else{
			local_enu_errorstate = ES_OUT_OF_RANGE;
		}
		}else{
		local_enu_errorstate = ES_NULL_POINTER;
	}
	
	return local_enu_errorstate;
	
}


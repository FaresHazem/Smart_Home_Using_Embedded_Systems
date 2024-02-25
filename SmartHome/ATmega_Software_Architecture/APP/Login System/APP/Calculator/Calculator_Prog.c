/*
 * Calculator_Prog.c
 *
 * Created: 10/3/2023 8:57:29 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

//Application Libraries
#include "../../HAL/KeyPad/KeyPad_Interface.h"
#include "../../HAL/LCD/LCD_Init.h"


ES Calc_enuInit(){

	ES local_enu_errorstate = ES_NOK;
	
	local_enu_errorstate = LCD_enuInit();
	local_enu_errorstate = KEYPAD_enuInit();	
	
	return local_enu_errorstate;
}

ES enu_Calculate(u8 copy_u8_operation, s32 copy_s32_firstN, s32 copy_s32_secondN, s32 *copy_ps32_result){
	
	if(copy_u8_operation == '+'){
		*copy_ps32_result = copy_s32_firstN + copy_s32_secondN;
	}else if(copy_u8_operation == '-'){
		*copy_ps32_result = copy_s32_firstN - copy_s32_secondN;
	}else if(copy_u8_operation == '*'){
		*copy_ps32_result = copy_s32_firstN * copy_s32_secondN;
	}else if(copy_u8_operation == '/'){
		*copy_ps32_result = copy_s32_firstN / copy_s32_secondN;
	}else{
		// do nothing
	}
	
	return ES_OK;
}

ES Calc_enuStart(){

	u8 local_u8_value;
	s32 local_u8_first = 0;
	u8 local_u8_operation;
	s32 local_u8_second = 0;
	s32 local_u8_result = 0;
	
	u8 local_u8_numberSize1 = 0;
	u8 local_u8_numberSize2 = 0;
	
	boolean local_bool_reading = True;
	boolean local_bool_firstDone = False;                    // 1st number is not done
	boolean local_bool_secondDone = False;				     // 2nd number is not done
	boolean local_bool_operation = False;

	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		
		if(local_u8_value >= '0' && local_u8_value <= '9' && local_u8_value != KEYPAD_NOT_PRESSED){
			
			if(local_bool_firstDone == False){
				LCD_enuSendData(local_u8_value);
				// Convert ASCII character to numeric value
				s32 numeric_value = local_u8_value - '0';
			
 				local_u8_first *= 10;                         // if 1 --> 10 so the other number is added 3 --> 13
 				local_u8_first += numeric_value;
 				local_u8_numberSize1++;
 				if(local_u8_numberSize1 == 5){
					local_bool_firstDone = True;
 				}	 
			}else{
				if(local_bool_operation == True && local_bool_secondDone == False){
					LCD_enuSendData(local_u8_value);
					// Convert ASCII character to numeric value
					s32 numeric_value = local_u8_value - '0';
					
					local_u8_second *= 10;                         // if 1 --> 10 so the other number is added 3 --> 13
					local_u8_second += numeric_value;
					local_u8_numberSize2++;
					if(local_u8_numberSize2 == 5){
						local_bool_secondDone = True;
					}
				}
			}
		}
		
		if((local_u8_value == '+' || local_u8_value == '*' || local_u8_value == '-' || local_u8_value == '/') && (local_bool_operation == False)){
			local_u8_operation = local_u8_value;
			local_bool_firstDone = True;
			local_bool_operation = True;
			LCD_enuSendData(local_u8_value);
		}
		if(local_u8_value == '=' && local_bool_firstDone == True && local_bool_operation == True){
			local_bool_secondDone = True;  // if not true set it to true
			local_bool_reading = False;     // stop reading
		}
	}
	// Get the result
	enu_Calculate(local_u8_operation, local_u8_first, local_u8_second, &local_u8_result);
	// print result
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("Result: ");
	LCD_enuDisplayIntNumbers(local_u8_result);
	boolean local_bool_again = True;
	
	while(local_bool_again){  // reset
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value == 'r'){
			LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
			LCD_enuSetCursor(ROW1, COL1);	
			local_bool_again = False;
		}else{
			// do nothing
		}
	}
	return ES_OK;
}
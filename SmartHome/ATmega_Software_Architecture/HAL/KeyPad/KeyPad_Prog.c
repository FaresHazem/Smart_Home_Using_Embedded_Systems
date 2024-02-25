/*
 * KeyPad_Prog.c
 *
 * Created: 10/3/2023 7:07:59 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#define F_CPU 16000000
#include <util/delay.h>

//Lower Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"

//KeyPad Private Libraries
#include "KeyPad_Priv.h"
#include "KeyPad_Config.h"

ES KEYPAD_enuInit(){
	
	ES local_enu_errorstate = ES_NOK;	
	ES local_aenu_errorstate[16];
	
	#if (KEYPAD_ROW1_GP <= DIO_GROUP_D && KEYPAD_ROW2_GP <= DIO_GROUP_D && KEYPAD_ROW3_GP <= DIO_GROUP_D && KEYPAD_ROW4_GP <= DIO_GROUP_D && KEYPAD_COL1_GP <= DIO_GROUP_D && KEYPAD_COL2_GP <= DIO_GROUP_D && KEYPAD_COL3_GP <= DIO_GROUP_D && KEYPAD_COL4_GP <= DIO_GROUP_D)
		#if (KEYPAD_ROW1_PIN <= DIO_PIN_7 && KEYPAD_ROW2_PIN <= DIO_PIN_7 && KEYPAD_ROW3_PIN <= DIO_PIN_7 && KEYPAD_ROW4_PIN <= DIO_PIN_7 && KEYPAD_COL1_PIN <= DIO_PIN_7 && KEYPAD_COL2_PIN <= DIO_PIN_7 && KEYPAD_COL3_PIN <= DIO_PIN_7 && KEYPAD_COL4_PIN <= DIO_PIN_7)
		
			//Set all columns to output mode (Main Signal)
			local_aenu_errorstate[0] = DIO_enuSetPinMode(KEYPAD_COL1_GP, KEYPAD_COL1_PIN, DIO_OUTPUT);
			local_aenu_errorstate[1] = DIO_enuSetPinMode(KEYPAD_COL2_GP, KEYPAD_COL2_PIN, DIO_OUTPUT);
			local_aenu_errorstate[2] = DIO_enuSetPinMode(KEYPAD_COL3_GP, KEYPAD_COL3_PIN, DIO_OUTPUT);
			local_aenu_errorstate[3] = DIO_enuSetPinMode(KEYPAD_COL4_GP, KEYPAD_COL4_PIN, DIO_OUTPUT);
	
			//Set all rows to input mode so we can check for button signal at a row and a column
			local_aenu_errorstate[4] = DIO_enuSetPinMode(KEYPAD_ROW1_GP, KEYPAD_ROW1_PIN, DIO_INPUT);
			local_aenu_errorstate[5] = DIO_enuSetPinMode(KEYPAD_ROW2_GP, KEYPAD_ROW2_PIN, DIO_INPUT);
			local_aenu_errorstate[6] = DIO_enuSetPinMode(KEYPAD_ROW3_GP, KEYPAD_ROW3_PIN, DIO_INPUT);
			local_aenu_errorstate[7] = DIO_enuSetPinMode(KEYPAD_ROW4_GP, KEYPAD_ROW4_PIN, DIO_INPUT);
	
			//At the beginning of the initialization all rows will have Pull Up values so when a signal is applied it changes its row value to 0 so we can know that at this row there is a button pressed
			local_aenu_errorstate[8] = DIO_enuSetPinValue(KEYPAD_ROW1_GP, KEYPAD_ROW1_PIN, DIO_PULL_UP);
			local_aenu_errorstate[9] = DIO_enuSetPinValue(KEYPAD_ROW2_GP, KEYPAD_ROW2_PIN, DIO_PULL_UP);
			local_aenu_errorstate[10] = DIO_enuSetPinValue(KEYPAD_ROW3_GP, KEYPAD_ROW3_PIN, DIO_PULL_UP);
			local_aenu_errorstate[11] = DIO_enuSetPinValue(KEYPAD_ROW4_GP, KEYPAD_ROW4_PIN, DIO_PULL_UP);
	
			//Also we set all the column values to high signal, so we can loop through each one of them changing the signal to low so if at this column a button is pressed it will operate since there is a signal
			local_aenu_errorstate[12] = DIO_enuSetPinValue(KEYPAD_COL1_GP, KEYPAD_COL1_PIN, DIO_HIGH);
			local_aenu_errorstate[13] = DIO_enuSetPinValue(KEYPAD_COL2_GP, KEYPAD_COL2_PIN, DIO_HIGH);
			local_aenu_errorstate[14] = DIO_enuSetPinValue(KEYPAD_COL3_GP, KEYPAD_COL3_PIN, DIO_HIGH);
			local_aenu_errorstate[15] = DIO_enuSetPinValue(KEYPAD_COL4_GP, KEYPAD_COL4_PIN, DIO_HIGH);
			
		#else
			#error "Wrong Keypad Group Configuration"
			#endif
	#else
		#error "Wrong Keypad Pin Configuration"
		#endif
		
	for(u8 i = 0; i < 16; i++){
		local_enu_errorstate = local_aenu_errorstate[i];
		if(local_enu_errorstate != ES_OK){
			break;
		}
	}
	
	return local_enu_errorstate;
}

ES KEYPAD_enuRead(u8 *copy_u8_value){
	
	ES local_enu_errorstate = ES_NOK;
	ES local_aenu_errorstate[4];
	
	*copy_u8_value = KEYPAD_NOT_PRESSED;
	
	u8 local_u8_buttonstate;
	
	boolean local_bool_pressed = False;
	
	u8 local_au8_rows[][4] = KEYPAD_ROWS;            // all keypad rows
	u8 local_au8_cols[][4] = KEYPAD_COLUMNS;         // all keypad cols
	u8 local_au8_keypadValues[][4] = KEYPAD_VALUES;  // all keypad values
	
	if(copy_u8_value == NULL){
		local_enu_errorstate = ES_NULL_POINTER;
	}else{
		for(u8 i = 0; i < 4; i++){
			// Set each column at a time to low and check for button signal
			local_aenu_errorstate[0] = DIO_enuSetPinValue(local_au8_cols[i][0], local_au8_cols[i][1], DIO_LOW);
			
			for(u8 j = 0; j < 4; j++){
				//Check every row at this column if its button is pressed
				local_aenu_errorstate[1] = DIO_enuReadBit(local_au8_rows[j][0], local_au8_rows[j][1], &local_u8_buttonstate);
				
				if(local_u8_buttonstate == DIO_LOW){
					_delay_ms(3);  // for bouncing
					local_aenu_errorstate[2] = DIO_enuReadBit(local_au8_rows[j][0], local_au8_rows[j][1], &local_u8_buttonstate);  // get the reading again to confirm
					if(local_u8_buttonstate == DIO_LOW){                            
						*copy_u8_value = local_au8_keypadValues[j][i];             // take action
						local_bool_pressed = True;
						while(local_u8_buttonstate == DIO_LOW)                     // to hold the state until the next press and prvent hold on a button
							local_aenu_errorstate[2] = DIO_enuReadBit(local_au8_rows[j][0], local_au8_rows[j][1], &local_u8_buttonstate);
						break;                       // breaks from the first loop
					}
				}
			}
			// Return the value of the column to high since there is no signal or after detecting a signal so we can make it to low again and check for buttons state
			local_aenu_errorstate[4] = DIO_enuSetPinValue(local_au8_cols[i][0], local_au8_cols[i][1], DIO_HIGH);
			if(local_bool_pressed == True){
				break;                             // breaks from the second loop
			}
		}
	}
	for(u8 i = 0; i < 4; i++){
		local_enu_errorstate = local_aenu_errorstate[i];
		if(local_enu_errorstate != ES_OK){
			break;
		}
	}
	
	return local_enu_errorstate;
}
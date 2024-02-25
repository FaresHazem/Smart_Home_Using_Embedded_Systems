/*
 * Login_Prog.c
 *
 * Created: 10/4/2023 6:02:58 PM
 *  Author: Beast
 */ 

#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#define F_CPU 16000000
#include <util/delay.h>

#include "../../HAL/KeyPad/KeyPad_Interface.h"
#include "../../HAL/LCD/LCD_Init.h"
#include "../../HAL/LED/LED_Interface.h"

#include "Login_Priv.h"

extern boolean local_bool_welcomeScreen;
extern boolean local_bool_loginscreen;
extern boolean local_bool_thiefscreen;
extern boolean local_bool_ledscreen;
static boolean local_bool_mainscreen;

extern u8 local_u8_error_counter;
extern const u8 local_au8_thief[];

ES LoginSystem_enuInit(){
	
	LCD_enuInit();
	KEYPAD_enuInit();
	LED_enuInit();
	
	return ES_OK;
}

ES enuWelcomeScreen(boolean copy_bool_screen_state){
	
	if(copy_bool_screen_state == True){         // Screen on
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);				//Clear the display
		LCD_enuSendCommand(LCD_CURSOR_OFF);					//Remove the cursor
		LCD_enuSetCursor(ROW1, COL6);
		_delay_ms(100);
		LCD_enuSendString("Welcome");                       //Print welcome on the LCD
		_delay_ms(300);
		LCD_enuSetCursor(ROW2, COL1);
		LCD_enuSendString("Loading");                       //Loading Phase
		_delay_ms(500);
		LCD_enuSetCursor(ROW2, COL8);
		for(u8 i = 0; i < 9; i++){
			LCD_enuSendData('-');
			_delay_ms(150);
		}
	}
	return ES_OK;
}

ES enuLoginScreen(boolean copy_bool_screen_state, boolean *copy_bool_pass_state, u8 *copy_u8_error_counter){
	
	u8 local_u8_value;
	u8 local_u8_counter = 0;
	boolean local_bool_reading = True;
	boolean local_bool_checked = True;
	
	char local_char_password[] = PASSWORD;
	u8 local_u8_password_size = (sizeof(local_char_password) / sizeof(local_char_password[0]));
	char local_char_inputPass[local_u8_password_size];
	
	if(copy_bool_screen_state == True){
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);				//Clear the display
		LCD_enuSendCommand(LCD_CURSOR_OFF);					//Remove the cursor
		
		LCD_enuSetCursor(ROW2, COL16);
		LCD_enuSendData(*copy_u8_error_counter);

		LCD_enuSetCursor(ROW1, COL1);
		LCD_enuSendString("Enter Password");

		LCD_enuSetCursor(ROW2, COL1);
		while(local_bool_reading){
			
			KEYPAD_enuRead(&local_u8_value);
			if(local_u8_value != KEYPAD_NOT_PRESSED){
				if(local_u8_value != 'r' && local_u8_counter < 8){
					local_char_inputPass[local_u8_counter] = local_u8_value;
					LCD_enuSendData('*');
					local_u8_counter++;
				}else if(local_u8_value == 'r'){
					// check password
					for(u8 i = 0; i < local_u8_password_size; i++){
						if(local_char_password[i] != local_char_inputPass[i]){
							local_bool_checked = False;
							(*copy_u8_error_counter)++;
							break;
						}
					}
					*copy_bool_pass_state = local_bool_checked;
					local_bool_reading = False;
				}
			}	
		}
	}
	return ES_OK;
}

ES enuThiefScreen(boolean copy_bool_screen_state){
	
	if(copy_bool_screen_state == True){
		
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		LCD_enuDisplaySpecialPattern(local_au8_thief, 0, 5, ROW1, COL10, LCD_ARABIC);
		
		while(1){
			
			LCD_enuSendCommand(LCD_ENGLISH);
			LCD_enuSetCursor(ROW2, COL5);
			for(u8 i = 0; i < 8; i++){
				LCD_enuSendData('|');
			}
			_delay_ms(200);
			for(u8 i = 0; i < 8; i++){
				LCD_enuSetCursor(ROW2, COL5+i);
				LCD_enuSendData(LCD_SPACE_CLEAR);
			}
			_delay_ms(200);
			
		}
		
	}
	return ES_OK;
}

ES enuLedStates(u8 copy_u8_led_id){
	
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-ON");
	
	LCD_enuSetCursor(ROW1, COL8);
	LCD_enuSendString("2-OFF");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("3-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				// LED ON
				LED_enuON(copy_u8_led_id);
			}else if(local_u8_value == '2'){
				// LED OFF
				LED_enuOFF(copy_u8_led_id);
			}else if(local_u8_value == '3'){
				// Return to LED screen
				local_bool_reading = False;
				enuLedScreen(local_bool_ledscreen);
			}else{}
		}
	}
	
	return ES_OK;
}

ES enuLedScreen(boolean copy_bool_screen_state){
	
	boolean local_bool_reading = True;
	u8 local_u8_value;
	u8 local_u8_led_id;
	
	if(copy_bool_screen_state == True){
		
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		
		LCD_enuSetCursor(ROW1, COL1);
		LCD_enuSendString("1-LED1");
		
		LCD_enuSetCursor(ROW1, COL8);
		LCD_enuSendString("2-LED2");
		
		LCD_enuSetCursor(ROW2, COL1);
		LCD_enuSendString("3-LED3");
		
		LCD_enuSetCursor(ROW2, COL8);
		LCD_enuSendString("4-BACK");
		
		while(local_bool_reading){
			
			KEYPAD_enuRead(&local_u8_value);
			if(local_u8_value != KEYPAD_NOT_PRESSED){
				if(local_u8_value == '1'){
					//Led state Screen with parameter led 1
					local_u8_led_id = LED_0;
					enuLedStates(local_u8_led_id);
				}else if(local_u8_value == '2'){
					//Led state Screen with parameter led 2
					local_u8_led_id = LED_1;
					enuLedStates(local_u8_led_id);
				}else if(local_u8_value == '3'){
					//Led state Screen with parameter led 3
					local_u8_led_id = LED_2;
					enuLedStates(local_u8_led_id);
				}else if(local_u8_value == '4'){
					//Main Screen
					local_bool_reading = False;
					enuMainScreen(local_bool_mainscreen);
				}else{}//Do Nothing
			}	
		}
	}
	return ES_OK;
}

ES enuTempScreen(){
	
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("Temp: ");
	LCD_enuDisplayFNumbers(25.75);
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("1-Back");
	
	while(local_bool_reading){
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				local_bool_reading = False;
				enuMainScreen(local_bool_mainscreen);
			}
		}
	}
	return ES_OK;
}


ES LoginSystem_enuStart(){
	
	boolean local_bool_pass_state;
	
	// Welcome Screen
	enuWelcomeScreen(local_bool_welcomeScreen);                        // Starting the system with one time welcome screen
	local_bool_welcomeScreen = False;								   // Shutting down the welcome screen for the rest of the program
	
	if(local_u8_error_counter < '3'){
		// Login Screen
		enuLoginScreen(local_bool_loginscreen, &local_bool_pass_state, &local_u8_error_counter);
		// Checking
		if(local_bool_pass_state == True){
			local_u8_error_counter = '0';  // Reset Password
			local_bool_loginscreen = False;
			LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
			// Home Screen (Main Dashboard)
			enuMainScreen(local_bool_mainscreen);
		}
	}else{
		local_bool_loginscreen = False;
		// 7ramy screen
		enuThiefScreen(local_bool_thiefscreen);
		local_bool_thiefscreen = False;
	}
	return ES_OK;
}

ES enuMainScreen(boolean copy_bool_screen_state){
	
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	if(copy_bool_screen_state == True){
		
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		LCD_enuSetCursor(ROW1, COL1);
		LCD_enuSendString("1-LED");
		
		LCD_enuSetCursor(ROW1, COL8);
		LCD_enuSendString("2-AC");
		
		LCD_enuSetCursor(ROW2, COL1);
		LCD_enuSendString("3-Temp");
		
		LCD_enuSetCursor(ROW2, COL8);
		LCD_enuSendString("4-Exit");
		
		while(local_bool_reading){
			
			KEYPAD_enuRead(&local_u8_value);
			if(local_u8_value != KEYPAD_NOT_PRESSED){
				if(local_u8_value == '1'){
					//Led Screen
					enuLedScreen(local_bool_ledscreen);
					}else if(local_u8_value == '2'){
					//AC Screen
					enuLedScreen(local_bool_ledscreen);
					}else if(local_u8_value == '3'){
					//Temp Screen
					enuTempScreen();
					}else if(local_u8_value == '4'){
					//Exit Screen
					local_bool_reading = False;
					local_bool_welcomeScreen = True;
					local_bool_loginscreen = True;
					LoginSystem_enuStart();
					}else{//Do Nothing}
				}
			}
		}
	}
	return ES_OK;
}
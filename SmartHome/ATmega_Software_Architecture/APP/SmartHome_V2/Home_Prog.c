/*
 * Home_Prog.c
 *
 * Created: 2/22/2024 7:55:56 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

//Lower Libraries
//MCAL
#include "../../MCAL/DIO/DIO_INT.h"
#include "../../MCAL/EXTI/EXTI_Interface.h"
#include "../../MCAL/ADC/ADC_Interface.h"
#include "../../MCAL/Timers/Timer0/Timer0_Interface.h"
#include "../../MCAL/Timers/Timer1_PWM/PWM_Interface.h"
#include "../../MCAL/Timers/Timer2/Timer2_Interface.h"

//HAL
#include "../../HAL/LED/LED_Interface.h"
#include "../../HAL/Switch/Switch_INT.h"
#include "../../HAL/LCD/LCD_Init.h"
#include "../../HAL/KeyPad/KeyPad_Interface.h"
#include "../../HAL/HexDecoder/HexDec_Interface.h"
#include "../../HAL/ServoMotor/Servo_Interface.h"

//Private Libraries
#include "Home_Priv.h"

u8 Home_u8_Screen_Counter = 0;
u8 Home_u8_Wrong_Pass_Counter = 0;

u8 Servo_Map(u8 angle) {
	if (angle < 0) angle = 0;
	if (angle > 180) angle = 180;
	
	return (u8)(((f32)angle / 180.0) * (64 - 9) + 9);
}

ES Home_enuInit(){
	
	LED_enuInit();
	LCD_enuInit();
	Timer0_enuInit();
	PWM_enuInit();
	Timer2_enuInit();
	KEYPAD_enuInit();
	
	return ES_OK;
}

ES enuLedSystem(u8 copy_u8_led_id){
	LED_enuToggle(copy_u8_led_id);
	return ES_OK;		
}


ES enuServoSystem(u8 copy_u8_servo_id, u8 copy_u8_angle){
	
	u8 local_u8_duty_cycle = Servo_Map(copy_u8_angle);
	
	if(copy_u8_servo_id == 1){
		//Servo at oc1A
		PWM_enuGeneratePWM_FixedFreq(local_u8_duty_cycle, 0);
		
	}else if (copy_u8_servo_id == 2){
		//Servo at oc1B
		PWM_enuGeneratePWM_FixedFreq(0, local_u8_duty_cycle);
		
	}else if (copy_u8_servo_id == 3){
		Timer2_enuGeneratePWM(local_u8_duty_cycle);
		
	}else{
		return ES_NOK;
	}
	return ES_OK;
}

ES enuHomeWelcomeScreen(){
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);				//Clear the display
	LCD_enuSendCommand(LCD_CURSOR_OFF);					//Remove the cursor
	LCD_enuSetCursor(ROW1, COL6);
	Timer0_enuPolling_SetTimer(100);
	LCD_enuSendString("Welcome");                       //Print welcome on the LCD
	Timer0_enuPolling_SetTimer(300);
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("Loading");                       //Loading Phase
	Timer0_enuPolling_SetTimer(500);
	LCD_enuSetCursor(ROW2, COL8);
	for(u8 i = 0; i < 9; i++){
		LCD_enuSendData('-');
		Timer0_enuPolling_SetTimer(150);
	}
	return ES_OK;
}

ES enuHomePortalMaster(u8 copy_u8_portal, void *copy_pvid_functionPtr()){
	u8 local_u8_value;
	boolean local_bool_reading = True;

	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-Open");
	
	LCD_enuSetCursor(ROW1, COL9);
	LCD_enuSendString("2-Close");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("3-Back");
	
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if((local_u8_value == '1') || (local_u8_value == '2') || (local_u8_value == '3')){
				switch(copy_u8_portal){
					case 1:
						//Door
						if(local_u8_value == '1'){
							enuServoSystem(1, 90);
							/*enuServoSystem(1, 180);
							Timer0_enuPolling_SetTimer(800);
							enuServoSystem(1, 90);*/
						}else if(local_u8_value == '2'){
							/*enuServoSystem(1, 0);
							Timer0_enuPolling_SetTimer(800);
							enuServoSystem(1, 90);*/
						}else if(local_u8_value == '3'){
							copy_pvid_functionPtr();
						}
					break;
					
					case 2:
						//Gate
						if(local_u8_value == '1'){
							/*enuServoSystem(2, 180);
							Timer0_enuPolling_SetTimer(2000);
							enuServoSystem(2, 90);*/
						}else if(local_u8_value == '2'){
							/*enuServoSystem(2, 0);
							Timer0_enuPolling_SetTimer(2000);
							enuServoSystem(2, 90);*/
						}else if(local_u8_value == '3'){
							copy_pvid_functionPtr();
						}
					break;
					
					case 3:
						//Window
						if(local_u8_value == '1'){
							//enuServoSystem(3, 180);
						}else if(local_u8_value == '2'){
							//enuServoSystem(3, 90);
						}else if(local_u8_value == '3'){
							copy_pvid_functionPtr();
						}
					break;
				}
			}
		}
	}
	return ES_OK;
}

ES enuHomeLoginScreen(boolean *copy_bool_pass_state){
	
	u8 local_u8_value;
	u8 local_u8_counter = 0;
	boolean local_bool_reading = True;
	boolean local_bool_checked = True;
	
	char local_char_password[] = HOME_SYSTEM_PASS;
	u8 local_u8_password_size = (sizeof(local_char_password) / sizeof(local_char_password[0]));
	char local_char_inputPass[local_u8_password_size];
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);				//Clear the display
	LCD_enuSendCommand(LCD_CURSOR_OFF);					//Remove the cursor
		
	LCD_enuSetCursor(ROW2, COL16);
	LCD_enuDisplayIntNumbers(Home_u8_Wrong_Pass_Counter);

	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("Enter Password");

	LCD_enuSetCursor(ROW2, COL1);
	while(local_bool_reading){
			
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_counter < local_u8_password_size && (local_u8_value != 'A') && (local_u8_value != 'D')){
				local_char_inputPass[local_u8_counter] = local_u8_value;
				LCD_enuSendData('*');
				local_u8_counter++;
			}else if(local_u8_value == 'D'){
				if(local_u8_counter != 0){
					//Delete One Character
					LCD_enuSetCursor(ROW2, (COL1 + local_u8_counter)-1);
					LCD_enuSendData(' ');
					local_u8_counter--;
					LCD_enuSetCursor(ROW2, (COL1 + local_u8_counter));
				}
			}else if(local_u8_value == 'A'){
				//Check password
				for(u8 i = 0; i < local_u8_password_size; i++){
					if(local_char_inputPass[i] != local_char_password[i]){
						Home_u8_Wrong_Pass_Counter++;
						local_bool_checked = False;
						break;
					}
				}
				local_bool_reading = False;
				*copy_bool_pass_state = local_bool_checked;
			}
		}
	}
	return ES_OK;
}

ES enuHomeThiefScreen(){
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuDisplaySpecialPattern(Home_au8_thief, 0, 5, ROW1, COL10, LCD_ARABIC);
	
	while(1){
		LCD_enuSendCommand(LCD_ENGLISH);
		LCD_enuSetCursor(ROW2, COL5);
		for(u8 i = 0; i < 8; i++){
			LCD_enuSendData('|');
		}
		DIO_enuSetPinValue(DIO_GROUP_B, DIO_PIN_6, DIO_HIGH);
		Timer0_enuPolling_SetTimer(300);
		for(u8 i = 0; i < 8; i++){
			LCD_enuSetCursor(ROW2, COL5+i);
			LCD_enuSendData(LCD_SPACE_CLEAR);
		}
		Timer0_enuPolling_SetTimer(300);
		DIO_enuSetPinValue(DIO_GROUP_B, DIO_PIN_6, DIO_LOW);
	}
	return ES_OK;
}

// ES enuHomeLedScreen(){
// 	
// 	boolean local_bool_reading = True;
// 	u8 local_u8_value;
// 	u8 local_u8_led_id;
// 	int local_casted_led_id;
// 	
// 	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
// 		
// 	LCD_enuSetCursor(ROW1, COL1);
// 	LCD_enuSendString("1-Led1");
// 	
// 	LCD_enuSetCursor(ROW1, COL8);
// 	LCD_enuSendString("2-Led2");
// 	
// 	LCD_enuSetCursor(ROW2, COL1);
// 	LCD_enuSendString("3-Next");
// 	
// 	LCD_enuSetCursor(ROW2, COL5);
// 	LCD_enuSendString("5-L5");
// 	
// 	LCD_enuSetCursor(ROW2, COL8);
// 	LCD_enuSendString("6-Back");
// 		
// 	while(local_bool_reading){
// 			
// 		KEYPAD_enuRead(&local_u8_value);
// 		if(local_u8_value != KEYPAD_NOT_PRESSED){
// 			if((local_u8_value == '1') || (local_u8_value == '2') || (local_u8_value == '3') || (local_u8_value == '4') || (local_u8_value == '5')){
// 				local_casted_led_id = (local_u8_value - '0');
// 				enuLedSystem(local_casted_led_id - 1);
// 			}
// 		}
// 	}
// 	return ES_OK;
// }

ES enuHomeLivingRoomScreen(){
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-Light");
	
	LCD_enuSetCursor(ROW1, COL9);
	LCD_enuSendString("2-Door");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("3-Other");
	
	LCD_enuSetCursor(ROW2, COL9);
	LCD_enuSendString("4-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				//Light Condition
				enuLedSystem(LED_0);
			}else if(local_u8_value == '2'){
				enuHomePortalMaster(1, &enuHomeLivingRoomScreen);
			}else if(local_u8_value == '3'){
				//Other
			}else if(local_u8_value == '4'){
				enuHomeMainScreen();
			}
		}
	}
	return ES_OK;
}

ES enuHomeBedRoomScreen(){
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-LightA");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("2-LightB");
	
	LCD_enuSetCursor(ROW2, COL11);
	LCD_enuSendString("3-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				//Light Condition
				enuLedSystem(LED_1);
			}else if(local_u8_value == '2'){
				enuLedSystem(LED_2);
			}else if(local_u8_value == '3'){
				//Back
				enuHomeMainScreen();
			}
		}
	}
	return ES_OK;
}


ES enuHomeKitchenScreen(){
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-Light");
	
	LCD_enuSetCursor(ROW1, COL9);
	LCD_enuSendString("2-Window");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("3-Temp");
	
	LCD_enuSetCursor(ROW2, COL9);
	LCD_enuSendString("4-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				//Light Condition
				enuLedSystem(LED_3);
			}else if(local_u8_value == '2'){
				enuHomePortalMaster(2, &enuHomeKitchenScreen);
			}else if(local_u8_value == '3'){
				//Display Temperature
			}else if(local_u8_value == '4'){
				enuHomeMenuSubScreen();
			}
		}
	}
	return ES_OK;
}

ES enuHomeGateScreen(){
	boolean local_bool_reading = True;
	u8 local_u8_value;
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-Light");
	
	LCD_enuSetCursor(ROW1, COL9);
	LCD_enuSendString("2-Gate");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("3-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				//Light Condition
				enuLedSystem(LED_4);
			}else if(local_u8_value == '2'){
				enuHomePortalMaster(2, &enuHomeGateScreen);
			}else if(local_u8_value == '3'){
				enuHomeMenuSubScreen();
			}
		}
	}
	return ES_OK;
}

ES enuHomeMenuSubScreen(){
	boolean local_bool_reading = True;
	u8 local_u8_value;

	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("4-Kitchen");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("5-Garden");
	
	LCD_enuSetCursor(ROW2, COL11);
	LCD_enuSendString("6-Back");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '4'){
				//Kitchen
				enuHomeKitchenScreen();
			}else if(local_u8_value == '5'){
				//Garden
				enuHomeGateScreen();
			}else if(local_u8_value == '6'){
				//Back to MainScreen
				enuHomeMainScreen();
			}
		}
	}
	return ES_OK;
}


ES enuHomeMainScreen(){
	
	boolean local_bool_reading = True;
	u8 local_u8_value;

	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("1-Living Room");
	
	LCD_enuSetCursor(ROW2, COL1);
	LCD_enuSendString("2-BedRoom");
	
	LCD_enuSetCursor(ROW2, COL11);
	LCD_enuSendString("3-Next");
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_value == '1'){
				//Living Room
				enuHomeLivingRoomScreen();
			}else if(local_u8_value == '2'){
				//BedRoom
				enuHomeBedRoomScreen();
			}else if(local_u8_value == '3'){
				enuHomeMenuSubScreen();
			}
		}
	}
	return ES_OK;
}


ES Home_enuStart(){
	
	boolean local_bool_system_pass_state;
	
	switch(Home_u8_Screen_Counter){
		
		case 0:
			//Welcome Screen
			enuHomeWelcomeScreen();
			Home_u8_Screen_Counter++;
			break;
			
		case 1:
			//Login Screen
			enuHomeLoginScreen(&local_bool_system_pass_state);
			if(local_bool_system_pass_state == True){
				Home_u8_Screen_Counter++; //enter menu screen
			}else{
				if(Home_u8_Wrong_Pass_Counter == 3){
					Home_u8_Screen_Counter = 69;
					enuHomeThiefScreen();
				}
			}
			break;
			
		case 2:
			//Menu Screen
			enuHomeMainScreen();
		
		break;
			//Menu
	}
	return ES_OK;
}
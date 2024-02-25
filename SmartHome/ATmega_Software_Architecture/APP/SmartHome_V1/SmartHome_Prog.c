/*
 * SmartHome_Prog.c
 *
 * Created: 2/21/2024 6:13:00 PM
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

//HAL
#include "../../HAL/LED/LED_Interface.h"
#include "../../HAL/Switch/Switch_INT.h"
#include "../../HAL/LCD/LCD_Init.h"
#include "../../HAL/KeyPad/KeyPad_Interface.h"
#include "../../HAL/HexDecoder/HexDec_Interface.h"
#include "../../HAL/ServoMotor/Servo_Interface.h"

//Private Libraries
#include "SmartHome_Priv.h"

u8 Home_u8_WrongPassCounter = 0;

ES SmartHome_enuInit(){
	
	//Initialize all MCAL
	//DIO_enuInit();
	//EXTI_enuInit();
	//ADC_enuInit();
	Timer0_enuInit();
	PWM_enuInit();
	
	//Initialize all HAL
	//LED_enuInit();
	//Switch_enu_Init();
	LCD_enuInit();
	KEYPAD_enuInit();
	//HexDec_enu_Init();
	//Switch_enu_Init();
	return ES_OK;
}

u8 map(u8 angle) {
	if (angle < 0) angle = 0;
	if (angle > 180) angle = 180;
	
	return (u8)(((f32)angle / 180.0) * (64 - 9) + 9);
}

ES enuGateControl(boolean * copy_pbool_gatestate){
	
	u8 local_u8_value;
	boolean local_bool_reading = True;
	u8 local_u8_counter = 0;
	boolean local_bool_checked = True;
	
	char local_char_password[] = HOME_GATE_PASSWORD;
	u8 local_u8_password_size = (sizeof(local_char_password) / sizeof(local_char_password[0]));
	char local_char_inputPass[local_u8_password_size];
	
	LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
	LCD_enuSendCommand(LCD_CURSOR_OFF);					//Remove the cursor
	
	LCD_enuSetCursor(ROW2, COL16);
	LCD_enuSendData(Home_u8_WrongPassCounter);

	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendString("Gate Password: ");
	LCD_enuSetCursor(ROW2, COL1);
	
	while(local_bool_reading){
		
		KEYPAD_enuRead(&local_u8_value);
		if(local_u8_value != KEYPAD_NOT_PRESSED){
			if(local_u8_counter < local_u8_password_size && (local_u8_value != 'A') && (local_u8_value != 'D')){
				local_char_inputPass[local_u8_counter] = local_u8_value;
				LCD_enuSendData('*');
				local_u8_counter++;
			}else if(local_u8_value == 'D'){
				//Delete One Character
				LCD_enuSetCursor(ROW2, (COL1 + local_u8_counter)-1);
				LCD_enuSendData(' ');
				local_u8_counter--;
				LCD_enuSetCursor(ROW2, (COL1 + local_u8_counter));
			}else if(local_u8_value == 'A'){
				//Check password
				for(u8 i = 0; i < local_u8_password_size; i++){
					if(local_char_inputPass[i] != local_char_password[i]){
						Home_u8_WrongPassCounter++;
						local_bool_checked = False;
						break;
					}
				}
				*copy_pbool_gatestate = local_bool_checked;
				local_bool_reading = False;
			}	
		}
	}
	return ES_OK;
}

ES enuLoginControl(){
	
	
	
}

ES enuOperateGate(boolean copy_bool_gatestate){
	u8 duty_cycle;
	
	if(Home_u8_WrongPassCounter < 3){
		if(copy_bool_gatestate != False){
			Home_bool_gate_system = False;
			Home_u8_WrongPassCounter = 0;
			LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
			LCD_enuSetCursor(ROW1, COL1);
			LCD_enuSendString("Access Granted!");
			//Open Gate
			duty_cycle = map(90);
			PWM_enuGeneratePWM_FixedFreq(duty_cycle, duty_cycle);
		}else if(copy_bool_gatestate == False && Home_u8_WrongPassCounter < 3){
			LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
			LCD_enuSetCursor(ROW1, COL1);
			LCD_enuSendString("Wrong Password!");
			Timer0_enuPolling_SetTimer(1000);
		}
	}else{
		Home_bool_gate_system = False;
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		LCD_enuSetCursor(ROW1, COL1);
		//Thief Screen
		LCD_enuSendString("Thief!");
		Timer0_enuPolling_SetTimer(1000);
	}
	return ES_OK;
}


ES SmartHome_enuStartSystem(){
	
	boolean local_bool_gatestate;
	
	if(Home_bool_gate_system == True){
		enuGateControl(&local_bool_gatestate);
		enuOperateGate(local_bool_gatestate);
		if(Home_bool_gate_system == False){
			Home_bool_login_system = True;
		}
	}
	
	if(Home_bool_login_system == True){
		//login
	}
	
	return ES_OK;
}
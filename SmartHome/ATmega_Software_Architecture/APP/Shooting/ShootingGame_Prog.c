/*
 * ShootingGame_Prog.c
 *
 * Created: 10/1/2023 4:36:07 PM
 *  Author: Beast
 */ 

#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"
#include "../../LIB/BitMath.h"

#define F_CPU 16000000
#include <util/delay.h>

#include "../../MCAL/DIO/DIO_INT.h"
#include "../../HAL/LCD/LCD_Init.h"
#include "../../HAL/LED/LED_Interface.h"
#include "../../HAL/Switch/Switch_INT.h"

#include "ShootingGame_Priv.h"

extern u8 local_au8_shooter[];
extern u8 local_au8_pistol[];

ES Game_enuInit(){
	
	LCD_enuInit();
	Switch_enu_Init();
	DIO_enuSetPinMode(DIO_GROUP_A, DIO_PIN_3, DIO_OUTPUT);  // buzzer
	
	LCD_enuDisplaySpecialPattern(local_au8_shooter, 0, 1, ROW1, COL16, LCD_ENGLISH);
	LCD_enuDisplaySpecialPattern(local_au8_pistol, 1, 1, ROW1, COL15, LCD_ENGLISH);
	LCD_enuDisplaySpecialPattern(local_au8_victim, 2, 1, ROW1, COL1, LCD_ENGLISH);
	LCD_enuDisplaySpecialPattern(local_au8_bullet, 3, 1, ROW1, COL14, LCD_ENGLISH);
	
	LCD_enuSetCursor(ROW1, COL16);
	LCD_enuSendData(0);
	LCD_enuSetCursor(ROW1, COL15);
	LCD_enuSendData(1);
	LCD_enuSetCursor(ROW1, COL1);
	LCD_enuSendData(2);
	LCD_enuSendCommand(LCD_CURSOR_OFF);
	
	return ES_OK;
}

ES Game_enuStart(){
	
	ES local_enu_errorstate = ES_NOK;
	
	u8 local_u8_buttonstate;
	boolean local_bool_flag = False;
	
	Switch_enu_GetSwitchState(SWITCH_BUTTON_0, &local_u8_buttonstate);
	if(local_u8_buttonstate == DIO_HIGH && local_bool_flag == False){
		
		for(u8 i = 13; i > 0; i--){
			LCD_enuSetCursor(ROW1, i+1);
			LCD_enuSendData(LCD_SPACE_CLEAR);
		
			LCD_enuSetCursor(ROW1, i);
			LCD_enuSendData(3);
			_delay_ms(50);
		}
	
		//Clear the bullet
		LCD_enuSetCursor(ROW1, 1);
		LCD_enuSendData(LCD_SPACE_CLEAR);
		
		//Alarm
		for(u8 i = 0; i < 5; i++){
			DIO_enuSetPinValue(DIO_GROUP_A, DIO_PIN_3, DIO_HIGH);
			_delay_ms(200);
			DIO_enuSetPinValue(DIO_GROUP_A, DIO_PIN_3, DIO_LOW);
		}
		
		local_bool_flag = True;	
		
		LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		LCD_enuSetCursor(ROW1, COL5);
		LCD_enuSendString("Game Over");
		
	}else{}
		
	return ES_OK;
}
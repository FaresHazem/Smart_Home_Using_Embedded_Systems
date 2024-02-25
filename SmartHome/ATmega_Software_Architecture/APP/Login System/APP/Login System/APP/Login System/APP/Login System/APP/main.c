/*
 * ATmega_Software_Architecture.c
 *
 * Created: 9/22/2023 6:32:55 PM
 * Author : Beast
 */ 

//Public Libraries
#include <avr/io.h>
#include "../LIB/BitMath.h"
#include "../LIB/ErrorState.h"
#include "../LIB/stdTypes.h"

//Delay
#define F_CPU 16000000
#include <util/delay.h>
#include <stdio.h>

//Hal-Mcal
#include "../MCAL/DIO/DIO_INT.h"
#include "../MCAL/GIE/GIE_Interface.h"
#include "../MCAL/EXTI/EXTI_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"

#include "../HAL/HexDecoder/HexDec_Interface.h"
#include "../HAL/LED/LED_Interface.h"
#include "../HAL/Switch/Switch_INT.h"
#include "../HAL/LCD/LCD_Init.h"
#include "../HAL/KeyPad/KeyPad_Interface.h"

#include "../HAL/Test/Interface.h"

//APPS
#include "../APP/Traffic_Light/Traffic_Light_INT.h"
#include "../APP/Shooting/ShootingGame_Interface.h"
#include "../APP/Calculator/Calculator_Interface.h"
#include "../APP/Login System/Login_Interface.h"

extern u16 ISR_CALLS;
extern u16 PRELOAD;
extern u16 SAVER;

void delay(){
	
	ISR_CALLS--;
	if(ISR_CALLS == 0){
		ISR_CALLS = 250;
		DIO_enuToggleBit(DIO_GROUP_D, DIO_PIN_0);
	}
	
}

int main(void){
	
	DIO_enuSetPinMode(DIO_GROUP_B, DIO_PIN_3, DIO_OUTPUT);				//oc0
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_0, DIO_OUTPUT);				//oc0
	
	Timer_enuInit();
	
	//Timer_enuSetCallBack(delay);
	
	Timer_enuSetTimerValue(200);
	
	GIE_vidEnable();
	while(1){
		
	}
}


// void __vector_11(void)__attribute__((signal));
// void __vector_11(void){
// 	no_of_ISR--;
// 	if(no_of_ISR == 0){
// 		TCNT0 = preload;
// 		DIO_enuToggleBit(DIO_GROUP_D, DIO_PIN_0);
// 		no_of_ISR = no_OVF_needed;
// 	}
// }


/*   ------------ADC-------------------
struct x{
	
	u16 local_u16_value;
	boolean local_bool_flag;
	
};

void adc(void * px){
	
	struct x * ptr = (struct x *)px;				// Type casting the parameter to struct and assigning it to a pointer to struct
	
	ADC_enuRead(&(ptr->local_u16_value));
	ptr->local_bool_flag = True;
	
}

int main(void)
{
	
	struct x Param;
	
	 ADC_enuInit();
	 LCD_enuInit();
	 EXTI_enuInit();
	 
	 DIO_enuSetPinMode(DIO_GROUP_A, DIO_PIN_1, DIO_INPUT);		// Analog Pin
	 //DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_2, DIO_INPUT);		// External Interrupt
	 
	 ADC_enuSetCallBack(adc, &Param);
	 
	 //ADC_enuTriggerSource(ADC_SINGLE_CONVERSION);
	 ADC_enuStartConversion();
	 
	 GIE_vidEnable();
	 
	 while(1){
		//ADC_enuStartConversion(); // single conversion
		LCD_enuSendCommand(LCD_CURSOR_OFF);
		 if(Param.local_bool_flag == True){
			Param.local_bool_flag = False;
			LCD_enuSetCursor(ROW1, COL1);
			LCD_enuDisplayIntNumbers(Param.local_u16_value);
			//_delay_ms(200);
			//LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
		 }
	 }
	
}
*/
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
#include <stdio.h>

//Hal-Mcal
#include "../MCAL/DIO/DIO_INT.h"
#include "../MCAL/GIE/GIE_Interface.h"
#include "../MCAL/EXTI/EXTI_Interface.h"
#include "../MCAL/ADC/ADC_Interface.h"
#include "../MCAL/Timers/Timer0/Timer0_Interface.h"
#include "../MCAL/Timers/Timer1_PWM/PWM_Interface.h"

#include "../HAL/HexDecoder/HexDec_Interface.h"
#include "../HAL/LED/LED_Interface.h"
#include "../HAL/Switch/Switch_INT.h"
#include "../HAL/LCD/LCD_Init.h"
#include "../HAL/KeyPad/KeyPad_Interface.h"


//APPS
#include "../APP/Traffic_Light_V0/Traffic_Light_INT.h"
#include "../APP/Traffic_Light_V1/TrafficLight_V1_Interface.h"
#include "../APP/Shooting/ShootingGame_Interface.h"
#include "../APP/Calculator/Calculator_Interface.h"
#include "../APP/Login System/Login_Interface.h"
#include "../APP/POV/POV_Interface.h"
#include "../APP/SmartHome_V1/SmartHome_Interface.h"
#include "../APP/SmartHome_V2/Home_Interface.h"


int main(void){
	
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_5, DIO_OUTPUT);
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_4, DIO_OUTPUT);
	DIO_enuSetPinMode(DIO_GROUP_B, DIO_PIN_6, DIO_OUTPUT);
	Home_enuInit();
	//SmartHome_enuStartSystem();
	while(1){
		Home_enuStart();
	}
}

//------------------------------------Servo--------------------------------
// for fast PWM  10bit
/*
u8 map(u8 angle) {                                   
	if (angle < 0) angle = 0;
	if (angle > 180) angle = 180;
	
	return (u8)(((f32)angle / 180.0) * (64 - 9) + 9);
}

int main(void) {
	PWM_enuInit();
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_5, DIO_OUTPUT);
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_4, DIO_OUTPUT);
	u8 duty_cycle;
	
	while (1) {
		for(u8 i = 0; i < 180; i++){
			duty_cycle = map(i);
			PWM_enuGeneratePWM_FixedFreq(duty_cycle, duty_cycle);
			_delay_ms(50);
		}
	}
}
*/
//-------------------------------------------------------------------------

//--------------------------PWM--------------------------------------------

// int main(void){
// 	
// 	PWM_enuInit();
// 	/*PWM_enuGeneratePWM_FixedFreq(35, 50);*/
// 	PWM_enuGeneratePWM_AdjustableFreq_2X(50, 50, 50);
// 	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_5, DIO_OUTPUT);
// 	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_4, DIO_OUTPUT);
// 	
// 	while(1){
// 	}
// }

/*
int main(void){
	
	//Calculation of Frequency:
	//In Fast PWM = fcpu / (top * presc)
	//In Phase PWM = fcpu / (2 * top * presc)
	
	TCCR1A = 0X21;
	TCCR1B = 0X13;
	
	u8 freq = 50;
	u16 Top = (16000000UL) / (2UL*64UL*freq);
	OCR1AH = (Top >> 8);
	OCR1AL = (u8)Top;
	
	u8 duty = 5;
	u16 ocr = (Top * (u32)duty) / 100;
	
	OCR1BH = (ocr >> 8);
	OCR1BL = (ocr);
	
	DIO_enuSetPinMode(DIO_GROUP_D, DIO_PIN_4, DIO_OUTPUT);
}
*/

//----------------------Timer0 (Phase Correct-PWM)--------------------------
/*
int main(void){
	
	u8 duty = 50;
	TCCR0 = 0b01100101;     //phase correct, 1024, non inverted
	
	OCR0 = ((256UL * duty) / 100);
	
	DIO_enuSetPinMode(DIO_GROUP_A, DIO_PIN_3, DIO_OUTPUT);
}
*/
//------------------------------------------------------------------

//----------------------Timer0 (Fast-PWM)--------------------------
/*
int main(void){
	
	u8 duty = 50;
	TCCR0 = 0b01101101;     //fast, 1024, non inverted
	
	OCR0 = ((256UL * duty) / 100) - 1;
	
	DIO_enuSetPinMode(DIO_GROUP_A, DIO_PIN_3, DIO_OUTPUT);
}
*/
//------------------------------------------------------------------



/*-------------------Timer0 (Normal OVF and CTC)----------------------
static void delay(){
	LED_enuON(LED_0);
}

int main(void){
			
	TIMER0_enuInit();
	LED_enuInit();
	
	Timer0_enuInterruptDelay(500);
	TIMER0_enuSetCallBack(delay, NULL);
	
	GIE_vidEnable();
	
	while(1){
		LED_enuON(LED_1);
	}
	
}
*/


/*------------ADC-------------------
struct x{
	boolean flag;
	u16 read;
	};

void adc(void *pv){           // call back function with multiple parameters
	
	struct x * ptr = (struct x*)pv; //
	
	ADC_enuRead(&(ptr->read));
	(ptr->flag) = True;
	
}


int main(void){
	
	struct x Param;
	
	ADC_enuInit();
	LCD_enuInit();
	EXTI_enuInit();
	
	DIO_enuSetPinMode(DIO_GROUP_A, DIO_PIN_1, DIO_INPUT);
	DIO_enuSetPinValue(DIO_GROUP_A, DIO_PIN_1, DIO_FLOAT);
		
	ADC_enuSetCallBack(adc, &Param);
	ADC_enuEnableInterrupt();
	ADC_enuStartConversion();
	
	GIE_vidEnable();	
	
	
	while(1){
		//ADC_enuStartConversion();
		if(Param.flag == True){
			LCD_enuSetCursor(ROW1, COL1);
			LCD_enuDisplayIntNumbers(Param.read);
			_delay_ms(200);
			LCD_enuSendCommand(LCD_CLEAR_DISPLAY);
			(Param.flag) = False;
		}	
	}
}
*/
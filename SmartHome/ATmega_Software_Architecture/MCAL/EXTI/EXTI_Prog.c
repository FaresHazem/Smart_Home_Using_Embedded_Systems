/*
 * EXTI_Prog.c
 *
 * Created: 10/8/2023 9:24:07 PM
 *  Author: Beast
 */ 

// Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

// Lower Level Libraries

// Private Libraries
#include "EXTI_Config.h"
#include "EXTI_Priv.h"

extern u8 local_u8_EXTI_max_pin_no;
extern EXTI_Str EXTI_astr_PIN_CONFIG[];

static void (*GLOBAL_FUNCTION_ISR_PTRs[3])(void) = {NULL, NULL, NULL};						// Pointer to function of return type void and with void parameters

ES enuSetIntMode(u8 copy_u8_interrupt_pin, u8 copy_u8_mode){                         // Static Function that Sets the Interrupt Mode of our Interrupts
																					 // All operations here are executed in the GICR which controls whether an interrupt will be enabled or disabled
	ES local_enu_errorstate = ES_OK;
	
	switch(copy_u8_interrupt_pin){
		
		case EXTI_INTERRUPT_PIN0:
			if(copy_u8_mode == EXTI_INTERRUPT_MODE){
				// Set INT0 at pin 6 in General Interrupt Control Register (GICR)
				SetBit(GICR, 6);
			}else{
				// Clear INT0 at pin 6 in General Interrupt Control Register (GICR)
				ClearBit(GICR, 6);
			}
			break;
		case EXTI_INTERRUPT_PIN1:
			if(copy_u8_mode == EXTI_INTERRUPT_MODE){
				// INT1 at pin 7
				SetBit(GICR, 7);
			}else{
				ClearBit(GICR, 7);
			}
			break;
		case EXTI_INTERRUPT_PIN2:
			if(copy_u8_mode == EXTI_INTERRUPT_MODE){
				// INT2 at pin 5
				SetBit(GICR, 5);
			}else{
				ClearBit(GICR, 5);
			}
			break;	
	}	
	return local_enu_errorstate;
}

ES enuSetIntSenseLevel(u8 copy_u8_interrupt_pin, u8 copy_u8_sense_level){						// Static Function that Sets the Sense Level of our Interrupts, this sense level is the detection signal used to know if an interrupt has occurred
																								// Operations here depends on the interrupt pin
	ES local_enu_errorstate = ES_OK;															// if interrupt pin is INT0 or INT1, the operations will occur in MCUCR which is responsible for assigning the sense detection of INT0 and INT1
																								// Also in MCUCR sense levels is adjusted using the 2 bit of INT0 or INT1
	switch(copy_u8_interrupt_pin){
		
		case EXTI_INTERRUPT_PIN0:
			if(copy_u8_sense_level == EXTI_LOW_LEVEL){         // 0 on ISC00(bit-0) and 0 on ISC01(bit-1)
				ClearBit(MCUCR, 0);
				ClearBit(MCUCR, 1);
			}else if(copy_u8_sense_level == EXTI_ANY_LOGICAL_CHANGE){		// 0 on ISC00(bit-0) and 1 on ISC01(bit-1)
				ClearBit(MCUCR, 0);
				SetBit(MCUCR, 1);
			}else if(copy_u8_sense_level == EXTI_FALLING_EDGE){		// 1 on ISC00(bit-0) and 0 on ISC01(bit-1)
				SetBit(MCUCR, 0);
				ClearBit(MCUCR, 1);
			}else if(copy_u8_sense_level == EXTI_RISING_EDGE){		// 1 on ISC00(bit-0) and 1 on ISC01(bit-1)
				SetBit(MCUCR, 0);
				SetBit(MCUCR, 1);
			}else{}
			break;
			
		case EXTI_INTERRUPT_PIN1:
			if(copy_u8_sense_level == EXTI_LOW_LEVEL){         // 0 on ISC10(bit-2) and 0 on ISC11(bit-3)
				ClearBit(MCUCR, 2);
				ClearBit(MCUCR, 3);
			}else if(copy_u8_sense_level == EXTI_ANY_LOGICAL_CHANGE){		// 0 on ISC10(bit-2) and 1 on ISC11(bit-3)
				ClearBit(MCUCR, 2);
				SetBit(MCUCR, 3);
			}else if(copy_u8_sense_level == EXTI_FALLING_EDGE){		// 1 on ISC10(bit-2) and 0 on ISC11(bit-3)
				SetBit(MCUCR, 2);
				ClearBit(MCUCR, 3);
			}else if(copy_u8_sense_level == EXTI_RISING_EDGE){		// 1 on ISC10(bit-2) and 1 on ISC11(bit-3)
				SetBit(MCUCR, 2);
				SetBit(MCUCR, 3);
			}else{}
			break;
			
		case EXTI_INTERRUPT_PIN2:																							// If the interrupt is INT2 then we will use the MCUCSR register which have the ISC2 bit is used to classify wether INT2 will detect falling or rising edge only
			if(copy_u8_sense_level == EXTI_FALLING_EDGE){		// 0 on ISC2(bit-6)
				ClearBit(MCUCSR, 6);
			}else if(copy_u8_sense_level == EXTI_RISING_EDGE){		// 1 on ISC2(bit-6)
				SetBit(MCUCSR, 6);
			}else{}
			break;
	}
	
	return local_enu_errorstate;
}

ES EXTI_enuInit(){
	
	ES local_enu_errorstate = ES_OK;	
	
	for(u8 i = 0; i < local_u8_EXTI_max_pin_no; i++){
		enuSetIntMode(EXTI_astr_PIN_CONFIG[i].interrupt_pin, EXTI_astr_PIN_CONFIG[i].interrupt_mode);						// Setting interrupt mode
		enuSetIntSenseLevel(EXTI_astr_PIN_CONFIG[i].interrupt_pin, EXTI_astr_PIN_CONFIG[i].interrupt_sense_level);			// Setting interrupt edge detection
	}
	
	return local_enu_errorstate;
}

// Manually set the interrupt mode
ES EXTI_enuSetInterruptMode(u8 copy_u8_interrupt_pin, u8 copy_u8_mode){
	
	ES local_enu_errorstate = ES_OK;
	
	enuSetIntMode(copy_u8_interrupt_pin, copy_u8_mode);
	
	return local_enu_errorstate;
}

// Manually set the interrupt sense level detection
ES EXTI_enuSetInterruptSenseLevel(u8 copy_u8_interrupt_pin, u8 copy_u8_sense_level){
	
	ES local_enu_errorstate = ES_NOK;
	
	enuSetIntSenseLevel(copy_u8_interrupt_pin, copy_u8_sense_level);
	
	return local_enu_errorstate;
}

// Manually enable the interrupt
ES EXTI_enuEnableInterrupt(u8 copy_u8_interrupt_pin){
	
	ES local_enu_errorstate = ES_NOK;
	
	enuSetIntMode(copy_u8_interrupt_pin, EXTI_INTERRUPT_MODE);
	
	return local_enu_errorstate;
}

// Manually disable the interrupt
ES EXTI_enuDisableInterrupt(u8 copy_u8_interrupt_pin){
	
	ES local_enu_errorstate = ES_NOK;
	
	enuSetIntMode(copy_u8_interrupt_pin, EXTI_POLLING_MODE);
	
	return local_enu_errorstate;
}

ES EXTI_enuCallBack(u8 copy_u8_interrupt_pin, void(*copy_pfun_ptr)(void)){
	
	ES local_enu_errorstate = ES_NOK;
	
	GLOBAL_FUNCTION_ISR_PTRs[copy_u8_interrupt_pin] = copy_pfun_ptr;	// Now our global pointer to function points to our implemented app of ISR
	
	return local_enu_errorstate;
}

//Writting our ISR() Function for every intrrupt

// Here we make our ISR functions return RETI for interrupt indication
void __vector_1(void)__attribute__((signal));
void __vector_2(void)__attribute__((signal));
void __vector_3(void)__attribute__((signal));

//Interrupt Function INT0
void __vector_1(void){
	
	// Executes at the moment of interrupt
	if(GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN0] != NULL){
		GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN0]();
	}
}

//Interrupt Function INT1
void __vector_2(void){
	if(GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN1] != NULL){
		GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN1]();
	}
}

//Interrupt Function INT2
void __vector_3(void){
	if(GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN2] != NULL){
		GLOBAL_FUNCTION_ISR_PTRs[EXTI_INTERRUPT_PIN2]();
	}
}
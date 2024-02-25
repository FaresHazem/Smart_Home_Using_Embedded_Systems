/*
 * Timer0_Porg.c
 *
 * Created: 1/24/2024 8:33:49 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../../LIB/BitMath.h"
#include "../../../LIB/ErrorState.h"
#include "../../../LIB/stdTypes.h"

//Private Libraries
#include "../../../MCAL/DIO/DIO_INT.h"
#include "Timer0_Config.h"
#include "Timer0_Priv.h"

//Global Private Variable-----------------------------------------------------
static u16 Timer0_U16_Ovfs_Needed;
static u8 Timer0_U8_Preload;
static u16 Timer0_U16_CTC_Interrupts;
//----------------------------------------------------------------------------

//Functions Holder------------------------------------------------------------ 
static void (*local_pvid_fun[4])(void) = {NULL, NULL, NULL, NULL};			// ptr to function
static void * local_pvid_parameter;
//----------------------------------------------------------------------------

ES Timer0_enuInit(){
		
	ES local_enu_errorstate = ES_NOK;
		
	//Set Timer Mode
	#if ((TIMER0_OPERATION_MODE == TIMER0_OVF_MODE) || (TIMER0_OPERATION_MODE == TIMER0_CTC_MODE) || (TIMER0_OPERATION_MODE == TIMER0_FAST_PWM) || (TIMER0_OPERATION_MODE == TIMER0_PHASECORRECT_PWM))
		local_enu_errorstate = ES_OK;
		#if TIMER0_OPERATION_MODE == TIMER0_OVF_MODE
			TCCR0 &= ~(1<<6);
			TCCR0 &= ~(1<<3);
		#elif TIMER0_OPERATION_MODE == TIMER0_CTC_MODE
			TCCR0 &= ~(1<<6);
			TCCR0 |= (1<<3);
		#elif TIMER0_OPERATION_MODE == TIMER0_PHASECORRECT_PWM
			TCCR0 |= (1<<6);
			TCCR0 &= ~(1<<3);
		#elif TIMER0_OPERATION_MODE == TIMER0_FAST_PWM
			TCCR0 |= (1<<6);
			TCCR0 |= (1<<3);
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer0 Configuration Error in Timer Mode"
	#endif
		
	//Set Timer Prescaler
	#if ((TIMER0_PRESCALER >= 0) && (TIMER0_PRESCALER <= 7))
		local_enu_errorstate = ES_OK;
		TCCR0 |= TIMER0_PRESCALER;
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer0 Configuration Error in Timer Prescaler"
	#endif

	//Set OC0-Output Compare Mode in OVF or CTC Mode
	#if ((TIMER0_OC0_MODE == TIMER0_OC0_DISCONNECTED) || (TIMER0_OC0_MODE == TIMER0_OC0_SET) || (TIMER0_OC0_MODE == TIMER0_OC0_CLEAR) || (TIMER0_OC0_MODE == TIMER0_OC0_TOGGLE) || (TIMER0_OC0_MODE == TIMER0_OC0_RESERVED) || (TIMER0_OC0_MODE == TIMER0_OC0_INVERTED) || (TIMER0_OC0_MODE == TIMER0_OC0_NON_INVERTED))
		local_enu_errorstate = ES_OK;
		#if ((TIMER0_OPERATION_MODE == TIMER0_OVF_MODE) || (TIMER0_OPERATION_MODE == TIMER0_CTC_MODE))
			#if TIMER0_OC0_MODE == TIMER0_OC0_DISCONNECTED
				TCCR0 &= ~(1<<5);
				TCCR0 &= ~(1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_SET
				TCCR0 |= (1<<5);
				TCCR0 |= (1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_CLEAR
				TCCR0 |= (1<<5);
				TCCR0 &= ~(1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_TOGGLE
				TCCR0 &= ~(1<<5);
				TCCR0 |= (1<<4);
			#else
				#error "Timer0 Mismatch Error cannot configure OC0-pin mode with the current Timer Mode"
			#endif
		#elif ((TIMER0_OPERATION_MODE == TIMER0_FAST_PWM) || (TIMER0_OPERATION_MODE == TIMER0_PHASECORRECT_PWM))
			#if TIMER0_OC0_MODE == TIMER0_OC0_DISCONNECTED
				TCCR0 &= ~(1<<5);
				TCCR0 &= ~(1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_RESERVED
				TCCR0 &= ~(1<<5);
				TCCR0 |= (1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_NON_INVERTED
				TCCR0 |= (1<<5);
				TCCR0 &= ~(1<<4);
			#elif TIMER0_OC0_MODE == TIMER0_OC0_INVERTED
				TCCR0 |= (1<<5);
				TCCR0 |= (1<<4);
			#else
				#error "Timer0 Mismatch Error cannot configure OC0-pin mode with the current Timer Mode"
			#endif
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer0 Configuration Error in Timer OC0-Output Mode"
	#endif
		
	//Set System Mode
	#if	((TIMER0_SERVICE_MODE == TIMER0_POLLING_MODE) || (TIMER0_SERVICE_MODE == TIMER0_INTERRUPT_MODE))
		local_enu_errorstate = ES_OK;
		#if TIMER0_SERVICE_MODE == TIMER0_POLLING_MODE
			TIMSK &= ~(3);  // 3 --> 0000 0011 so clearing first two bits
		#elif TIMER0_SERVICE_MODE == TIMER0_INTERRUPT_MODE
			TIMSK |= (3);
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer0 Configuration Error in Timer System Mode"
	#endif
			
	return local_enu_errorstate;
}

static ES enu_Calculate_OVF_Mode(f32 copy_f32_prescaler, f32 copy_f32_time, f32 *copy_pf32_number_of_ovfs, u16 *copy_pu16_preload){
	
	copy_f32_time /= 1000;
	
	f32 local_f32_ovf_time = (256.0) * (copy_f32_prescaler / TIMER0_SYSTEM_FREQUENCY);
	
	*copy_pf32_number_of_ovfs = (copy_f32_time / local_f32_ovf_time);
	*copy_pu16_preload = ((256.0) * (((u16)*copy_pf32_number_of_ovfs + 1) - *copy_pf32_number_of_ovfs));	
	
	*copy_pf32_number_of_ovfs = (u16)(*copy_pf32_number_of_ovfs) + 1;
	
	return ES_OK;
}

static ES enu_Calculate_CTC_Mode(f32 copy_f32_prescaler, f32 copy_f32_time, u8 *copy_pu8_OCR_value, u16 *copy_pu16_CTC_interrupts){
	
	f32 local_f32_ovf_time = (256.0) * (copy_f32_prescaler / TIMER0_SYSTEM_FREQUENCY);
	u8 local_u8_CTCtime = (u8)(local_f32_ovf_time*1000);
	*copy_pu8_OCR_value = ((f32)local_u8_CTCtime/1000) * (TIMER0_SYSTEM_FREQUENCY / copy_f32_prescaler);
	*copy_pu16_CTC_interrupts = copy_f32_time / local_u8_CTCtime;
	
	return ES_OK;
}


ES Timer0_enuPolling_SetTimer(f32 copy_f32_time){
	
	ES local_enu_errorstate = ES_NOK;
		
	f32 local_f32_prescaler;
	//OVF-Variables
	f32 local_f32_ovfs_needed;
	u16 local_u16_preload;
	//CTC-Variables
	u8 local_u8_OCR_value;
	u16 local_u16_CTC_interrupts;
	
	switch(TIMER0_OPERATION_MODE){
		
		case TIMER0_OVF_MODE:
			//Calculate Number of Overflows needed and Preload value
			#if (TIMER0_PRESCALER >= 1 && TIMER0_PRESCALER <= 5)
				local_f32_prescaler = TIMER0_PRESCALERS[TIMER0_PRESCALER-1];
				enu_Calculate_OVF_Mode(local_f32_prescaler, copy_f32_time, &local_f32_ovfs_needed, &local_u16_preload);
		
				TCNT0 = local_u16_preload;              //one overflow must have a preload value
		
				while(local_f32_ovfs_needed != 0){
					local_f32_ovfs_needed--;
					while(GetBit(TIFR, 0) == 0);          // wait overflow
					SetBit(TIFR, 0);        //clear the flag
				}
			#else
				#error "Timer0 not in overflow mode"
			#endif
			local_enu_errorstate = ES_OK;
		break;
		
		case TIMER0_CTC_MODE:
			#if (TIMER0_PRESCALER >= 1 && TIMER0_PRESCALER <= 5)
				local_f32_prescaler = TIMER0_PRESCALERS[TIMER0_PRESCALER-1];
				enu_Calculate_CTC_Mode(local_f32_prescaler, copy_f32_time, &local_u8_OCR_value, &local_u16_CTC_interrupts);
				OCR0 = local_u8_OCR_value;             // set OCR0 register with it's value
				u16 local_u16_number_of_compares = 0;         // to count ctc interrupts
				while(local_u16_number_of_compares != local_u16_CTC_interrupts){
					local_u16_number_of_compares++;
					while(GetBit(TIFR, 1) == 0);          // wait overflow
					SetBit(TIFR, 1);        //clear the flag
				}
			#else
				#error "Timer0 not in CTC mode"
			#endif
			local_enu_errorstate = ES_OK;
		break;
	}
	return local_enu_errorstate;
}

ES Timer0_enuInterrupt_SetTimer(f32 copy_f32_time){

	ES local_enu_errorstate = ES_NOK;
	
	f32 local_f32_prescaler;
	//OVF-Variables
	f32 local_f32_ovfs_needed;
	u16 local_u16_preload;
	//CTC-Variables
	u8 local_u8_OCR_value;
	u16 local_u16_CTC_interrupts;
	
	switch(TIMER0_OPERATION_MODE){
		
		case TIMER0_OVF_MODE:
			ClearBit(TIMSK, 1);          //disable CTC interrupt
			//Calculate Number of Overflows needed and Preload value
			#if (TIMER0_PRESCALER >= 1 && TIMER0_PRESCALER <= 5)
				local_f32_prescaler = TIMER0_PRESCALERS[TIMER0_PRESCALER-1];
				enu_Calculate_OVF_Mode(local_f32_prescaler, copy_f32_time, &local_f32_ovfs_needed, &local_u16_preload);
				//Use these values in the ISR
				Timer0_U16_Ovfs_Needed = local_f32_ovfs_needed;
				Timer0_U8_Preload = local_u16_preload;
			#else
				#error "Timer0 not in overflow mode"
			#endif
		break;
		
		case TIMER0_CTC_MODE:
			ClearBit(TIMSK, 0);        //disable OVF interrupt
			//Calculate OCR0 value and number of CTC interrupts
			#if (TIMER0_PRESCALER >= 1 && TIMER0_PRESCALER <= 5)
				local_f32_prescaler = TIMER0_PRESCALERS[TIMER0_PRESCALER-1];
				enu_Calculate_CTC_Mode(local_f32_prescaler, copy_f32_time, &local_u8_OCR_value, &local_u16_CTC_interrupts);
				//Use these values in the ISR and set the OCR0-register
				OCR0 = local_u8_OCR_value;
				Timer0_U16_CTC_Interrupts = local_u16_CTC_interrupts;
			#else
				#error "Timer0 not in CTC mode"
			#endif
		break;
	}
	
	return local_enu_errorstate;
}

ES Timer0_enuInterrupt_GeneratePWM(u8 copy_u8_dutycycle){
	
	ES local_enu_errorstate = ES_NOK;
	u8 local_u8_OCRvalue;

	switch(TIMER0_OPERATION_MODE){
		
		case TIMER0_FAST_PWM:
			#if (TIMER0_OC0_MODE == TIMER0_OC0_NON_INVERTED)
				local_u8_OCRvalue = ((copy_u8_dutycycle * (TIMER0_TOP)) / 100) - 1;
			#elif (TIMER0_OC0_MODE == TIMER0_OC0_INVERTED)
				local_u8_OCRvalue = (TIMER0_TOP - ((copy_u8_dutycycle * (TIMER0_TOP + 1)) / 100));
			#endif
			local_enu_errorstate = ES_OK;
			OCR0 = local_u8_OCRvalue;
			break;
			
		case TIMER0_PHASECORRECT_PWM:
			#if (TIMER0_OC0_MODE == TIMER0_OC0_NON_INVERTED)
				local_u8_OCRvalue = ((copy_u8_dutycycle * TIMER0_TOP) / 100);
			#elif (TIMER0_OC0_MODE == TIMER0_OC0_INVERTED)
				local_u8_OCRvalue = (TIMER0_TOP * (copy_u8_dutycycle + 1)) / 100;
			#endif
			local_enu_errorstate = ES_OK;
			OCR0 = local_u8_OCRvalue;
			break;
	}
	return local_enu_errorstate;
}

ES TIMER0_enuSetCallBack(void *(ptr_to_function)(void*), void * copy_pvid_parameter){
	
	if(ptr_to_function != NULL){
		local_pvid_fun[TIMER0_OPERATION_MODE] = ptr_to_function;                         //depending on the timer mode the function will be assigned to its position in the array and then used
	}
	
	if(copy_pvid_parameter != NULL){
		local_pvid_parameter = copy_pvid_parameter;                           //saving the function parameter as a pointer to be used
	}
	
	return ES_OK;
}

void __vector_11(void)__attribute__((signal));				// Timer0 Normal Overflow ISR
void __vector_10(void)__attribute__((signal));				// Timer0 CTC ISR

void __vector_10(){
	
	static u16 local_u16_interrupts = 0;                        // every time a compare is matched between TCNT0 and OCR0 this interrupt is executed
	local_u16_interrupts++;
	
	if(local_u16_interrupts == Timer0_U16_CTC_Interrupts){
		local_u16_interrupts = 0;
		//stop timer and execute the desired function
		if(local_pvid_fun[1]!=NULL){
			if(local_pvid_parameter == NULL){
				local_pvid_fun[1]();
			}
		}
	}
}

void __vector_11(){
	
	static u16 local_u16_count = 0;                             // every time TCNT0 overflows this interrupts is executed
	local_u16_count++;
	
	if(local_u16_count == Timer0_U16_Ovfs_Needed-1){
		TCNT0 = Timer0_U8_Preload;
	}
	
	if(local_u16_count == Timer0_U16_Ovfs_Needed){
		local_u16_count = 0;
		//stop timer and execute the desired function
		if(local_pvid_fun[0]!=NULL){
			if(local_pvid_parameter == NULL){
				local_pvid_fun[0]();
			}
		}
	}
}
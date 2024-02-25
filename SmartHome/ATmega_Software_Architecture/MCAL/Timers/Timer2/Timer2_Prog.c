/*
 * Timer2_Prog.c
 *
 * Created: 2/25/2024 4:19:33 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../../LIB/BitMath.h"
#include "../../../LIB/ErrorState.h"
#include "../../../LIB/stdTypes.h"

//Private Libraries
#include "../../../MCAL/DIO/DIO_INT.h"
#include "Timer2_Config.h"
#include "Timer2_Priv.h"

//Global Private Variable-----------------------------------------------------
// static u16 Timer2_U16_Ovfs_Needed;
// static u8 Timer2_U8_Preload;
// static u16 Timer2_U16_CTC_Interrupts;
//----------------------------------------------------------------------------

//Functions Holder------------------------------------------------------------
// static void (*local_pvid_fun[4])(void) = {NULL, NULL, NULL, NULL};			// ptr to function
// static void * local_pvid_parameter;
//----------------------------------------------------------------------------

ES Timer2_enuInit(){
	
	ES local_enu_errorstate = ES_NOK;
	
	//Set Timer Mode
	#if ((TIMER2_OPERATION_MODE == TIMER2_OVF_MODE) || (TIMER2_OPERATION_MODE == TIMER2_CTC_MODE) || (TIMER2_OPERATION_MODE == TIMER2_FAST_PWM) || (TIMER2_OPERATION_MODE == TIMER2_PHASECORRECT_PWM))
		local_enu_errorstate = ES_OK;
		#if TIMER2_OPERATION_MODE == TIMER2_OVF_MODE
			TCCR2 &= ~(1<<6);
			TCCR2 &= ~(1<<3);
		#elif TIMER2_OPERATION_MODE == TIMER2_CTC_MODE
			TCCR2 &= ~(1<<6);
			TCCR2 |= (1<<3);
		#elif TIMER2_OPERATION_MODE == TIMER2_PHASECORRECT_PWM
			TCCR2 |= (1<<6);
			TCCR2 &= ~(1<<3);
		#elif TIMER2_OPERATION_MODE == TIMER2_FAST_PWM
			TCCR2 |= (1<<6);
			TCCR2 |= (1<<3);
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer2 Configuration Error in Timer Mode"
	#endif
	
	//Set Timer Prescaler
	#if ((TIMER2_PRESCALER >= 0) && (TIMER2_PRESCALER <= 7))
		local_enu_errorstate = ES_OK;
		TCCR2 |= TIMER2_PRESCALER;
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer2 Configuration Error in Timer Prescaler"
	#endif

	//Set OC2-Output Compare Mode in OVF or CTC Mode
	#if ((TIMER2_OC2_MODE == TIMER2_OC2_DISCONNECTED) || (TIMER2_OC2_MODE == TIMER2_OC2_SET) || (TIMER2_OC2_MODE == TIMER2_OC2_CLEAR) || (TIMER2_OC2_MODE == TIMER2_OC2_TOGGLE) || (TIMER2_OC2_MODE == TIMER2_OC2_RESERVED) || (TIMER2_OC2_MODE == TIMER2_OC2_INVERTED) || (TIMER2_OC2_MODE == TIMER2_OC2_NON_INVERTED))
		local_enu_errorstate = ES_OK;
		#if ((TIMER2_OPERATION_MODE == TIMER2_OVF_MODE) || (TIMER2_OPERATION_MODE == TIMER2_CTC_MODE))
		
			#if TIMER2_OC2_MODE == TIMER2_OC2_DISCONNECTED
				TCCR2 &= ~(1<<5);
				TCCR2 &= ~(1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_SET
				TCCR2 |= (1<<5);
				TCCR2 |= (1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_CLEAR
				TCCR2 |= (1<<5);
				TCCR2 &= ~(1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_TOGGLE
				TCCR2 &= ~(1<<5);
				TCCR2 |= (1<<4);
			#else
				#error "Timer2 Mismatch Error cannot configure OC2-pin mode with the current Timer Mode"
			#endif
		
		#elif ((TIMER2_OPERATION_MODE == TIMER2_FAST_PWM) || (TIMER2_OPERATION_MODE == TIMER2_PHASECORRECT_PWM))
			#if TIMER2_OC2_MODE == TIMER2_OC2_DISCONNECTED
				TCCR2 &= ~(1<<5);
				TCCR2 &= ~(1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_RESERVED
				TCCR2 &= ~(1<<5);
				TCCR2 |= (1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_NON_INVERTED
				TCCR2 |= (1<<5);
				TCCR2 &= ~(1<<4);
			#elif TIMER2_OC2_MODE == TIMER2_OC2_INVERTED
				TCCR2 |= (1<<5);
				TCCR2 |= (1<<4);
			#else
				#error "Timer2 Mismatch Error cannot configure OC2-pin mode with the current Timer Mode"
			#endif
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer2 Configuration Error in Timer OC2-Output Mode"
	#endif
	
	//Set System Mode
	#if	((TIMER2_SERVICE_MODE == TIMER2_POLLING_MODE) || (TIMER2_SERVICE_MODE == TIMER2_INTERRUPT_MODE))
		local_enu_errorstate = ES_OK;
		#if TIMER2_SERVICE_MODE == TIMER2_POLLING_MODE
			TIMSK &= ~(3 << 5);  // 3 --> 0000 0011 so clearing first two bits
		#elif TIMER2_SERVICE_MODE == TIMER2_INTERRUPT_MODE
			TIMSK |= (3 << 5);
		#endif
	#else
		local_enu_errorstate = ES_NOK;
		#error "Timer2 Configuration Error in Timer System Mode"
	#endif
	
	return local_enu_errorstate;
}


ES Timer2_enuGeneratePWM(u8 copy_u8_dutycycle){
	
	ES local_enu_errorstate = ES_NOK;
	u8 local_u8_OCRvalue;

	switch(TIMER2_OPERATION_MODE){
		
		case TIMER2_FAST_PWM:
			#if (TIMER2_OC2_MODE == TIMER2_OC2_NON_INVERTED)
				local_u8_OCRvalue = ((copy_u8_dutycycle * (TIMER2_TOP)) / 100) - 1;
			#elif (TIMER2_OC2_MODE == TIMER2_OC2_INVERTED)
				local_u8_OCRvalue = (TIMER2_TOP - ((copy_u8_dutycycle * (TIMER2_TOP + 1)) / 100));
			#endif
			local_enu_errorstate = ES_OK;
			OCR2 = local_u8_OCRvalue;
		break;
		
		case TIMER2_PHASECORRECT_PWM:
			#if (TIMER2_OC2_MODE == TIMER2_OC2_NON_INVERTED)
				local_u8_OCRvalue = ((copy_u8_dutycycle * TIMER2_TOP) / 100);
			#elif (TIMER2_OC2_MODE == TIMER2_OC2_INVERTED)
				local_u8_OCRvalue = (TIMER2_TOP * (copy_u8_dutycycle + 1)) / 100;
			#endif
			local_enu_errorstate = ES_OK;
			OCR2 = local_u8_OCRvalue;
		break;
	}
	return local_enu_errorstate;
}
/*
 * PWM_Prog.c
 *
 * Created: 2/5/2024 6:52:49 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../../LIB/BitMath.h"
#include "../../../LIB/ErrorState.h"
#include "../../../LIB/stdTypes.h"

//Lower Level Libraries

//Private Libraries
#include "PWM_Config.h"
#include "PWM_Priv.h"

ES PWM_enuInit(){
	
	ES local_aenu_errorstate[4];
	ES local_enu_errorstate = ES_NOK;
	
	//Setting PWM-Mode
	#if (PWM_MODE == PWM_PHASE_CORRECT_8BIT_2X ||					\
		PWM_MODE == PWM_PHASE_CORRECT_9BIT_2X ||					\
		PWM_MODE == PWM_PHASE_CORRECT_10BIT_2X ||					\
		PWM_MODE == PWM_FAST_8BIT_2X ||								\
		PWM_MODE == PWM_FAST_9BIT_2X ||								\
		PWM_MODE == PWM_FAST_10BIT_2X ||							\
		PWM_MODE == PWM_PHASE_CORRECT_2X_BOTTOM_UPDATE ||			\
		PWM_MODE == PWM_PHASE_CORRECT_1X_BOTTOM_UPDATE ||			\
		PWM_MODE == PWM_PHASE_CORRECT_2X_TOP_UPDATE ||				\
		PWM_MODE == PWM_PHASE_CORRECT_1X_TOP_UPDATE ||				\
		PWM_MODE == PWM_FAST_2X_TOP_UPDATE ||						\
		PWM_MODE == PWM_FAST_1X_TOP_UPDATE)							
		
		local_aenu_errorstate[0] = ES_OK;
		//Since mode have 4 bits two in a register and other two in different register so we have to select each two bits and write to the registers at there positions
		TCCR1A |= (PWM_MODE & 0b11);						//masking first 2 bits two get them then write them at bit 0 and bit 1 
		u8 local_u8_upperbits = ((PWM_MODE >> 2) & (0b11)); //selecting the next two bits 
		TCCR1B |= (local_u8_upperbits << 3);                //writing the upper bits at there current location in TCCR1B
	#else
		local_aenu_errorstate[0] = ES_OUT_OF_RANGE;
		#error "PWM Mode Configurartion Error Invalid Mode"
	#endif
	
	//Setting PWM-Prescaler
	#if ((PWM_PRESCALLER >= 0) && (PWM_PRESCALLER <= 7))
		local_aenu_errorstate[1] = ES_OK;
		TCCR1B |= PWM_PRESCALLER;
	#else
		local_enu_errorstate[1] = ES_OUT_OF_RANGE;
		#error "PWM Configuration Error in PWM Prescaler"
	#endif
	
	//Setting OC1A and OC1B Output Mode (Normal-Inverted-NonInverted) 
	//1-OC1A
	#if ((PWM_OC1A_OUTPUT_MODE == PWM_OC1A_NORMAL) || (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_NON_INVERTED) || (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_INVERTED))
		local_aenu_errorstate[2] = ES_OK;
		TCCR1A |= (PWM_OC1A_OUTPUT_MODE << 6);  //writing the output pin mode of OC1A at its configuaration bits number 6 and 7 in TCCR1A
	#else
		local_enu_errorstate[2] = ES_OUT_OF_RANGE;
		#error "PWM Configuration Error in PWM_OC1A_OUTPUT_MODE"
	#endif
	//2-OC1B
	#if ((PWM_OC1B_OUTPUT_MODE == PWM_OC1B_NORMAL) || (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_NON_INVERTED) || (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_INVERTED))
		local_aenu_errorstate[3] = ES_OK;
		TCCR1A |= (PWM_OC1B_OUTPUT_MODE << 4);  //writing the output pin mode of OC1A at its configuaration bits number 6 and 7 in TCCR1A
	#else
		local_enu_errorstate[3] = ES_OUT_OF_RANGE;
		#error "PWM Configuration Error in PWM_OC1B_OUTPUT_MODE"
	#endif
	
	for(u8 i = 0; i < 4; i++){
		local_enu_errorstate = local_aenu_errorstate[i];
		if(local_enu_errorstate != ES_OK){
			return local_enu_errorstate;
		}
	}
	return local_enu_errorstate;
}

static ES enu_SetPwm(u8 copy_u8_channel, u8 copy_u8_dutycycle, u16 copy_u16_top){
	
	ES local_enu_errorstate = ES_OK;
	u16 local_u16_OCR1A_value;
	u16 local_u16_OCR1B_value;
	
	
	//If PWM Mode is Fast PWM
	#if ((PWM_MODE == PWM_FAST_8BIT_2X) || (PWM_MODE == PWM_FAST_9BIT_2X) || (PWM_MODE == PWM_FAST_10BIT_2X) || (PWM_MODE == PWM_FAST_1X_TOP_UPDATE) || (PWM_MODE == PWM_FAST_2X_TOP_UPDATE))
		switch(copy_u8_channel){
			case 'A':
				#if (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_NON_INVERTED)
					local_u16_OCR1A_value = ((copy_u8_dutycycle * ((u32)copy_u16_top)) / 100) - 1;
				#elif (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_INVERTED)
					local_u16_OCR1A_value = ((u32)copy_u16_top - ((copy_u8_dutycycle * ((u32)copy_u16_top + 1)) / 100));
				#else
					#warning "Cannot use this OC1A-Pin configuration to generate PWM"
				#endif
				OCR1AH = (local_u16_OCR1A_value >> 8);     //writing upper bits first
				OCR1AL = (local_u16_OCR1A_value);		  //writing lower bits
			break;
			
			case 'B':
				#if (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_NON_INVERTED)
					local_u16_OCR1B_value = ((copy_u8_dutycycle * ((u32)copy_u16_top)) / 100) - 1;
				#elif (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_INVERTED)
					local_u16_OCR1B_value = ((u32)copy_u16_top - ((copy_u8_dutycycle * ((u32)copy_u16_top + 1)) / 100));
				#else
					#warning "Cannot use this OC1B-Pin configuration to generate PWM"
				#endif
				OCR1BH = (local_u16_OCR1B_value >> 8);     //writing upper bits first
				OCR1BL = (local_u16_OCR1B_value);		  //writing lower bits
			break;	
		}
		
	//If PWM Mode is Phase Correct PWM
	#elif ((PWM_MODE == PWM_PHASE_CORRECT_8BIT_2X) || (PWM_MODE == PWM_PHASE_CORRECT_9BIT_2X) || (PWM_MODE == PWM_PHASE_CORRECT_10BIT_2X) || (PWM_MODE == PWM_PHASE_CORRECT_1X_BOTTOM_UPDATE) || (PWM_MODE == PWM_PHASE_CORRECT_2X_BOTTOM_UPDATE) || (PWM_MODE == PWM_PHASE_CORRECT_1X_TOP_UPDATE) || (PWM_MODE == PWM_PHASE_CORRECT_2X_TOP_UPDATE))
		switch(copy_u8_channel){
			case 'A':
				#if (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_NON_INVERTED)
					local_u16_OCR1A_value = ((copy_u8_dutycycle * (u32)copy_u16_top) / 100);
				#elif (PWM_OC1A_OUTPUT_MODE == PWM_OC1A_INVERTED)
					local_u16_OCR1A_value = ((u32)copy_u16_top * (copy_u8_dutycycle + 1)) / 100;
				#else
					#warning "Cannot use this OC1A-Pin configuration to generate PWM"
				#endif
				OCR1AH = (local_u16_OCR1A_value >> 8);     //writing upper bits first
				OCR1AL = (local_u16_OCR1A_value);		  //writing lower bits
			break;
			
			case 'B':
				#if (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_NON_INVERTED)
					local_u16_OCR1B_value = ((copy_u8_dutycycle * (u32)copy_u16_top) / 100);
				#elif (PWM_OC1B_OUTPUT_MODE == PWM_OC1B_INVERTED)
					local_u16_OCR1B_value = ((u32)copy_u16_top * (copy_u8_dutycycle + 1)) / 100;
				#else
					#warning "Cannot use this OC1B-Pin configuration to generate PWM"
				#endif
				OCR1BH = (local_u16_OCR1B_value >> 8);     //writing upper bits first
				OCR1BL = (local_u16_OCR1B_value);		  //writing lower bits
			break;
		}
	#endif
	
	return local_enu_errorstate;
}

ES PWM_enuGeneratePWM_FixedFreq(u8 copy_u8_dutycycle_A, u8 copy_u8_dutycycle_B){
	
	ES local_aenu_errorstate[2];
	ES local_enu_errorstate = ES_NOK;
	
	u8 local_enu_channelA = 'A';
	u8 local_enu_channelB = 'B';
	
	switch (PWM_MODE) {
		
		case PWM_FAST_8BIT_2X:
		case PWM_PHASE_CORRECT_8BIT_2X:
			//Set PWM for 8-bit mode
			local_aenu_errorstate[0] = enu_SetPwm(local_enu_channelA, copy_u8_dutycycle_A, PWM_TOP_256);
			local_aenu_errorstate[1] = enu_SetPwm(local_enu_channelB, copy_u8_dutycycle_B, PWM_TOP_256);
		break;

		case PWM_FAST_9BIT_2X:
		case PWM_PHASE_CORRECT_9BIT_2X:
			//Set PWM for 9-bit mode
			local_aenu_errorstate[0] = enu_SetPwm(local_enu_channelA, copy_u8_dutycycle_A, PWM_TOP_512);
			local_aenu_errorstate[1] = enu_SetPwm(local_enu_channelB, copy_u8_dutycycle_B, PWM_TOP_512);
		break;

		case PWM_FAST_10BIT_2X:
		case PWM_PHASE_CORRECT_10BIT_2X:
			//Set PWM for 10-bit mode
			local_aenu_errorstate[0] = enu_SetPwm(local_enu_channelA, copy_u8_dutycycle_A, PWM_TOP_1024);
			local_aenu_errorstate[1] = enu_SetPwm(local_enu_channelB, copy_u8_dutycycle_B, PWM_TOP_1024);
		break;

		default:
			local_aenu_errorstate[0] = ES_OUT_OF_RANGE;
			local_aenu_errorstate[1] = ES_OUT_OF_RANGE;
		break;
	}
	
	for(u8 i = 0; i < 2; i++){
		if(local_enu_errorstate == local_aenu_errorstate[i]){
			if(local_enu_errorstate != ES_OK){
				return local_enu_errorstate;
			}
		}
	}
	return local_enu_errorstate;
}

ES PWM_enuGeneratePWM_AdjustableFreq_1X(u8 copy_u8_dutycycle, u8 copy_u8_frequency){
	
	u16 local_u16_top;
	u16 local_u16_prescaler = PWM_PRESCALERS[PWM_PRESCALLER - 1];
	u8 local_enu_channelB = 'B';
	
	//Here the top of TCNT1 is OCR1A
	#if ((PWM_MODE == PWM_PHASE_CORRECT_1X_BOTTOM_UPDATE) || (PWM_MODE == PWM_PHASE_CORRECT_1X_TOP_UPDATE))
		local_u16_top = (F_CPU / (2 * copy_u8_frequency * local_u16_prescaler));
		OCR1AH = (local_u16_top >> 8);
		OCR1AL = (u8)local_u16_top;
		enu_SetPwm(local_enu_channelB, copy_u8_dutycycle, local_u16_top);
	
	#elif ((PWM_MODE == PWM_FAST_1X_TOP_UPDATE))
		local_u16_top = (F_CPU / (copy_u8_frequency * local_u16_prescaler));
		OCR1AH = (local_u16_top >> 8);
		OCR1AL = (local_u16_top);
		enu_SetPwm(local_enu_channelB, copy_u8_dutycycle, local_u16_top);
	#endif
	return ES_OK;
}

ES PWM_enuGeneratePWM_AdjustableFreq_2X(u8 copy_u8_dutycycle_A, u8 copy_u8_dutycycle_B, u8 copy_u8_frequency){
	u16 local_u16_top;
	u16 local_u16_prescaler = PWM_PRESCALERS[PWM_PRESCALLER - 1];
	u8 local_enu_channelA = 'A';
	u8 local_enu_channelB = 'B';
	
	//Here the top of TCNT1 is ICR1
	#if ((PWM_MODE == PWM_PHASE_CORRECT_2X_BOTTOM_UPDATE) || (PWM_MODE == PWM_PHASE_CORRECT_2X_TOP_UPDATE))
		local_u16_top = (F_CPU / (2 * copy_u8_frequency * local_u16_prescaler));
		ICR1H = (local_u16_top >> 8);
		ICR1L = (u8)local_u16_top;
		enu_SetPwm(local_enu_channelA, copy_u8_dutycycle_A, local_u16_top);
		enu_SetPwm(local_enu_channelB, copy_u8_dutycycle_B, local_u16_top);
	
	#elif ((PWM_MODE == PWM_FAST_2X_TOP_UPDATE))
		local_u16_top = (F_CPU / (copy_u8_frequency * local_u16_prescaler));
		ICR1H = (local_u16_top >> 8);
		ICR1L = (u8)local_u16_top;
		enu_SetPwm(local_enu_channelA, copy_u8_dutycycle_A, local_u16_top);
		enu_SetPwm(local_enu_channelB, copy_u8_dutycycle_B, local_u16_top);
	#endif
	return ES_OK;
}
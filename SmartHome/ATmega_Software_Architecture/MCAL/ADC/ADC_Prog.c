/*
 * ADC_Prog.c
 *
 * Created: 10/18/2023 8:03:46 PM
 *  Author: Beast
 */ 

//Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

//Lower Level Libraries

//Private Libraries
#include "ADC_Config.h"
#include "ADC_Priv.h"

static void (*local_pvid_fun)(void*) = NULL;			// ptr to function with parameters
static void * local_pvid_parameter;


ES ADC_enuInit(){
	
	//1-Set Prescale Value
	#if (ADC_PRESCALER >= ADC_PRESC_2 && ADC_PRESCALER <= ADC_PRESC_64)
		// Clear prescaler bits(ADPS2-->ADPS0)
		ADCSRA &= (0XF8);
		ADCSRA |= ADC_PRESCALER;
	#else
		#error "ADC_PRESCALER configuration is out of range"
	#endif
	//-----------------------------------------------------------------------------------
	
	//2-Set Volt Reference
	// Clear volt reference bits
	ADMUX &= (0X3F);
	#if (ADC_VOLT_REFERENCE == ADC_AREF)
		ClearBit(ADMUX, 7);
		ClearBit(ADMUX, 6);
	#elif (ADC_VOLT_REFERENCE == ADC_AVCC)
		ClearBit(ADMUX, 7);
		SetBit(ADMUX, 6);
	#elif (ADC_VOLT_REFERENCE == ADC_INTERNAL_VOLT)
		SetBit(ADMUX, 7);
		SetBit(ADMUX, 6);
	#else
		#error "ADC_VOLT_REFERENCE configuration is out of range"
	#endif
	//-----------------------------------------------------------------------------------
	
	//3-Set Adjustment Level (read mode)
	#if (ADC_ADJUSTMENT_LEVEL == ADC_LEFT_ADJ)
		SetBit(ADMUX, 5);
	#elif (ADC_ADJUSTMENT_LEVEL == ADC_RIGHT_ADJ)
		ClearBit(ADMUX, 5);
	#else
		#error "ADC_ADJUSTMENT_LEVEL configuration is out of range"
	#endif
	//-----------------------------------------------------------------------------------
	
	//4-Set Trigger Source
	#if (ADC_TRIGGER_SOURCE == ADC_SINGLE_CONVERSION)
		//disable auto trigger
		ClearBit(ADCSRA, 5);
	#elif (ADC_TRIGGER_SOURCE >= ADC_FREE_RUNNING && ADC_TRIGGER_SOURCE <= ADC_TIMER_OVERFLOW)
		ClearBit(ADCSRA, 5);
		
		ClearBit(SFIOR, 5);		// clear trigger source bits
		ClearBit(SFIOR, 6);
		ClearBit(SFIOR, 7);
		
		SFIOR |= ((ADC_TRIGGER_SOURCE - 10) << 5);
		//enable auto trigger
		SetBit(ADCSRA, 5);
	#else
		#error "ADC_TRIGGER_SOURCE configuration is out of range"
	#endif
	//-----------------------------------------------------------------------------------
	
	//5-Set Channel
	//clear channel bits
	#if (ADC_INITIALIZATION_CHANNEL >= ADC0 && ADC_INITIALIZATION_CHANNEL <= ADC5_ADC2_1X)
		ADMUX &= (0xE0);
		ADMUX |= ADC_INITIALIZATION_CHANNEL;
	#else
		#error "ACC_INITIALIZATION_CHANNEL configuration is out of range"
	#endif
	//-----------------------------------------------------------------------------------
	
	//6-Set Interrupt Mode
	#if ADC_INTERRUPT_MODE == ADC_INTERRUPT
		SetBit(ADCSRA, 3);
	#elif ADC_INTERRUPT_MODE == ADC_POLLING
		ClearBit(ADCSRA, 3);
	#else
		#error "ADC interrupt mode configuration is invalid"
	#endif
	//-----------------------------------------------------------------------------------
	
	//7-Enable ADC
	SetBit(ADCSRA, 7);
	
	return ES_OK;	
}

ES ADC_enuReadPolling(u8 copy_u8_channel, u16 *copy_u8_value){
	
	if(copy_u8_value != NULL){
	
		//clear the flag
		ClearBit(ADCSRA, 4);
	
		//start conversion
		SetBit(ADCSRA, 6);
		
		if(copy_u8_channel >= ADC0 && copy_u8_channel <= ADC5_ADC2_1X){
			ADMUX &= (0xE0);
			ADMUX |= ADC_INITIALIZATION_CHANNEL;          // set the channel
			
			while(GetBit(ADCSRA, 4) == False);            // Wait till the flag is raised
			
			#if (ADC_ADJUSTMENT_LEVEL == ADC_LEFT_ADJ)
				*copy_u8_value = (ADCL >> 6);			// to read the first two bit in its actual form
				*copy_u8_value |= ((u16)ADCH << 2);
			#elif (ADC_ADJUSTMENT_LEVEL == ADC_RIGHT_ADJ)
				*copy_u8_value = ADCL;			
				*copy_u8_value |= ((u16)ADCH << 8);
			#endif
			
			ClearBit(ADCSRA, 4);	
		}
	}
	return ES_OK;
}

ES ADC_enuStartConversion(){
	
	SetBit(ADCSRA, 6);
	
	return ES_OK;
}

ES ADC_enuEnableInterrupt(){
	
	SetBit(ADCSRA, 3);
	
	return ES_OK;
}

ES ADC_enuDisableInterrupt(){
	
	ClearBit(ADCSRA, 3);
	
	return ES_OK;
}

ES ADC_enuSetChannel(u8 copy_u8_channel){
	
	//clear channel bits
	if(copy_u8_channel >= ADC0 && copy_u8_channel <= ADC5_ADC2_1X){
		ADMUX &= (0xE0);
		ADMUX |= copy_u8_channel;
	}
	
	return ES_OK;
}

ES ADC_enuTriggerSource(u8 copy_u8_source){
	
	if(copy_u8_source == ADC_SINGLE_CONVERSION){
		//disable auto trigger
		ClearBit(ADCSRA, 5);
	}else if(copy_u8_source >= ADC_FREE_RUNNING && copy_u8_source <= ADC_TIMER_OVERFLOW){
		ClearBit(ADCSRA, 5);
	
		ClearBit(SFIOR, 5);		// clear trigger source bits
		ClearBit(SFIOR, 6);
		ClearBit(SFIOR, 7);
	
		SFIOR |= ((copy_u8_source - 10) << 5);
		//enable auto trigger
		SetBit(ADCSRA, 5);
	}else{}
		
	return ES_OK;
}

ES ADC_enuClearTrigger(){
	
	ClearBit(ADCSRA, 5);
	
	return ES_OK;
}

ES ADC_enuRead(u16 *copy_u16_value){
	
	if(copy_u16_value != NULL){
		#if (ADC_ADJUSTMENT_LEVEL == ADC_LEFT_ADJ)
			*copy_u16_value = (ADCL >> 6);			// to read the first two bit in its actual form
			*copy_u16_value |= ((u16)ADCH << 2);
			#warning "ADC_enuRead is not working with full efficiency in left adjusted mode"
		#elif (ADC_ADJUSTMENT_LEVEL == ADC_RIGHT_ADJ)
			*copy_u16_value = ADCL;
			*copy_u16_value |= ((u16)ADCH << 8);
		#endif
	}
	return ES_OK;
}

ES ADC_enuReadHigh(u16 *copy_u16_value){
	
	if(copy_u16_value != NULL){
		#if (ADC_ADJUSTMENT_LEVEL == ADC_LEFT_ADJ)
			*copy_u16_value = ADCH;
		#elif (ADC_ADJUSTMENT_LEVEL == ADC_RIGHT_ADJ)
			*copy_u16_value = ADCL;
			*copy_u16_value |= ((u16)ADCH << 8);
			#warning "ADC_enuReadHigh is not working with full efficiency in right adjusted mode"
		#endif
	}
	
	return ES_OK;
}

ES ADC_enuSetCallBack(void (*ptr_to_fun)(void*), void * copy_pvid_parameter){
	
	if(ptr_to_fun != NULL){
		local_pvid_fun = ptr_to_fun;
	}
	
	if(copy_pvid_parameter != NULL){
		local_pvid_parameter = copy_pvid_parameter;
	}
	
	return ES_OK;
}

//ADC Interrupt
void __vector_16()__attribute__((signal));

void __vector_16(){

	if(local_pvid_fun != NULL){
		local_pvid_fun(local_pvid_parameter);
	}
}
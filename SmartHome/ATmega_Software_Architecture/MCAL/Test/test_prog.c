/*
 * TIMER0_prog.c
 *
 *  Created on: Feb 12, 2024
 *      Author: Omar Gamal El-Kady
 */

// public lib
#include "../../LIB/stdTypes.h"
#include "../../LIB/errorState.h"
#include "../../LIB/bitMath.h"

// lower lib

// my own lib
#include "TIMER0_priv.h"
#include "TIMER0_config.h"
#include "../DIO/DIO_int.h"

static pfuncISRFunc_t LOC_pISRFunc;

static vidptr_t LOC_pISRFuncParameters;

static u16 LOC_u16OVF , LOC_u16OVFCounts , LOC_u16OCRValue , LOC_u16CTC , LOC_u16CTCCounts;
static u8 LOC_u8Preload;

ES_t TIEMR0_enuInit(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetWaveGenMode(TIMER0_WG_MODE);
	enuSetPrescaler(TIMER0_PRESCALER);
	enuSetCompNonPWMMode(TIMER0_COM_MODE_NON_PWM);
	enuSetInterruptMode(TIMER0_INTERRUPT_MODE);

	return Local_enuErrorState;
}

ES_t TIMER0_enuSetWaveGenMode(u8 Copy_u8WaveGenMode){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetWaveGenMode(Copy_u8WaveGenMode);

	return Local_enuErrorState;
}

ES_t TIMER0_enuSetPrescaler(u8 Copy_u8Prescaler){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetPrescaler(Copy_u8Prescaler);

	return Local_enuErrorState;
}

ES_t TIMER0_enuSetCompNonPWMMode(u8 Copy_u8COMNonPWMMode){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetCompNonPWMMode(Copy_u8COMNonPWMMode);

	return Local_enuErrorState;
}

ES_t TIMER0_SetOCRValue(u8 Copy_u8Cycles){

	ES_t Local_enuErrorState = ES_OK;

	//TIMER0_TCNT0 = 0;
	TIMER0_OCR0 = Copy_u8Cycles;
	TIMER0_TCNT0 = 0x01;

	return Local_enuErrorState;
}

ES_t TIMER0_enuGetTimerCounts(u8 * Copy_pu8Counts){

	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_pu8Counts != NULL){

		*Copy_pu8Counts = TIMER0_TCNT0;
		Local_enuErrorState = ES_OK;
	}
	else{

		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}

ES_t TIMER0_enuOVFInterruptEnable(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetInterruptMode(TIMER0_OVFINT);

	return Local_enuErrorState;
}

ES_t TIMER0_enuOVFInterruptDisable(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetInterruptMode(TIMER0_POLLING);

	return Local_enuErrorState;
}

ES_t TIMER0_enuCOMInterruptEnable(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetInterruptMode(TIMER0_COMINT);

	return Local_enuErrorState;
}

ES_t TIMER0_enuCOMInterruptDisable(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetInterruptMode(TIMER0_POLLING);

	return Local_enuErrorState;
}

ES_t TIMER0_enuInterruptDisable(void){

	ES_t Local_enuErrorState = ES_NOK;

	enuSetInterruptMode(TIMER0_POLLING);

	return Local_enuErrorState;
}

ES_t TIMER0_enuSyncDelayms(u16 Copy_u16DelayTime){

	ES_t Local_enuErrorState = ES_NOK;


	f32 Local_f32OVFTime = (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1] * 256.0) / (16000);

	if(Local_f32OVFTime <= Copy_u16DelayTime){

		//DIO_enuSetPinDirection(DIO_GROUP_D,DIO_PIN_7,DIO_OUTPUT);
		//DIO_enuSetPinValue(DIO_GROUP_D,DIO_PIN_7,DIO_HIGH);

		u16 Local_u16CyclesNum = (Copy_u16DelayTime * 16000UL) / (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1]);

		LOC_u16OVF = (Local_u16CyclesNum + 255) / (256);

		u8 Local_u8Preload = 256 - (Local_u16CyclesNum % 256);

		LOC_u8Preload = Local_u8Preload;
		LOC_u16OVFCounts = LOC_u16OVF;
		TIMER0_TCNT0 = Local_u8Preload;

		while(LOC_u16OVF != 0){

			//enuSetPrescaler(TIMER_INT0,TIMER_astrTimerConfig[Copy_u8IntPin].timerclockspeed);

			while((TIMER0_TIFR & 1) != 1);

			SET_BIT(TIMER0_TIFR,0);
			//TIMER0_TIFR |= (1<<0);

			LOC_u16OVF--;
		}

		enuSetInterruptMode(TIMER0_OVFINT);

		Local_enuErrorState = ES_OK;


	}
	else if(Local_f32OVFTime > Copy_u16DelayTime){

		enuSetWaveGenMode(TIMER0_WG_MODE_CTC);

		u16 Local_u16CyclesNum = (Copy_u16DelayTime * 16000UL) / (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1]);

		LOC_u16OVF = 1;
		TIMER0_OCR0 = Local_u16CyclesNum;

		while(LOC_u16OVF != 0){

			//enuSetPrescaler(TIMER_INT0,TIMER_astrTimerConfig[Copy_u8IntPin].timerclockspeed);

			while(((TIMER0_TIFR>>1) & 1) != 1);

			SET_BIT(TIMER0_TIFR,1);
			//TIMER0_TIFR |= (1<<0);

			LOC_u16OVF--;
		}

		enuSetInterruptMode(TIMER0_COMINT);

		Local_enuErrorState = ES_OK;
	}
	else{}


	return Local_enuErrorState;

}

ES_t TIMER0_enuAsyncDelayms(u16 Copy_u16DelayTime){

	ES_t Local_enuErrorState = ES_NOK;

	Local_enuErrorState = enuCalculateOVFNOCR(Copy_u16DelayTime);

	return Local_enuErrorState;
}

ES_t TIMER0_enuSetCallBack(void (* Copy_pfuncAppFunc)(void*),void * Copy_pvidParameter){

	ES_t Local_enuErrorState = ES_NOK;

	if((Copy_pfuncAppFunc != NULL) && (Copy_pvidParameter != NULL)){

		LOC_pISRFunc = Copy_pfuncAppFunc;
		LOC_pISRFuncParameters = Copy_pvidParameter;
		Local_enuErrorState = ES_OK;
	}
	else{

		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState;
}


// Static Func Implementation

ES_t enuSetWaveGenMode(u8 Copy_u8WaveGenMode){

	ES_t Local_enuErrorState = ES_NOK;

	CLEAR_BIT(TIMER0_TCCR0 , 6);
	CLEAR_BIT(TIMER0_TCCR0 , 3);

	if(Copy_u8WaveGenMode == TIMER0_WG_MODE_NORMAL){

		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8WaveGenMode == TIMER0_WG_MODE_PWM){

		SET_BIT(TIMER0_TCCR0 , 6);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8WaveGenMode == TIMER0_WG_MODE_CTC){

		SET_BIT(TIMER0_TCCR0 , 3);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8WaveGenMode == TIMER0_WG_MODE_FASTPWM){

		SET_BIT(TIMER0_TCCR0 , 6);
		SET_BIT(TIMER0_TCCR0 , 3);
		Local_enuErrorState = ES_OK;
	}
	else{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;

}

ES_t enuSetPrescaler(u8 Copy_u8Prescaler){

	ES_t Local_enuErrorState = ES_NOK;

	CLEAR_BIT(TIMER0_TCCR0,0);
	CLEAR_BIT(TIMER0_TCCR0,1);
	CLEAR_BIT(TIMER0_TCCR0,2);

	if(Copy_u8Prescaler == TIMER0_NO_CLK_SOURCE){

		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_PRESCALER_1){

		SET_BIT(TIMER0_TCCR0,0);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_PRESCALER_8){

		SET_BIT(TIMER0_TCCR0,1);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_PRESCALER_64){

		SET_BIT(TIMER0_TCCR0,0);
		SET_BIT(TIMER0_TCCR0,1);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_PRESCALER_256){

		SET_BIT(TIMER0_TCCR0,2);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_PRESCALER_1024){

		SET_BIT(TIMER0_TCCR0,0);
		SET_BIT(TIMER0_TCCR0,2);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_EXT_CLK_FALLING){

		SET_BIT(TIMER0_TCCR0,1);
		SET_BIT(TIMER0_TCCR0,2);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8Prescaler == TIMER0_EXT_CLK_RISING){

		SET_BIT(TIMER0_TCCR0,0);
		SET_BIT(TIMER0_TCCR0,1);
		SET_BIT(TIMER0_TCCR0,2);
		Local_enuErrorState = ES_OK;
	}
	else{

		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t enuSetCompNonPWMMode(u8 Copy_u8COMNonPWMMode){

	ES_t Local_enuErrorState = ES_NOK;

	CLEAR_BIT(TIMER0_TCCR0,4);
	CLEAR_BIT(TIMER0_TCCR0,5);

	if(Copy_u8COMNonPWMMode == TIMER0_COM_MODE_NON_PWM_NORMAL){

		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8COMNonPWMMode == TIMER0_COM_MODE_NON_PWM_TOGGLE){

		SET_BIT(TIMER0_TCCR0,4);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8COMNonPWMMode == TIMER0_COM_MODE_NON_PWM_CLEAR){

		SET_BIT(TIMER0_TCCR0,5);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8COMNonPWMMode == TIMER0_COM_MODE_NON_PWM_SET){

		SET_BIT(TIMER0_TCCR0,4);
		SET_BIT(TIMER0_TCCR0,5);
		Local_enuErrorState = ES_OK;
	}
	else{

		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t enuSetInterruptMode(u8 Copy_u8InterruptMode){

	ES_t Local_enuErrorState = ES_NOK;

	CLEAR_BIT(TIMER0_TIMSK,0);
	CLEAR_BIT(TIMER0_TIMSK,1);

	if(Copy_u8InterruptMode == TIMER0_POLLING){

		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8InterruptMode == TIMER0_OVFINT){

		SET_BIT(TIMER0_TIMSK,0);
		Local_enuErrorState = ES_OK;
	}
	else if(Copy_u8InterruptMode == TIMER0_COMINT){

		SET_BIT(TIMER0_TIMSK,1);
		Local_enuErrorState = ES_OK;
	}
	else{
		Local_enuErrorState = ES_OUT_OF_RANGE;
	}

	return Local_enuErrorState;
}

ES_t enuCalculateOVFNOCR(u16 Copy_u16DelayTime){

	ES_t Local_enuErrorState = ES_NOK;

	if(TIMER0_WG_MODE == TIMER0_WG_MODE_NORMAL){

		f32 Local_f32OVFTime = (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1] * 256.0) / (16000);

		if(Local_f32OVFTime <= Copy_u16DelayTime){

			DIO_enuSetPinDirection(DIO_GROUP_D,DIO_PIN_7,DIO_OUTPUT);
			DIO_enuSetPinValue(DIO_GROUP_D,DIO_PIN_7,DIO_HIGH);

			u16 Local_u16CyclesNum = (Copy_u16DelayTime * 16000UL) / (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1]);

			LOC_u16OVF = (Local_u16CyclesNum + 255) / (256);

			u8 Local_u8Preload = 256 - (Local_u16CyclesNum % 256);

			LOC_u8Preload = Local_u8Preload;
			LOC_u16OVFCounts = LOC_u16OVF;
			TIMER0_TCNT0 = Local_u8Preload;

			enuSetInterruptMode(TIMER0_OVFINT);

			Local_enuErrorState = ES_OK;


		}
		else if(Local_f32OVFTime > Copy_u16DelayTime){

			enuSetWaveGenMode(TIMER0_WG_MODE_CTC);

			u16 Local_u16CyclesNum = (Copy_u16DelayTime * 16000UL) / (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1]);

			LOC_u16OVF = 1;
			TIMER0_OCR0 = Local_u16CyclesNum;

			enuSetInterruptMode(TIMER0_COMINT);

			Local_enuErrorState = ES_OK;
		}
		else{}

	}
	else if(TIMER0_WG_MODE == TIMER0_WG_MODE_CTC){

		f32 Local_f32OVFTime = (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1] * 256.0) / (16000);

		LOC_u16OCRValue = ((u8)Local_f32OVFTime * 16000) / (LOC_au16PrescalerValues[TIMER0_PRESCALER - 1]);

		LOC_u16CTC = Copy_u16DelayTime / (u8)Local_f32OVFTime;
		LOC_u16CTCCounts = LOC_u16CTC;

		TIMER0_OCR0 = (u8)LOC_u16OCRValue;

		enuSetInterruptMode(TIMER0_COMINT);

		Local_enuErrorState = ES_OK;

	}
	else{}

	return Local_enuErrorState;

}

void __vector_10(void)__attribute__((signal));
void __vector_11(void)__attribute__((signal));

void __vector_10(void){

	LOC_u16CTC--;
	if(LOC_u16CTC == 0){

		LOC_pISRFunc(LOC_pISRFuncParameters);
		LOC_u16CTC = LOC_u16CTCCounts;

	}
}

void __vector_11(void){

	LOC_u16OVF--;
	if(LOC_u16OVF == 0){

		LOC_pISRFunc(LOC_pISRFuncParameters);
		TIMER0_TCNT0 = LOC_u8Preload;
		LOC_u16OVF = LOC_u16OVFCounts;
	}
}

/*
 * EXTI_Config.c
 *
 * Created: 10/8/2023 9:24:52 PM
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

u8 local_u8_EXTI_max_pin_no = EXTI_MAX_PIN_NO;

EXTI_Str EXTI_astr_PIN_CONFIG[EXTI_MAX_PIN_NO] = {
	
	{EXTI_INTERRUPT_PIN0, EXTI_INTERRUPT_MODE, EXTI_FALLING_EDGE},
	{EXTI_INTERRUPT_PIN1, EXTI_INTERRUPT_MODE, EXTI_ANY_LOGICAL_CHANGE},	
	{EXTI_INTERRUPT_PIN2, EXTI_INTERRUPT_MODE, EXTI_ANY_LOGICAL_CHANGE}
		
};
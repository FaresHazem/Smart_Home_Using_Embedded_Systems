/*
 * Switch_Config.c
 *
 * Created: 9/24/2023 7:39:38 PM
 *  Author: Beast
 */ 

// Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

// Lower Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"

// Switch Private Libraries
#include "Switch_Config.h"
#include "Switch_Priv.h"

Switch Switch_astr_switch_config[SWITCH_MAX_SIZE] = {
	
	{DIO_GROUP_A, DIO_PIN_1, DIO_FLOAT},       // sw0
	{DIO_GROUP_D, DIO_PIN_6, DIO_PULL_UP},     // sw1
	{DIO_GROUP_D, DIO_PIN_2, DIO_FLOAT}        // sw2
	
};

u8 local_u8_switch_max = SWITCH_MAX_SIZE;
/*
 * LED_Config.c
 *
 * Created: 9/25/2023 7:12:00 PM
 *  Author: Beast
 */

#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

#include "../../MCAL/DIO/DIO_INT.h"

#include "LED_Config.h"
#include "LED_Priv.h"
 
LED local_astr_led_config[LED_MAX_SIZE] = {
	
	{DIO_GROUP_C, DIO_PIN_2},              // Led 0
	{DIO_GROUP_C, DIO_PIN_7},              // Led 1
	{DIO_GROUP_D, DIO_PIN_3},              // Led 2
	{DIO_GROUP_C, DIO_PIN_1},
	{DIO_GROUP_C, DIO_PIN_0}
	
};

u8 local_u8_led_max = LED_MAX_SIZE;
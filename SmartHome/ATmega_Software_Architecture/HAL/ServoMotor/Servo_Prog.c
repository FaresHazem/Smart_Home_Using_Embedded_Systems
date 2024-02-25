/*
 * Servo_Prog.c
 *
 * Created: 2/20/2024 7:22:52 PM
 *  Author: Beast
 */ 

/*
//Public Libraries
#include "../../LIB/BitMath.h"
#include "../../LIB/ErrorState.h"
#include "../../LIB/stdTypes.h"

//Lower Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"
#include "../../MCAL/Timers/Timer0/Timer0_Interface.h"
#include "../../MCAL/Timers/Timer1_PWM/PWM_Interface.h"

//Private Libraries
#include "Servo_Config.h"
#include "Servo_Priv.h"

extern u8 local_u8_servo_max;
extern SERVO local_astr_servo_config[];

ES Servo_enuInit(){
	ES local_aenu_errorstates[2];
	ES local_enu_errorstate = ES_NOK;
	
	Timer0_enuInit();
	PWM_enuInit();
	
	for(u8 i = 0; i < local_u8_servo_max; i++){
		
		//Set Servo Pin Configurations
		local_aenu_errorstates[0] = DIO_enuSetPinMode(local_astr_servo_config[i].servo_grp, local_astr_servo_config[i].servo_pin, DIO_OUTPUT);
		local_aenu_errorstates[1] = DIO_enuSetPinValue(local_astr_servo_config[i].servo_grp, local_astr_servo_config[i].servo_pin, local_astr_servo_config[i].servo_state);

		for(u8 i = 0; i < 2; i++){
			if(local_aenu_errorstates[i] != ES_OK){
				local_enu_errorstate = local_aenu_errorstates[i];
				break;
			}else{
				local_enu_errorstate = ES_OK;
			}
		}
	}
	
	return local_enu_errorstate;
}


ES Servo_enuWrite(u8 copy_u8_servo_pin_id, u8 copy_u8_angle){
	
	u8 local_u8_servo_group = local_astr_servo_config[copy_u8_servo_pin_id].servo_grp;
	u8 local_u8_servo_pin = local_astr_servo_config[copy_u8_servo_pin_id].servo_pin;

	#if ((local_u8_servo_group == DIO_GROUP_B) || (local_u8_servo_group == DIO_GROUP_D))
		#if ((local_u8_servo_pin == DIO_PIN_3) || (local_u8_servo_pin == DIO_PIN_4) || (local_u8_servo_pin == DIO_PIN_5))
		
		#else
			#warning "The servo is connected to a pin that does not generate PWM"
		#endif
	#else
		#warning "The servo is connected to a group that does not have PWM pins"
	#endif
	
	return ES_OK;
}
*/
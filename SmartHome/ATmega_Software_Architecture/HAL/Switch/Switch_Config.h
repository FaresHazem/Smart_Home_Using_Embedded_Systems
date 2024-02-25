/*
 * Switch_Config.h
 *
 * Created: 9/24/2023 5:14:00 PM
 *  Author: Beast
 */ 


#ifndef SWITCH_CONFIG_H_
#define SWITCH_CONFIG_H_

typedef struct{
	
	u8 sw_grp;
	u8 sw_pin;
	u8 sw_state;
	
}Switch;

#define SWITCH_MAX_SIZE   3



#endif /* SWITCH_CONFIG_H_ */
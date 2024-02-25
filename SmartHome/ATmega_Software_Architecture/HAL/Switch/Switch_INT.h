/*
 * Switch_INT.h
 *
 * Created: 9/24/2023 5:14:21 PM
 *  Author: Beast
 */ 


#ifndef SWITCH_INT_H_
#define SWITCH_INT_H_

ES Switch_enu_Init(void);                                // Initializing all switches

ES Switch_enu_GetSwitchState(u8 copy_u8_switch_id, u8 *copy_pu8_pressed);

#define SWITCH_BUTTON_0      0
#define SWITCH_BUTTON_1      1
#define SWITCH_BUTTON_2      2


#endif /* SWITCH_INT_H_ */
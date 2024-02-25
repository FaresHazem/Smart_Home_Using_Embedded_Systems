/*
 * KeyPad_Interface.h
 *
 * Created: 10/3/2023 7:08:45 PM
 *  Author: Beast
 */ 


#ifndef KEYPAD_INTERFACE_H_
#define KEYPAD_INTERFACE_H_

ES KEYPAD_enuInit(void);

ES KEYPAD_enuRead(u8 *copy_u8_value);

#define KEYPAD_NOT_PRESSED		0xFF

#endif /* KEYPAD_INTERFACE_H_ */
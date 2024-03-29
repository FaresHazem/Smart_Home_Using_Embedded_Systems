/*
 * KeyPad_Priv.h
 *
 * Created: 10/3/2023 7:08:31 PM
 *  Author: Beast
 */ 


#ifndef KEYPAD_PRIV_H_
#define KEYPAD_PRIV_H_


#define KEYPAD_ROWS    {\
			{KEYPAD_ROW1_GP, KEYPAD_ROW1_PIN},\
			{KEYPAD_ROW2_GP, KEYPAD_ROW2_PIN},\
			{KEYPAD_ROW3_GP, KEYPAD_ROW3_PIN},\
			{KEYPAD_ROW4_GP, KEYPAD_ROW4_PIN}}
				
				
#define KEYPAD_COLUMNS		{\
			{KEYPAD_COL1_GP, KEYPAD_COL1_PIN},\
			{KEYPAD_COL2_GP, KEYPAD_COL2_PIN},\
			{KEYPAD_COL3_GP, KEYPAD_COL3_PIN},\
			{KEYPAD_COL4_GP, KEYPAD_COL4_PIN}}

#define KEYPAD_NOT_PRESSED		0xFF

#endif /* KEYPAD_PRIV_H_ */
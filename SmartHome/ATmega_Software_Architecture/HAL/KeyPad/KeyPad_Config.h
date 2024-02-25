/*
 * KeyPad_Config.h
 *
 * Created: 10/3/2023 7:08:15 PM
 *  Author: Beast
 */ 


#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define KEYPAD_ROW1_GP					DIO_GROUP_B
#define KEYPAD_ROW1_PIN					DIO_PIN_4

#define KEYPAD_ROW2_GP					DIO_GROUP_B
#define KEYPAD_ROW2_PIN					DIO_PIN_5

#define KEYPAD_ROW3_GP					DIO_GROUP_B
#define KEYPAD_ROW3_PIN					DIO_PIN_6

#define KEYPAD_ROW4_GP					DIO_GROUP_B
#define KEYPAD_ROW4_PIN					DIO_PIN_7

#define KEYPAD_COL1_GP					DIO_GROUP_C
#define KEYPAD_COL1_PIN					DIO_PIN_3

#define KEYPAD_COL2_GP					DIO_GROUP_C
#define KEYPAD_COL2_PIN					DIO_PIN_4

#define KEYPAD_COL3_GP					DIO_GROUP_C
#define KEYPAD_COL3_PIN					DIO_PIN_5

#define KEYPAD_COL4_GP					DIO_GROUP_C
#define KEYPAD_COL4_PIN					DIO_PIN_6

#define KEYPAD_VALUES		{\
						{'1', '2', '3', 'A'},\
						{'4', '5', '6', 'B'},\
						{'7', '8', '9', 'C'},\
						{'*', '0', '#', 'D'}}

#endif /* KEYPAD_CONFIG_H_ */
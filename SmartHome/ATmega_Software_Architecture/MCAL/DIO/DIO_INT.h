/*
 * DIO_INT.h
 *
 * Created: 9/22/2023 6:36:43 PM
 *  Author: Beast
 */ 


#ifndef DIO_INT_H_
#define DIO_INT_H_


ES DIO_enuInit(void);															// Function DIO_initialization return type enum

ES DIO_enuSetPinMode(u8 copy_u8_groupID, u8 copy_bitno, u8 copy_bitmode);           // Function DIO_SetPinMode return type enum  (Input/Output)

ES DIO_enuSetPinValue(u8 copy_u8_groupID, u8 copy_bitno, u8 copy_bitvalue);			// Function DIO_SetPinValue return type enum  (High/Low)

ES DIO_enuToggleBit(u8 copy_u8_groupID, u8 copy_bitno);                             // Function DIO_ToggleBit return type enum (toggles the bit)

ES DIO_enuGetBit(u8 copy_u8_groupID, u8 copy_bitno, u8 * copy_pu8_value);           // Function DIO_GetBit return type enum (Get the current bit value at a current pin)

ES DIO_enuReadBit(u8 copy_u8_groupID, u8 copy_u8_bitNo, u8 * copy_pu8_value);

#define DIO_GROUP_A  0
#define DIO_GROUP_B  1
#define DIO_GROUP_C  2
#define DIO_GROUP_D  3

#define DIO_PIN_0  0
#define DIO_PIN_1  1
#define DIO_PIN_2  2
#define DIO_PIN_3  3
#define DIO_PIN_4  4
#define DIO_PIN_5  5
#define DIO_PIN_6  6
#define DIO_PIN_7  7

#define DIO_HIGH  1
#define DIO_LOW   0

#define DIO_INPUT   0
#define DIO_OUTPUT  1

#define DIO_FLOAT    0
#define DIO_PULL_UP  1


#endif /* DIO_INT_H_ */
/*
 * DIO_Priv.h
 *
 * Created: 9/22/2023 6:35:53 PM
 *  Author: Beast
 */ 


#ifndef DIO_PRIV_H_
#define DIO_PRIV_H_

//casting and dereferencing the register at this address to get the register 0000 0000

#define DDRA_  (*(volatile u8*)0x3A)
#define PORTA_  (*(volatile u8*)0x3B)
#define PINA_  (*(volatile u8*)0x39)

#define DDRB_  (*(volatile u8*)0x37)
#define PORTB_  (*(volatile u8*)0x38)
#define PINB_  (*(volatile u8*)0x36)

#define DDRC_  (*(volatile u8*)0x34)
#define PORTC_  (*(volatile u8*)0x35)
#define PINC_  (*(volatile u8*)0x33)

#define DDRD_  (*(volatile u8*)0x31)
#define PORTD_  (*(volatile u8*)0x32)
#define PIND_  (*(volatile u8*)0x30)

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

#endif /* DIO_PRIV_H_ */
/*
 * Ports.h
 *
 * Created: 9/16/2023 11:29:45 PM
 *  Author: Beast
 */ 


#ifndef PORTS_H_
#define PORTS_H_

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


#endif /* PORTS_H_ */
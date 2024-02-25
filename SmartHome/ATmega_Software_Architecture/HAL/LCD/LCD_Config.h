/*
 * LCD_Config.h
 *
 * Created: 9/26/2023 6:26:28 PM
 *  Author: Beast
 */ 


#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

//LCD Mode
//************8-BIT************
//OR
//************4-BIT************
#define LCD_MODE  FOUR_BIT


//Control Pins
#define LCD_RS_GRP  DIO_GROUP_B
#define LCD_RS_PIN  DIO_PIN_1

#define LCD_RW_GRP  DIO_GROUP_B
#define LCD_RW_PIN  DIO_PIN_2

#define LCD_EN_GRP  DIO_GROUP_B
#define LCD_EN_PIN  DIO_PIN_3

//Data Pins

#define LCD_D0_GRP  DIO_GROUP_B
#define LCD_D0_PIN  DIO_PIN_7

#define	LCD_D1_GRP  DIO_GROUP_B
#define LCD_D1_PIN  DIO_PIN_6

#define LCD_D2_GRP  DIO_GROUP_B
#define LCD_D2_PIN  DIO_PIN_5

#define LCD_D3_GRP  DIO_GROUP_B
#define LCD_D3_PIN  DIO_PIN_4


#define LCD_D4_GRP  DIO_GROUP_A
#define LCD_D4_PIN  DIO_PIN_4

#define LCD_D5_GRP  DIO_GROUP_A
#define LCD_D5_PIN  DIO_PIN_5

#define LCD_D6_GRP  DIO_GROUP_A
#define LCD_D6_PIN  DIO_PIN_6

#define LCD_D7_GRP  DIO_GROUP_A
#define LCD_D7_PIN  DIO_PIN_7

//Backlight Pins


#endif /* LCD_CONFIG_H_ */
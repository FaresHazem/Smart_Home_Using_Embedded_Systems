/*
 * LCD_Init.h
 *
 * Created: 9/26/2023 6:26:41 PM
 *  Author: Beast
 */ 


#ifndef LCD_INIT_H_
#define LCD_INIT_H_

//Initialization
ES LCD_enuInit(void);

//Send Command to the LCD
ES LCD_enuSendCommand(u8 copy_u8_command);

//Send Data to the LCD
ES LCD_enuSendData(u8 copy_u8_data);

//Send String to the LCD
ES LCD_enuSendString(const char *copy_PC_string);

// X 1, 2 | Y 1 -> 16 Go To a position on the LCD
ES LCD_enuSetCursor(u8 copy_u8_X, u8 copu_u8_Y);

//To Print a Special Character
ES LCD_enuDisplaySpecialPattern(u8 copy_u8_special_pattern[], u8 copy_u8_starting_pattern, u8 copy_u8_no_of_patterns, u8 copy_u8_X, u8 copy_u8_Y, u8 copy_u8_language);

//To print integer numbers
ES LCD_enuDisplayIntNumbers(s32 copy_u64_num);

//To print float numbers
ES LCD_enuDisplayFNumbers(f32 copy_f64_num);

#define ROW1               1
#define ROW2               2

#define COL1               1
#define COL2               2
#define COL3               3
#define COL4               4
#define COL5               5
#define COL6               6
#define COL7               7
#define COL8               8
#define COL9               9
#define COL10              10
#define COL11              11
#define COL12              12
#define COL13              13
#define COL14              14
#define COL15              15
#define COL16              16

//Commands

//1-Shift display and cursor commands
#define LCD_DISPLAY_LEFT		0x18
#define LCD_DISPLAY_RIGHT		0x1C
#define LCD_CURSOR_LEFT			0x10
#define	LCD_CURSOR_RIGHT		0x14

//2-To clear display
#define LCD_CLEAR_DISPLAY		0x01

//3-CGRAM Pattern Addresses
#define LCD_PATTERN_0			0x40
#define LCD_PATTERN_1			0x48
#define LCD_PATTERN_2			0x50
#define LCD_PATTERN_3			0x58
#define LCD_PATTERN_4			0x60
#define LCD_PATTERN_5			0x68
#define LCD_PATTERN_6			0x70
#define LCD_PATTERN_7			0x78

//4-Language
#define LCD_ENGLISH				0x06
#define LCD_ARABIC				0x04

//5-Cursor OFF
#define LCD_CURSOR_OFF			0x0C

//6-Space
#define LCD_SPACE_CLEAR			0x20


#endif /* LCD_INIT_H_ */
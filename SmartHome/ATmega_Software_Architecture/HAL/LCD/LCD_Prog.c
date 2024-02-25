/*
 * LCD_Prog.c
 *
 * Created: 9/26/2023 6:26:01 PM
 *  Author: Beast
 */ 

//Public
#include "../../LIB/BitMath.h"
#include "../../LIB/stdTypes.h"
#include "../../LIB/ErrorState.h"

#define F_CPU 16000000
#include <util/delay.h>

//Low Level Libraries
#include "../../MCAL/DIO/DIO_INT.h"

//Private Libraries
#include "LCD_Config.h"
#include "LCD_Priv.h"


//This function is to initialize the LCD configurations
ES LCD_enuInit(){
	
	ES local_enu_error_state = ES_NOK;
	ES local_aenu_error_state[12] = {ES_OK};
		
	_delay_ms(35);
	
	local_aenu_error_state[0] = DIO_enuSetPinMode(LCD_EN_GRP, LCD_EN_PIN, DIO_OUTPUT);
	local_aenu_error_state[1] = DIO_enuSetPinMode(LCD_RW_GRP, LCD_RW_PIN, DIO_OUTPUT);          // Since we are writing therefore we put zero
	local_aenu_error_state[2] = DIO_enuSetPinMode(LCD_RS_GRP, LCD_RS_PIN, DIO_OUTPUT);
	
	local_aenu_error_state[3] = DIO_enuSetPinMode(LCD_D4_GRP, LCD_D4_PIN, DIO_OUTPUT);
	local_aenu_error_state[4] = DIO_enuSetPinMode(LCD_D5_GRP, LCD_D5_PIN, DIO_OUTPUT);
	local_aenu_error_state[5] = DIO_enuSetPinMode(LCD_D6_GRP, LCD_D6_PIN, DIO_OUTPUT);
	local_aenu_error_state[6] = DIO_enuSetPinMode(LCD_D7_GRP, LCD_D7_PIN, DIO_OUTPUT);
	
	#if LCD_MODE == EIGHT_BIT
	
		local_aenu_error_state[7] = DIO_enuSetPinMode(LCD_D0_GRP, LCD_D0_PIN, DIO_OUTPUT);
		local_aenu_error_state[8] = DIO_enuSetPinMode(LCD_D1_GRP, LCD_D1_PIN, DIO_OUTPUT);
		local_aenu_error_state[9] = DIO_enuSetPinMode(LCD_D2_GRP, LCD_D2_PIN, DIO_OUTPUT);
		local_aenu_error_state[10] = DIO_enuSetPinMode(LCD_D3_GRP, LCD_D3_PIN, DIO_OUTPUT);
		
		//Function Set Command
		local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
		enuWriteNLacth(0x38);
		// 0x38 --> For 8 bit mode and with two lines
	
	#elif LCD_MODE == FOUR_BIT
	
		// In the Function Set Command for 4 bit mode we need to send the first command manualy then send it using our funtion
		DIO_enuSetPinValue(LCD_D7_GRP, LCD_D7_PIN, 0);
		DIO_enuSetPinValue(LCD_D6_GRP, LCD_D6_PIN, 0);
		DIO_enuSetPinValue(LCD_D5_GRP, LCD_D5_PIN, 1);
		DIO_enuSetPinValue(LCD_D4_GRP, LCD_D4_PIN, 0);
		
		DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_HIGH);
		_delay_ms(1);
		DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_LOW);
	
		local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
		enuWriteNLacth(0x28);
		// 0x28 --> For 4 bit mode and with two lines
	
	#else
		#error "LCD Mode Config must be Eight or Four Bit"
	#endif	
	
	// Display ON/OFF Control Command
	local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(0x0F);
	
	// Display Clear Command
	local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(0x01);
	
	// Entry Set Mode Command --> Controls writing from left of right
	local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(0x06);
	
	for(u8 i = 0; i < 12; i++){
		if(local_aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_aenu_error_state[i];
			break;
		}
	}
	return local_enu_error_state;
}

static ES enuWriteNLacth(u8 copy_u8_byte){
	
	// If mode == 8 bit or 4 bit this pinValues will be common in both of them
	ES local_enu_error_state = ES_NOK;
	ES local_aenu_error_state[12] = {ES_OK};
	
	local_aenu_error_state[1] = DIO_enuSetPinValue(LCD_RW_GRP, LCD_RW_PIN, DIO_LOW);          // Since we are writing therefore we put zero
	local_aenu_error_state[0] = DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_LOW);
		
	local_aenu_error_state[2] = DIO_enuSetPinValue(LCD_D4_GRP, LCD_D4_PIN, GetBit(copy_u8_byte, 4));
	local_aenu_error_state[3] = DIO_enuSetPinValue(LCD_D5_GRP, LCD_D5_PIN, GetBit(copy_u8_byte, 5));
	local_aenu_error_state[4] = DIO_enuSetPinValue(LCD_D6_GRP, LCD_D6_PIN, GetBit(copy_u8_byte, 6));
	local_aenu_error_state[5] = DIO_enuSetPinValue(LCD_D7_GRP, LCD_D7_PIN, GetBit(copy_u8_byte, 7));
	
	#if LCD_MODE == EIGHT_BIT
	
		local_aenu_error_state[6] = DIO_enuSetPinValue(LCD_D0_GRP, LCD_D0_PIN, GetBit(copy_u8_byte, 0));
		local_aenu_error_state[7] = DIO_enuSetPinValue(LCD_D1_GRP, LCD_D1_PIN, GetBit(copy_u8_byte, 1));
		local_aenu_error_state[8] = DIO_enuSetPinValue(LCD_D2_GRP, LCD_D2_PIN, GetBit(copy_u8_byte, 2));
		local_aenu_error_state[9] = DIO_enuSetPinValue(LCD_D3_GRP, LCD_D3_PIN, GetBit(copy_u8_byte, 3));
		
	#elif LCD_MODE == FOUR_BIT
		
		// Latch
		DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_HIGH);
		_delay_ms(1);
		DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_LOW);
		
		local_aenu_error_state[2] = DIO_enuSetPinValue(LCD_D4_GRP, LCD_D4_PIN, GetBit(copy_u8_byte, 0));
		local_aenu_error_state[3] = DIO_enuSetPinValue(LCD_D5_GRP, LCD_D5_PIN, GetBit(copy_u8_byte, 1));
		local_aenu_error_state[4] = DIO_enuSetPinValue(LCD_D6_GRP, LCD_D6_PIN, GetBit(copy_u8_byte, 2));
		local_aenu_error_state[5] = DIO_enuSetPinValue(LCD_D7_GRP, LCD_D7_PIN, GetBit(copy_u8_byte, 3));
	
	#else
		#error "LCD Mode Config must be Eight or Four Bit"
	#endif
	
	// Falling Edge
	local_aenu_error_state[10] = DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_HIGH);
	_delay_ms(1);
	local_aenu_error_state[11] = DIO_enuSetPinValue(LCD_EN_GRP, LCD_EN_PIN, DIO_LOW);
	
	// delay for waiting LCD to finish its processing
	_delay_ms(2);
	
	// Checking for local errors
	for(u8 i = 0; i < 12; i++){
		if(local_aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_aenu_error_state[i];
			break;
		}else{
			local_enu_error_state = local_aenu_error_state[i];
		}
	}
	return local_enu_error_state;
}

ES LCD_enuSendCommand(u8 copy_u8_command){
	
	ES local_enu_error_state = ES_NOK;
	ES local_aenu_error_state[2];
	
	local_aenu_error_state[0] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	local_aenu_error_state[1] = enuWriteNLacth(copy_u8_command);
	
	for(u8 i = 0; i < 2; i++){
		if(local_aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_aenu_error_state[i];
			break;
		}
	}
	
	return local_enu_error_state;
}

ES LCD_enuSendData(u8 copy_u8_data){
	
	ES local_enu_error_state = ES_NOK;
	ES local_aenu_error_state[2];
	
	local_aenu_error_state[0] = DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
	local_aenu_error_state[1] = enuWriteNLacth(copy_u8_data);
	
	for(u8 i = 0; i < 2; i++){
		if(local_aenu_error_state[i] != ES_OK){
			local_enu_error_state = local_aenu_error_state[i];
			break;
		}else{
			local_enu_error_state = local_aenu_error_state[i];
		}
	}
	
	return local_enu_error_state;
}

ES LCD_enuSendString(const char *copy_PC_string){
	
	ES local_enu_error_state = ES_NOK;
	
	if(copy_PC_string == NULL){
		local_enu_error_state = ES_NULL_POINTER;
	}else{
		
		while(*copy_PC_string != '\0'){
			
			DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
			local_enu_error_state = enuWriteNLacth(*copy_PC_string);
			if(local_enu_error_state != ES_OK){
				break;
			}
			copy_PC_string++;
		}
	}
	return local_enu_error_state;
}

ES LCD_enuSetCursor(u8 copy_u8_X, u8 copu_u8_Y){
	
	ES local_enu_error_state = ES_NOK;
	
	if((copy_u8_X <= ROW2 && copy_u8_X >= ROW1) && (copu_u8_Y <= COL16 && copu_u8_Y >= COL1)){
		
		// Since our address starts from 0x80
		DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
		enuWriteNLacth(0x80 + (0x40 * (copy_u8_X - 1)) + (copu_u8_Y - 1));
		
	}else{
		local_enu_error_state = ES_OUT_OF_RANGE;
	}
	
	return local_enu_error_state;
}

ES LCD_enuDisplaySpecialPattern(u8 copy_u8_special_pattern[], u8 copy_u8_start_pattern, u8 copy_u8_no_of_patterns, u8 copy_u8_X, u8 copy_u8_Y, u8 copy_u8_language){
	
	ES local_enu_errorstate = ES_OK;
	
	u8 local_u8_pattern = LCD_u8_PATTERNS[copy_u8_start_pattern];
	
	//1-Access the pattern in the CGRAM to store your custom patterns and display it
	DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(local_u8_pattern);       // Access Pattern at CGRAM
	
	//2-A single pattern has 8 characters, so we can calculate the number of characters
	u8 local_u8_characters = (copy_u8_no_of_patterns * 8);
	
	//3-Sending each character to be stored in patterns
	for(u8 i = 0; i < local_u8_characters; i++){
		DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
		enuWriteNLacth(copy_u8_special_pattern[i]);
	}
	//4-Changing the language whether to write from left or right
	DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(copy_u8_language);      // Language
	
	//5-SetCursor at which position the pattern will be displayed
	DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_LOW);
	enuWriteNLacth(0x80 + (0x40 * (copy_u8_X - 1)) + (copy_u8_Y - 1));
	
	//6-Access each pattern and display it
	u8 local_u8_patterns_to_display = (copy_u8_no_of_patterns + copy_u8_start_pattern);
	for(copy_u8_start_pattern; copy_u8_start_pattern < local_u8_patterns_to_display; copy_u8_start_pattern++){
		DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
		enuWriteNLacth(copy_u8_start_pattern);
	}
	
	return local_enu_errorstate;
}

ES enuDisplayNum(u32 copy_u32_num){
	
	ES local_enu_errorstate = ES_OK;
	
	u8 digit;         // to access every digit
	char digits[12];  // to store all digits
	u8 i = 0;
	s8 no_of_digits = 0;
	
	while(copy_u32_num > 0){
		digit = copy_u32_num % 10;            // accessing the last digit
		digits[i] = digit + '0';              // changing the digit to char so we can display it
		copy_u32_num /= 10;                   // removing last digit
		i++;
		no_of_digits++;                       // counting digits
	}
	
	// Displaying
	for(no_of_digits; no_of_digits > 0; no_of_digits--){
		if(digits[no_of_digits - 1] == '0'){
			DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
			enuWriteNLacth('0');
		}else{
			DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
			enuWriteNLacth(digits[no_of_digits-1]);
		}
	}
	return local_enu_errorstate;
}

ES LCD_enuDisplayIntNumbers(s32 copy_s32_num){
	
	ES local_enu_errorstate = ES_NOK;
	
	if(copy_s32_num < 0){
		copy_s32_num *= -1;        // change to positive
		
		DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
		enuWriteNLacth('-');
	}
	
	local_enu_errorstate = enuDisplayNum(copy_s32_num);
	return local_enu_errorstate;
}

ES LCD_enuDisplayFNumbers(f32 copy_f32_num){
	
	ES local_enu_errorstate = ES_OK;
	
	u8 local_u8_decimal_counter = 0;
	
	// 1200.125
	enuDisplayNum((s32)copy_f32_num);
	f32 local_f32_decimalpart;
	local_f32_decimalpart = copy_f32_num - (s32)copy_f32_num;
	
 	if(copy_f32_num < 0){
 		copy_f32_num *= -1;        // change to positive
 		
 		DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
 		enuWriteNLacth('-');
 	}

	
	// Converting decimal part to integer
	while(local_f32_decimalpart - (s32)local_f32_decimalpart){
		local_f32_decimalpart *= 10;	
		local_u8_decimal_counter++;
		if(local_u8_decimal_counter == 3){
			break;
		}
	}
	
	DIO_enuSetPinValue(LCD_RS_GRP, LCD_RS_PIN, DIO_HIGH);
	enuWriteNLacth('.');
	
	enuDisplayNum(local_f32_decimalpart);
	return local_enu_errorstate;
}
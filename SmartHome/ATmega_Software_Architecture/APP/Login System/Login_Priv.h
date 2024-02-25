/*
 * Login_Priv.h
 *
 * Created: 10/4/2023 6:03:26 PM
 *  Author: Beast
 */ 


#ifndef LOGIN_PRIV_H_
#define LOGIN_PRIV_H_

#define PASSWORD		{'1', '5', '/', '2', '*', '-', '3'}

ES enuWelcomeScreen(boolean copy_bool_screen_state);
static boolean local_bool_welcomeScreen = True;

ES enuLoginScreen(boolean copy_bool_screen_state, boolean *copy_bool_pass_state, u8 *copy_u8_error_counter);
static boolean local_bool_loginscreen = True;
static u8 local_u8_error_counter = '0';

ES enuThiefScreen(boolean copy_bool_screen_state);
static boolean local_bool_thiefscreen = True;
static const u8 local_au8_thief[] = {0x00, 0x00, 0x1C, 0x02, 0x1F, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10,
									0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
									0x00, 0x00, 0x00, 0x1F, 0x0A, 0x0E, 0x00, 0x00,
									0x00, 0x00, 0x07, 0x04, 0x12, 0x1E, 0x00, 0x00};
									
ES enuMainScreen(boolean copy_bool_screen_state);
static boolean local_bool_mainscreen = True;

ES enuLedScreen(boolean copy_bool_screen_state);
static boolean local_bool_ledscreen = True;

ES enuLedStates(u8 copy_u8_led_id);  // since we have more than one led

ES enuTempScreen();

#endif /* LOGIN_PRIV_H_ */
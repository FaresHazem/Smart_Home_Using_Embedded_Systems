/*
 * HexDec_Config.h
 *
 * Created: 9/23/2023 9:45:52 PM
 *  Author: Beast
 */ 


#ifndef HEXDEC_CONFIG_H_
#define HEXDEC_CONFIG_H_

// Define all HexaDecoder pins

#define HexDec_A_GRP		DIO_GROUP_A                          // Pin a at which group
#define HexDec_PIN_A		DIO_PIN_4                          // Pin a at which pin in the group

#define HexDec_B_GRP		DIO_GROUP_A
#define HexDec_PIN_B		DIO_PIN_5

#define HexDec_C_GRP		DIO_GROUP_A
#define HexDec_PIN_C		DIO_PIN_6

#define HexDec_D_GRP		DIO_GROUP_A
#define HexDec_PIN_D		DIO_PIN_7

#define HexDec_EN1_GRP		DIO_GROUP_B
#define HexDec_EN1_PIN		DIO_PIN_1

#define HexDec_EN2_GRP		DIO_GROUP_B
#define HexDec_EN2_PIN		DIO_PIN_2


#endif /* HEXDEC_CONFIG_H_ */
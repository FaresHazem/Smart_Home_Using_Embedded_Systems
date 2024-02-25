/*
 * HexDec_Interface.h
 *
 * Created: 9/23/2023 9:46:27 PM
 *  Author: Beast
 */ 


#ifndef HEXDEC_INTERFACE_H_
#define HEXDEC_INTERFACE_H_

ES HexDec_enu_Init(void);

ES HexDec_enu_Display(u8 copy_u8_number);

ES HexDec_enu_EnableSegment(u8 copy_u8_segment_id);

ES HexDec_enu_DisableSegment(u8 copy_u8_segment_id);

ES HexDec_enu_StopDisplay();

#define Segment_1    0
#define Segment_2    1


#endif /* HEXDEC_INTERFACE_H_ */
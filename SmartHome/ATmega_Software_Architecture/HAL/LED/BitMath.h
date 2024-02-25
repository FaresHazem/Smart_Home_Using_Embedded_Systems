/*
 * BitMath.h
 *
 * Created: 9/16/2023 10:02:32 PM
 *  Author: Beast
 */ 


#ifndef BITMATH_H_
#define BITMATH_H_

#define SetBit(reg, bit_no) (reg |= (1<<bit_no))
#define ClearBit(reg, bit_no) (reg &= ~(1<<bit_no))
#define ToggleBit(reg, bit_no) (reg ^= (1<<bit_no))
#define GetBit(reg, bit_no) (((reg) >> (bit_no)) & 1)

#define InitializeBit(reg, bit_no, bit_value) \
((bit_value) ? ((reg) |= (1 << (bit_no))) : ((reg) &= ~(1 << (bit_no))))

#endif /* BITMATH_H_ */
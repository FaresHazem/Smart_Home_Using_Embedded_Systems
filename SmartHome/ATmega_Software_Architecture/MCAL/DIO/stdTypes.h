/*
 * stdTypes.h
 *
 * Created: 9/16/2023 10:03:16 PM
 *  Author: Beast
 */ 


#ifndef STDTYPES_H_
#define STDTYPES_H_

typedef unsigned char u8;          // 1byte
typedef unsigned short int u16;    // 2bytes
typedef unsigned long int u32;     // 4bytes
typedef unsigned long long u64;    // 8bytes 

typedef signed char s8;          // 1byte
typedef signed short int s16;    // 2bytes
typedef signed long int s32;     // 4bytes
typedef signed long long s64;    // 8bytes

typedef float f32;               // 4bytes
typedef double f64;              // 8bytes
typedef long double f128;        // 16bytes

#define NULL  ((void*)0)

#endif /* STDTYPES_H_ */
/*
 * ADC_Interface.h
 *
 * Created: 10/18/2023 8:04:27 PM
 *  Author: Beast
 */ 


#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_


ES ADC_enuInit(void);

ES ADC_enuReadPolling(u8 copy_u8_channel, u16 *copy_u16_value);		// Analog Read signals in a while loop generates no interrupt

//-------------------------------------------------------------------------
//Interrupt Functions

ES ADC_enuStartConversion(void);							// To start conversion

ES ADC_enuSetChannel(u8 copy_u8_channel);					// Set the channel we will use to read analog signal

ES ADC_enuTriggerSource(u8 copy_u8_source);					// Set trigger source that will trigger the next conversion

ES ADC_enuClearTrigger(void);								// Clears auto trigger

ES ADC_enuRead(u16 *copy_u16_value);						// Analog read all 10 bits

ES ADC_enuReadHigh(u16 *copy_u16_value);					// Analog read all 8 high bits

ES ADC_enuEnableInterrupt(void);				

ES ADC_enuDisableInterrupt(void);

ES ADC_enuSetCallBack(void (*ptr_to_fun)(void*), void * copy_pvid_parameter);			// Interrupt function implemented by the user
//-------------------------------------------------------------------------

//Channels
typedef enum{
	
	ADC0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7,		//single ended input (0-->7)
	ADC0_ADC0_10X, ADC1_ADC0_10X, ADC0_ADC0_200X, ADC1_ADC0_200X, ADC2_ADC2_10X, ADC3_ADC2_10X, ADC2_ADC2_200X, ADC3_ADC2_200X,		//two differential channels with gain (7-->15)
	ADC0_ADC1_1X, ADC1_ADC1_1X, ADC2_ADC1_1X, ADC3_ADC1_1X, ADC4_ADC1_1X, ADC5_ADC1_1X, ADC6_ADC1_1X, ADC7_ADC1_1X,		//Multiplexed differential channels with common negative on ADC1 (15-->23)
	ADC0_ADC2_1X, ADC1_ADC2_1X, ADC2_ADC2_1X, ADC3_ADC2_1X, ADC4_ADC2_1X, ADC5_ADC2_1X									//Multiplexed differential channels with common negative on ADC2 (23-->29)
	
}ADC_CHANNELS;

//Trigger Sources
#define ADC_SINGLE_CONVERSION		70
#define ADC_FREE_RUNNING			10
#define ADC_ANALOG_COMPARATOR		11
#define ADC_EXTI_REQUEST			12
#define ADC_TIMER_COMPARE			13
#define ADC_TIMER_OVERFLOW			14


#endif /* ADC_INTERFACE_H_ */
/*
 * ADC_Priv.h
 *
 * Created: 10/18/2023 8:04:00 PM
 *  Author: Beast
 */ 

#ifndef ADC_PRIV_H_
#define ADC_PRIV_H_

//Registers
#define ADCSRA		*((volatile u8*)(0x26))	//ADC Control and Status Register
#define ADMUX		*((volatile u8*)(0x27))	//ADC Multiplexer Selection Register
#define ADCL		*((volatile u8*)(0x24))	//Data Register1
#define ADCH		*((volatile u8*)(0x25))	//Data Register2
#define SFIOR		*((volatile u8*)(0x50)) //Special FunctionIO Register

//Prescale Values
#define ADC_PRESC_2				1
#define ADC_PRESC_4				2
#define ADC_PRESC_8				3
#define ADC_PRESC_16			4
#define ADC_PRESC_32			5
#define ADC_PRESC_64			6
#define ADC_PRESC_128			7

//Volt References
#define ADC_AREF				99                     //Uses the AREF pin as the reference voltage
#define ADC_AVCC				100                    //Uses microcontroller power supply 5v 
#define ADC_INTERNAL_VOLT		101                    //Uses internal built in voltage of 2.56v

//Adjustment Levels
#define ADC_LEFT_ADJ			20
#define ADC_RIGHT_ADJ			30

//Trigger Sources
#define ADC_SINGLE_CONVERSION		70					// Only one time conversion need to call start conversion every time
#define ADC_FREE_RUNNING			10					// Reads automatically the next signal after first conversion finishes (uses auto trigger)
#define ADC_ANALOG_COMPARATOR		11
#define ADC_EXTI_REQUEST			12					// Reads every time an external interrupt occurs
#define ADC_TIMER_COMPARE			13
#define ADC_TIMER_OVERFLOW			14

//Interrupt Modes
#define ADC_INTERRUPT				111
#define ADC_POLLING					222

//Channels
typedef enum{
	
	ADC0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7,		//single ended input (0-->7)
	ADC0_ADC0_10X, ADC1_ADC0_10X, ADC0_ADC0_200X, ADC1_ADC0_200X, ADC2_ADC2_10X, ADC3_ADC2_10X, ADC2_ADC2_200X, ADC3_ADC2_200X,		//two differential channels with gain (7-->15)
	ADC0_ADC1_1X, ADC1_ADC1_1X, ADC2_ADC1_1X, ADC3_ADC1_1X, ADC4_ADC1_1X, ADC5_ADC1_1X, ADC6_ADC1_1X, ADC7_ADC1_1X,		//Multiplexed differential channels with common negative on ADC1 (15-->23)
	ADC0_ADC2_1X, ADC1_ADC2_1X, ADC2_ADC2_1X, ADC3_ADC2_1X, ADC4_ADC2_1X, ADC5_ADC2_1X									//Multiplexed differential channels with common negative on ADC2 (23-->29)
	
}ADC_CHANNELS;


#endif /* ADC_PRIV_H_ */
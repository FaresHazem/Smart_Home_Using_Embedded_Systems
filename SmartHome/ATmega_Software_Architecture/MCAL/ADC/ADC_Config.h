/*
 * ADC_Config.h
 *
 * Created: 10/18/2023 8:04:12 PM
 *  Author: Beast
 */ 


#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#define ADC_PRESCALER						ADC_PRESC_64// Prescale value used to adjust the clock cycles per second for the ADC (since mc is 16MHz therefore we divide by the prescaler value to get a lower speed for the ADC)

#define ADC_VOLT_REFERENCE					ADC_AVCC// Volt reference is used to assign a value to the AREF bit

#define ADC_TRIGGER_SOURCE					ADC_FREE_RUNNING// The source that will trigger a new conversion

#define ADC_ADJUSTMENT_LEVEL				ADC_LEFT_ADJ// The way we will read from the data registers whether left or right

#define ADC_INITIALIZATION_CHANNEL			ADC1// if no channel selected this will be the main channel

#define ADC_INTERRUPT_MODE					ADC_INTERRUPT// interrupt mode

#endif /* ADC_CONFIG_H_ */
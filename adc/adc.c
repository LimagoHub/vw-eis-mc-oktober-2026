/*
 * adc.c
 *
 * Created: 19.09.2024 10:59:25
 *  Author: JoachimWagner
 */ 

#include "adc.h"

void adc_init()
{
	DDRA=0x0;			/* Make ADC port as input */
	ADCSRA = (1 << ADEN) | ( 1 << ADPS0) | ( 1 << ADPS1) | ( 1 << ADPS2);			/* Enable ADC, fr/128  */
	ADMUX = (1 << REFS0);
	
}

uint16_t adc_read(uint8_t channel)
{
	uint16_t result;
	
	ADMUX &= 0xf0;
	
	ADMUX |= (channel & 0x0f);	/* Set input channel to read */

	ADCSRA |= (1<<ADSC);		/* Start conversion */
	while(ADCSRA & (1<<ADSC));	/* Wait until conversion is finished */
	
	result = (uint16_t) ADCL;		/* Read lower byte*/
	result |= (uint16_t)ADCH << 8;		/* Read higher 2 bits */

	return result;			/* Return digital value*/
}

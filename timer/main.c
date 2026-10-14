#include <avr/io.h>
#include <avr/interrupt.h>
ISR(TIMER0_COMP_vect){

    PORTB ^=  0xFF;
}

int main(void)
{
    DDRB = 0xFF;
    PORTB = 0;

    TCCR0 = (1 << WGM01); // CTC Modus
    OCR0 = 141;

    TIMSK = (1 << OCIE0); // Interrupt anschalten
    sei();
    TCCR0 |= (1 << CS01) | (1 << CS00);
    while (1)
    {
    }
	
 }
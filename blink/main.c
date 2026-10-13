#include <avr/io.h>

#include <util/delay.h>
int main(void)
{

    // Data Direction Register^C
    DDRC = 0b000000001;
    PORTC = 0;

    while (1) {
        PORTC = 0b000000001;
        _delay_ms(500);

        PORTC = 0b000000001;
        _delay_ms(500);
    }
}
#include <avr/io.h>

int main(void) {
    TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
    DDRB|=(1<<PB3);
    OCR0 = 64;
    while(1);
}

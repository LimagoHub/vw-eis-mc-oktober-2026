#include <avr/io.h>
#include <avr/interrupt.h>
#define SET_BIT(byte,bit) byte|=(1<<bit)
#define CLEAR_BIT(byte,bit) byte&=~(1<<bit)
#define TOGGLE_BIT(byte,bit) byte^=(1<<bit)
#define CHECK_BIT(byte,bit) (byte&(1<<bit))

int main(void) {
    // Eingang und Pullup definieren
    DDRD = 0;
    PORTD = (1 << PD2) | (1 << PD3); // Setzen Pullup

    // Ausgang definieren
    SET_BIT(DDRC, PC0);
    PORTC = 0;

    while (1);
}

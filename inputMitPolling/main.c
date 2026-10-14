#include <avr/io.h>
#define SET_BIT(byte,bit) byte|=(1<<bit)
#define CLEAR_BIT(byte,bit) byte&=~(1<<bit)
#define TOGGLE_BIT(byte,bit) byte^=(1<<bit)
#define CHECK_BIT(byte,bit) (byte&(1<<bit))
void setup(void) {

    // Eingang und Pullup definieren
    DDRD = 0;
    PORTD = (1 << PD2) | (1 << PD3); // Setzen Pullup

    // Ausgang definieren
    SET_BIT(PORTC, PC0);
    PORTC = 0;
}

void loop(void) {
    if (CHECK_BIT(PIND, PIN2)==0) {
        SET_BIT(PORTC, PC0);

    }
    if (CHECK_BIT(PIND, PIN3)==0) {
        CLEAR_BIT(PORTC, PC0);
    }
}

int main(void) {

    setup();
    while (1) {
        loop();
    }
}

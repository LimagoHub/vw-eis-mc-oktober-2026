#include <avr/io.h>

#include <util/delay.h>

#define SET_BIT(byte,bit) byte|=(1<<bit)
#define CLEAR_BIT(byte,bit) byte&=~(1<<bit)
#define TOGGLE_BIT(byte,bit) byte^=(1<<bit)


int main(void)
{

    // Data Direction Register^C


    DDRD = 0;

    PORTD = 0b00000011;



    while (1) {
        if(!(PIND & (1 << PD2)))
           ; // Lampe an

        if(!(PIND & (1 << PD3)))
            ; // Lampe aus


    }
}
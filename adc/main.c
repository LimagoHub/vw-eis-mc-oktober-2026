#include "adc.h"
#include <avr/io.h>


int main(void)
{
    adc_init();
    /* Replace with your application code */
    while (1)
    {

        uint8_t kanal = 2;
        uint16_t messwert = adc_read(kanal);
    }
}

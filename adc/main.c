#include "adc.h"
#include <avr/io.h>
#include <util/delay.h>

#include "serial.h"

#define UREF 5.0f   // Referenzspannung AVCC in Volt


int main(void)
{
    serial_init();
    adc_init();

    uint8_t kanal = 0;
    while (1)
    {
        uint16_t messwert = adc_read(kanal);

        serial_send_string("Kanal ");
        serial_send_int(kanal);
        serial_send_string(": ");
        serial_send_int(messwert);

        // Spannung als Kommazahl (Referenz AVCC, 10 Bit -> 0..1023)
        float spannung = messwert * UREF / 1023.0f;
        serial_send_string(" = ");
        serial_send_float(spannung);
        serial_send_string_nl(" V");

        kanal ^= 1;        // abwechselnd Kanal 0 und Kanal 1
        _delay_ms(200);
    }
}

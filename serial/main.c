/*
 * Seriell.c
 *
 * Created: 18.09.2024 13:04:46
 * Author : JoachimWagner
 *
 * UART-Echo auf dem ATmega32: 9600 Baud, 8 Datenbits, keine Paritaet, 1 Stoppbit (8N1)
 */

#include <avr/io.h>
#include "serial.h"
#include <stdlib.h>
#include <util/delay.h>
#define _DEBUG




int main(void)
{

uint8_t zahl=65;
#ifdef _DEBUG
    serial_send_string_nl("hallo");
#endif

    while (1) {


        //uint8_t value = serial_received();
        char buffer[4];
        itoa(zahl, buffer, 10);
        serial_send_string_nl(buffer);
        _delay_ms(1000);
        //serial_send_char(zahl);
        serial_send_int(45);
        serial_send_float(3.14f);

    }
}

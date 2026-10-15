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


#define _DEBUG




int main(void)
{


#ifdef _DEBUG
    serial_send_string_nl("hallo");
#endif

    while (1) {


        uint8_t value = serial_received();
        serial_send_char(value);

    }
}

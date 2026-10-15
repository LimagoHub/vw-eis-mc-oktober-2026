//
// Created by JoachimWagner on 15.10.2026.
//



#ifndef SERIAL_SERIAL_H
#define SERIAL_SERIAL_H
#include <avr/io.h>


#define BAUD 9600UL
// Rundende Berechnung des Baudratenregisters (Klammern wichtig, da Makro!)
#define MYUBRR (((F_CPU + 8UL * BAUD) / (16UL * BAUD)) - 1UL)
void serial_init();
uint8_t serial_received();
void serial_send_char(uint8_t valueToSend);
void serial_send_string(const char *stringToSend);
void serial_send_string_nl(const char *stringToSend);


#endif //SERIAL_SERIAL_H

//
// Created by JoachimWagner on 15.10.2026.
//
#include "serial.h"
#include <stdlib.h>
void serial_init() {
    // Baudrate: UBRRH und UBRRL (UBRRH mit Bit 7 = 0 schreiben -> wird als UBRRH erkannt)
    UBRRH = (uint8_t) (MYUBRR >> 8);
    UBRRL = (uint8_t) MYUBRR;

    // ---------------------------------------------------------------------
    // Rahmenformat einstellen: 8N1 (8 Datenbits, keine Paritaet, 1 Stoppbit)
    //
    // UCSRC ist ein 8-Bit-Register. Jedes Bit ist ein Schalter:
    //
    //   Bit:    7      6      5     4     3     2      1      0
    //   Name: URSEL  UMSEL  UPM1  UPM0  USBS  UCSZ1  UCSZ0  UCPOL
    //   Wert:   1      0     0     0     0     1      1      0
    //
    // Was macht (1 << URSEL)?
    //   Die Zahl 1 wird um den Wert von URSEL nach links geschoben.
    //   URSEL ist in avr/io.h als 7 definiert (= Bitnummer 7).
    //   (1 << 7) ergibt also binaer 1000 0000 -> nur Bit 7 ist gesetzt.
    //   Genauso: (1 << UCSZ1) = 0000 0100 und (1 << UCSZ0) = 0000 0010.
    //
    // Der Operator | (ODER) legt die drei Einzelwerte uebereinander:
    //   1000 0000   (URSEL)
    //   0000 0100   (UCSZ1)
    //   0000 0010   (UCSZ0)
    //   ---------
    //   1000 0110   = 0x86 -> dieser Wert wird in UCSRC geschrieben
    //
    // Bedeutung der gesetzten Bits:
    //   UCSZ1 = 1, UCSZ0 = 1  -> 8 Datenbits pro Zeichen
    //                            (00=5 Bit, 01=6 Bit, 10=7 Bit, 11=8 Bit)
    //   URSEL = 1             -> Sonderfall beim ATmega32: UCSRC und UBRRH
    //                            teilen sich dieselbe Adresse im Chip. Beim
    //                            Schreiben entscheidet Bit 7, wer gemeint ist:
    //                            1 = UCSRC, 0 = UBRRH. Ohne URSEL = 1 wuerde
    //                            der Wert beim falschen Register landen.
    //
    // Alle anderen Bits bleiben 0, das ergibt die Standardeinstellungen:
    //   UMSEL = 0 -> asynchron (kein Taktsignal, beide Seiten haben ihren
    //                eigenen Takt und muessen dieselbe Baudrate nutzen)
    //   UPM1, UPM0 = 00 -> keine Paritaet (keine zusaetzliche Pruefbit)
    //   USBS = 0 -> 1 Stoppbit
    //   UCPOL = 0 -> nur fuer den synchronen Betrieb wichtig, hier egal
    //
    // Warum "=" und nicht "|=" ?
    //   Mit |= wuerde der Compiler erst den alten Wert LESEN, ihn mit dem
    //   neuen verodern und zurueckschreiben. Beim Lesen von UCSRC bekommt man
    //   beim ATmega32 aber unter Umstaenden den Inhalt von UBRRH (gleiche
    //   Adresse!). Das gaebe zufaellige Bits im Ergebnis. Mit "=" wird
    //   nichts gelesen, das Register bekommt genau den Wert, den wir wollen.
    // ---------------------------------------------------------------------
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

    // Empfaenger und Sender einschalten
    UCSRB = (1 << RXEN) | (1 << TXEN);
}

uint8_t serial_received() {
    while ((UCSRA & (1 << RXC)) == 0){} // BUSY WAIT
    return UDR;
}

void serial_send_char(uint8_t valueToSend)
{
    while (!(UCSRA & (1 << UDRE)));   // warten, bis Sendepuffer frei ist
    UDR = valueToSend;
}

void serial_send_string(const char *stringToSend)
{
    while (*stringToSend)
        serial_send_char((uint8_t) *stringToSend++);
}

void serial_send_string_nl(const char *stringToSend)
{
    serial_send_string(stringToSend);
    serial_send_string("\r\n");
}

void serial_send_int(uint16_t valueToSend) {
    char buffer[32];
    itoa(valueToSend, buffer, 10);
    serial_send_string(buffer);
}
void serial_send_float(float valueToSend) {
    char buffer[32];
    dtostrf(valueToSend, 10, 2, buffer);
    serial_send_string(buffer);
}

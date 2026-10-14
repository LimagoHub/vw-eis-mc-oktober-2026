/*
 * LCD.c - Demo der LCD-Bibliothek
 *
 * Created: 19.09.2024 12:20:31
 * Author : JoachimWagner
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lcd_lib.h"

/* Eigenes Zeichen: ein Herz (5x8 Punkte, 1 = Punkt an) */
static const unsigned char herz[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

int main(void)
{
    LCD4_init();                        /* immer zuerst! */
    LCD4_create_char(0, herz);          /* Herz in Slot 0 ablegen */

    while (1)
    {
        /* 1) Text und Position */
        LCD4_clear();
        LCD4_write_string("Hallo Azubis!");
        LCD4_gotoxy(2, 1);
        LCD4_write_string("LCD-Demo ");
        LCD4_write(0);                  /* Herz anzeigen */
        _delay_ms(2000);

        /* 2) Zahlen */
        LCD4_clear();
        LCD4_write_at(1, 1, "int:  ");
        LCD4_write_int(-1234);
        LCD4_write_at(2, 1, "dbl:  ");
        LCD4_write_double(3.14159);
        _delay_ms(2000);

        /* 3) Hex und Binaer */
        LCD4_clear();
        LCD4_write_at(1, 1, "Hex: ");
        LCD4_write_hex(0xA5);
        LCD4_write_at(2, 1, "Bin: ");
        LCD4_write_bin(0xA5);
        _delay_ms(2000);

        /* 4) Zaehler: nur die Zahl aktualisieren, nicht alles loeschen */
        LCD4_clear();
        LCD4_write_at(1, 1, "Zaehler:");
        for (unsigned int i = 0; i <= 10; i++)
        {
            LCD4_clear_line(2);
            LCD4_write_uint(i);
            _delay_ms(300);
        }

        /* 5) Cursor-Varianten */
        LCD4_clear();
        LCD4_write_string("Cursor: Strich");
        LCD4_cursor_on();
        _delay_ms(2000);
        LCD4_clear();
        LCD4_write_string("Cursor: Block");
        LCD4_cursor_blink();
        _delay_ms(2000);
        LCD4_cursor_off();

        /* 6) Display ein/aus: Inhalt bleibt erhalten */
        LCD4_clear();
        LCD4_write_string("Blinkt!");
        for (unsigned char i = 0; i < 3; i++)
        {
            LCD4_display_off();
            _delay_ms(300);
            LCD4_display_on();
            _delay_ms(300);
        }

        /* 7) Home: Cursor zurueck, ohne zu loeschen -> ueberschreiben */
        LCD4_clear();
        LCD4_write_string("AAAAAAAAAA");
        _delay_ms(1000);
        LCD4_home();
        LCD4_write_string("BBBBB");
        _delay_ms(2000);

        /* 8) Segmente: 4 Felder mit je 6 Zeichen */
        LCD4_clear();
        LCD4_write_segment(1, "T1:");
        LCD4_write_segment(2, "T2:");
        LCD4_write_segment(3, "21.5C");
        LCD4_write_segment(4, "19.0C");
        _delay_ms(2000);
        LCD4_write_segment(3, "7");        /* kurzer Wert: Rest wird geloescht */
        LCD4_write_segment(4, "ZULANGERTEXT"); /* wird auf 6 Zeichen gekuerzt */
        _delay_ms(2000);
        LCD4_write_segment_int(3, -42);
        LCD4_write_segment_double(4, 3.14159, 2);
        _delay_ms(2000);

        /* 9) Lauftext: gesamten Inhalt nach links schieben */
        LCD4_clear();
        LCD4_write_at(1, 1, "Lauftext - Lauftext");
        _delay_ms(1000);
        for (unsigned char i = 0; i < 16; i++)
        {
            LCD4_shift_left();
            _delay_ms(250);
        }
    }
}

#ifndef LCD_LIB_H
#define LCD_LIB_H

/*
 * Bibliothek fuer ein 2x16-Zeichen-LCD (HD44780-kompatibel) im 4-Bit-Modus.
 *
 * Kurz erklaert:
 *  - Das Display hat 2 Zeilen mit je 16 Zeichen.
 *  - Zeilen und Spalten werden ab 1 gezaehlt: (1,1) ist oben links,
 *    (2,16) ist unten rechts.
 *  - Im 4-Bit-Modus brauchen wir nur 4 Datenleitungen (D4..D7) statt 8.
 *    Jedes Byte wird deshalb in zwei Haelften (Nibbles) gesendet.
 *  - Immer zuerst LCD4_init() aufrufen, bevor etwas ausgegeben wird!
 */

#include <avr/io.h>
#include <util/delay.h>

/* Verdrahtung: Steuerleitungen PB0..PB2, Daten D4..D7 an PB4..PB7 */
#define LCD_DATA PORTB   /* Ausgangsregister des Ports */
#define LCD_DDR  DDRB    /* Richtungsregister (1 = Ausgang) */
#define LCD_RS   PB0     /* Register Select: 0 = Befehl, 1 = Zeichen */
#define LCD_RW   PB1     /* Read/Write: bei uns immer 0 (nur schreiben) */
#define LCD_EN   PB2     /* Enable: ein kurzer Puls uebernimmt die Daten */
#define LCD_D4   PB4
#define LCD_D5   PB5
#define LCD_D6   PB6
#define LCD_D7   PB7

#define LCD_COLUMNS 16   /* Zeichen pro Zeile */

/* Segmente: Jede Zeile ist in zwei Felder zu je LCD_SEG_WIDTH Zeichen
 * aufgeteilt. Zwischen und hinter den Feldern bleiben Leerzeichen frei.
 *
 *   Spalte:  1      6 7 8 9     14 15 16
 *           [Segment 1] .. [Segment 2] ..     <- Zeile 1
 *           [Segment 3] .. [Segment 4] ..     <- Zeile 2
 */
#define LCD_SEG_WIDTH       6
#define LCD_SEG_LEFT_COL    1   /* Startspalte von Segment 1 und 3 */
#define LCD_SEG_RIGHT_COL   9   /* Startspalte von Segment 2 und 4 */

/* ------------------------------------------------------------------ */
/* Grundfunktionen                                                    */
/* ------------------------------------------------------------------ */

/* Sendet einen Befehl (z.B. "Display loeschen") an das Display.
 * Meist brauchst du diese Funktion nicht direkt, sondern die
 * fertigen Funktionen weiter unten. */
void LCD4_cmd(unsigned char cmd);

/* Startet das Display. MUSS einmal am Programmanfang aufgerufen werden.
 * Stellt Ports ein, schaltet in den 4-Bit-Modus, Display an, Cursor aus. */
void LCD4_init(void);

/* Schreibt ein einzelnes Zeichen an die aktuelle Cursorposition,
 * z.B. LCD4_write('A'). Der Cursor rutscht danach automatisch
 * eine Stelle weiter. */
void LCD4_write(unsigned char data);

/* Schreibt einen Text, z.B. LCD4_write_string("Hallo").
 * Achtung: Es gibt KEINEN automatischen Zeilenumbruch. Ist der Text
 * laenger als die Zeile, geht er unsichtbar ueber den Rand hinaus.
 * Fuer die zweite Zeile LCD4_gotoxy(2, 1) benutzen. */
void LCD4_write_string(const char *data);

/* Schreibt eine ganze Zahl (mit Vorzeichen), z.B. LCD4_write_int(-42). */
void LCD4_write_int(int data);

/* Schreibt eine Kommazahl mit 2 Nachkommastellen,
 * z.B. LCD4_write_double(3.14159) zeigt "3.14". */
void LCD4_write_double(double data);

/* Setzt den Cursor auf eine Position. Danach schreibt die naechste
 * Ausgabe dort. zeile: 1..2, spalte: 1..16.
 * Beispiel: LCD4_gotoxy(2, 5) = Zeile 2, Spalte 5.
 * Ungueltige Werte werden ignoriert. */
void LCD4_gotoxy(int zeile, int spalte);

/* Loescht das ganze Display und setzt den Cursor auf (1,1). */
void LCD4_clear(void);

/* ------------------------------------------------------------------ */
/* Cursor und Display                                                 */
/* ------------------------------------------------------------------ */

/* Setzt den Cursor auf (1,1), OHNE den Inhalt zu loeschen.
 * Neue Texte ueberschreiben dann die alten Zeichen. */
void LCD4_home(void);

/* Schaltet die Anzeige ein bzw. aus. Beim Ausschalten bleibt der Inhalt
 * im Display gespeichert und ist beim Einschalten sofort wieder da.
 * Praktisch zum Blinken eines ganzen Textes. */
void LCD4_display_on(void);
void LCD4_display_off(void);

/* Cursor-Aussehen:
 *  cursor_off   - Cursor unsichtbar (Standard nach LCD4_init)
 *  cursor_on    - Unterstrich unter der aktuellen Stelle
 *  cursor_blink - blinkender Block (gut fuer Eingabefelder) */
void LCD4_cursor_on(void);
void LCD4_cursor_off(void);
void LCD4_cursor_blink(void);

/* Bewegt den Cursor um eine Stelle nach links bzw. rechts,
 * ohne etwas zu schreiben. */
void LCD4_cursor_left(void);
void LCD4_cursor_right(void);

/* Schiebt den GESAMTEN Inhalt um eine Stelle nach links bzw. rechts
 * (beide Zeilen gemeinsam). Wiederholt mit kleiner Pause aufgerufen
 * ergibt das einen Lauftext. */
void LCD4_shift_left(void);
void LCD4_shift_right(void);

/* ------------------------------------------------------------------ */
/* Komfort-Ausgabe                                                    */
/* ------------------------------------------------------------------ */

/* Position setzen und Text schreiben in einem Schritt.
 * Beispiel: LCD4_write_at(2, 1, "Temp:"); */
void LCD4_write_at(int zeile, int spalte, const char *data);

/* Schreibt eine Zahl ohne Vorzeichen (0..65535), z.B. fuer Zaehler. */
void LCD4_write_uint(unsigned int data);

/* Schreibt ein Byte als zweistellige Hex-Zahl.
 * Beispiel: LCD4_write_hex(255) zeigt "FF". */
void LCD4_write_hex(unsigned char data);

/* Schreibt ein Byte als 8 Binaerstellen.
 * Beispiel: LCD4_write_bin(5) zeigt "00000101".
 * Gut, um z.B. den Zustand eines Ports (PINA) anzuzeigen. */
void LCD4_write_bin(unsigned char data);

/* Loescht nur eine Zeile (mit Leerzeichen) und setzt den Cursor an
 * deren Anfang. So kann man einen Wert aktualisieren, ohne das ganze
 * Display zu loeschen (das wuerde flackern). */
void LCD4_clear_line(int zeile);

/* Beschreibt eines der 4 Anzeige-Segmente (siehe LCD_SEG_WIDTH oben):
 *   segment 1 = Zeile 1 links,  segment 2 = Zeile 1 rechts,
 *   segment 3 = Zeile 2 links,  segment 4 = Zeile 2 rechts.
 * Der Text wird auf 6 Zeichen gekuerzt, wenn er zu lang ist, und mit
 * Leerzeichen aufgefuellt, wenn er zu kurz ist. Dadurch bleiben keine
 * Reste vom alten Wert stehen und das Display muss nicht geloescht
 * werden (kein Flackern).
 * Beispiel: LCD4_write_segment(3, "12.5C");
 * Ungueltige Segmentnummern werden ignoriert. */
void LCD4_write_segment(unsigned char segment, const char *text);

/* Wie LCD4_write_segment, aber fuer Zahlen - die Umwandlung in Text
 * uebernimmt die Funktion. Passt die Zahl nicht in 6 Zeichen, wird
 * sie hinten abgeschnitten (z.B. 1234567 -> "123456"). */
void LCD4_write_segment_int(unsigned char segment, int value);

/* Kommazahl in ein Segment, mit frei waehlbaren Nachkommastellen.
 * Beispiel: LCD4_write_segment_double(2, 21.456, 1) zeigt "21.5". */
void LCD4_write_segment_double(unsigned char segment, double value, unsigned char nachkommastellen);

/* Legt ein eigenes Zeichen an. Das Display kann 8 selbst gemalte
 * Zeichen speichern (slot 0..7). Ein Zeichen ist 5 Punkte breit und
 * 8 hoch: pattern[0] ist die oberste Zeile, in jedem Byte zaehlen nur
 * die unteren 5 Bits (1 = Punkt an).
 * Danach mit LCD4_write(slot) anzeigen.
 * Achtung: Setzt die Cursorposition auf (1,1) zurueck, danach also
 * mit LCD4_gotoxy neu positionieren. */
void LCD4_create_char(unsigned char slot, const unsigned char pattern[8]);

#endif

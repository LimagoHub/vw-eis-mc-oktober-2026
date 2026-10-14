#include <stdlib.h>
#include "lcd_lib.h"

/* Sendet ein Nibble (obere 4 Bit von nibble) und erzeugt den Enable-Puls */
static void LCD4_nibble(unsigned char nibble)
{
	LCD_DATA = (LCD_DATA & 0x0F) | (nibble & 0xF0);
	LCD_DATA |= (1<<LCD_EN);
	_delay_us(1);
	LCD_DATA &= ~(1<<LCD_EN);
	_delay_us(200);
}

void LCD4_cmd(unsigned char cmd)
{
	LCD_DATA &= ~(1<<LCD_RS);  // RS = 0 for command
	LCD4_nibble(cmd);
	LCD4_nibble(cmd << 4);
	_delay_ms(2);
}

void LCD4_write(unsigned char data)
{
	LCD_DATA |= (1<<LCD_RS);  // RS = 1 for data
	LCD4_nibble(data);
	LCD4_nibble(data << 4);
	_delay_ms(2);
}

void LCD4_init(void)
{
	LCD_DDR = 0xFF;  // Set LCD port as output
	LCD_DATA &= ~((1<<LCD_RS) | (1<<LCD_RW) | (1<<LCD_EN));
	_delay_ms(50);   // Wait for LCD to power up

	// Reset-Sequenz laut Datenblatt: dreimal 0x3, dann 0x2 (4-Bit-Modus)
	LCD4_nibble(0x30);
	_delay_ms(5);
	LCD4_nibble(0x30);
	_delay_us(150);
	LCD4_nibble(0x30);
	_delay_us(150);
	LCD4_nibble(0x20);
	_delay_ms(2);

	LCD4_cmd(0x28);  // 4-bit mode, 2 lines, 5x8 font
	LCD4_cmd(0x0C);  // Display on, cursor off
	LCD4_cmd(0x06);  // Increment cursor
	LCD4_cmd(0x01);  // Clear display
}

void LCD4_write_string(const char *data)
{
	while (*data)
	{
		LCD4_write(*data++);
	}
}

void LCD4_gotoxy(int zeile, int spalte)
{
	if (spalte < 1 || spalte > LCD_COLUMNS)
		return;
	spalte--;
	if (zeile == 1)
	{
		LCD4_cmd(0x80 + spalte);
	}
	else if (zeile == 2)
	{
		LCD4_cmd(0xC0 + spalte);
	}
}

void LCD4_clear(void)
{
	LCD4_cmd(0x01);
}

void LCD4_write_int(int data)
{
	char buffer[8];  // "-32768" + '\0'
	itoa(data, buffer, 10);
	LCD4_write_string(buffer);
}

void LCD4_write_double(double data)
{
	const int mindestZahlDerStellen = 1;
	const int nachkommastellen = 2;
	char buffer[48];  // reicht auch fuer sehr grosse double-Werte

	dtostrf(data, mindestZahlDerStellen, nachkommastellen, buffer);
	LCD4_write_string(buffer);
}

void LCD4_home(void)
{
	LCD4_cmd(0x02);
	_delay_ms(2);   // Home braucht laenger (bis 1,52 ms)
}

void LCD4_display_on(void)    { LCD4_cmd(0x0C); }
void LCD4_display_off(void)   { LCD4_cmd(0x08); }
void LCD4_cursor_on(void)     { LCD4_cmd(0x0E); }
void LCD4_cursor_off(void)    { LCD4_cmd(0x0C); }
void LCD4_cursor_blink(void)  { LCD4_cmd(0x0F); }
void LCD4_cursor_left(void)   { LCD4_cmd(0x10); }
void LCD4_cursor_right(void)  { LCD4_cmd(0x14); }
void LCD4_shift_left(void)    { LCD4_cmd(0x18); }
void LCD4_shift_right(void)   { LCD4_cmd(0x1C); }

void LCD4_write_at(int zeile, int spalte, const char *data)
{
	LCD4_gotoxy(zeile, spalte);
	LCD4_write_string(data);
}

void LCD4_write_uint(unsigned int data)
{
	char buffer[6];  // "65535" + '\0'
	utoa(data, buffer, 10);
	LCD4_write_string(buffer);
}

void LCD4_write_hex(unsigned char data)
{
	const char hex[] = "0123456789ABCDEF";
	LCD4_write(hex[data >> 4]);
	LCD4_write(hex[data & 0x0F]);
}

void LCD4_write_bin(unsigned char data)
{
	for (signed char i = 7; i >= 0; i--)
	{
		LCD4_write((data & (1<<i)) ? '1' : '0');
	}
}

void LCD4_clear_line(int zeile)
{
	LCD4_gotoxy(zeile, 1);
	for (int i = 0; i < LCD_COLUMNS; i++)
	{
		LCD4_write(' ');
	}
	LCD4_gotoxy(zeile, 1);
}

void LCD4_create_char(unsigned char slot, const unsigned char pattern[8])
{
	LCD4_cmd(0x40 | ((slot & 0x07) << 3));   // CGRAM-Adresse setzen
	for (unsigned char i = 0; i < 8; i++)
	{
		LCD4_write(pattern[i]);
	}
	LCD4_cmd(0x80);                          // zurueck zum DDRAM
}

void LCD4_write_segment(unsigned char segment, const char *text)
{
	if (segment < 1 || segment > 4)
		return;

	int zeile  = (segment <= 2) ? 1 : 2;
	int spalte = (segment & 1) ? LCD_SEG_LEFT_COL : LCD_SEG_RIGHT_COL;

	LCD4_gotoxy(zeile, spalte);
	for (unsigned char i = 0; i < LCD_SEG_WIDTH; i++)
	{
		if (*text)
			LCD4_write(*text++);
		else
			LCD4_write(' ');
	}
}

void LCD4_write_segment_int(unsigned char segment, int value)
{
	char buffer[8];  // "-32768" + '\0'
	itoa(value, buffer, 10);
	LCD4_write_segment(segment, buffer);
}

void LCD4_write_segment_double(unsigned char segment, double value, unsigned char nachkommastellen)
{
	char buffer[48];
	dtostrf(value, 1, nachkommastellen, buffer);
	LCD4_write_segment(segment, buffer);
}

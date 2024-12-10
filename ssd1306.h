#include <bcm2835.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

#ifndef __SSD1306_H__
#define __SSD1306_H__

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define COLUMNS 128
#define PAGES 8

#define TURN_ON_SCREEN 0xAF
#define TURN_OFF_SCREEN 0xAE

#define CLEAR_SCREEN 

#define CHARGE_PUMP_SETTINGS 0x8D
#define ENABLE_CHARGE_PUMP 0x14
#define DISABLE_CHARGE_PUMP 0x10

#define DISPLAY_CLOCK_DIVIDE_RATIO 0xD5
#define OSCILLATOR_FRQ 0x80

#define MULTIPLEX_RATIO 0xA8
#define DISPLAY_HEIGHT 0x3F

#define SET_DISPLAY_OFFSET 0xD3

#define SET_COLUMN 0x21
#define SET_PAGE 0x22

#define HORIZONTAL_ADDRESSING_MODE 0x20

typedef struct SSD1306{
	
	unsigned char address;
	char **screenBuffer;
	
	void (*blit)(struct SSD1306 *screen);
	void (*clear)(struct SSD1306 *screen);
	void (*draw_pixel)(struct SSD1306 *screen, char x, char y);
	void (*erease_pixel)(struct SSD1306 *screen, char x, char y);
	void (*turn_on_charge_pump)(struct SSD1306 *screen);
	void (*turn_off_charge_pump)(struct SSD1306 *screen);
	
}SSD1306;

void SSD1306_init(SSD1306 *screen, unsigned char addr);
void SSD1306_destroy(SSD1306 *screen);

/******************************** hidden function ***************************************/
	
void _SSD1306_send_command(unsigned char addr, char command);
void _SSD1306_send_data(unsigned char addr, char data);
void _SSD1306_set_page(unsigned char addr, char page);
void _SSD1306_set_column(unsigned char addr, char column);

/******************************** power function ***************************************/

void SSD1306_turn_on_charge_pump(SSD1306 *screen);
void SSD1306_turn_off_charge_pump(SSD1306 *screen);

/******************************** graphical function ***************************************/

void SSD1306_clear_screen(SSD1306 *screen);
void SSD1306_draw_pixel(SSD1306 *screen, char x, char y);
void SSD1306_erease_pixel(SSD1306 *screen, char x, char y);
void SSD1306_blit(SSD1306 *screen);
#endif

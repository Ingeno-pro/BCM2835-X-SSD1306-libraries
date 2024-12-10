#include <png.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "SSD1306.h"

#ifndef __SSD1306_WRITER_H__
#define __SSD1306_WRITER_H__

#define SMALL_CHAR_HEIGHT 8
#define SMALL_CHAR_WIDTH 4
#define SMALL_CHAR_SHEET_PATH "smallChar.png"

#define MEDIUM_CHAR_HEIGHT 16
#define MEDIUM_CHAR_WIDTH 8
#define MEDIUM_CHAR_SHEET_PATH "mediumChar.png"

#define LARGE_CHAR_HEIGHT 24
#define LARGE_CHAR_WIDTH 12
#define LARGE_CHAR_SHEET_PATH "largeChar.png"


#define LITTLE 0
#define MEDIUM 1
#define BIG 2

#define N_ASCII_CHAR 128
#define SMALL_CHAR_SPRITE_SHEET_SIZE 64
#define ASCII_TABLE_OFFSET 32

typedef struct SSD1306Writer{
	
	SSD1306 *screen;
	char ***sct;
	char ***mct;
	char ***lct;
	
	void (*draw_small_char)(struct SSD1306Writer *sw, char id, char x, char y);
	void (*draw_small_string)(struct SSD1306Writer *sw, char x, char y, char *str);
	
}SSD1306Writer;

void SSD1306Writer_init(SSD1306Writer *sw, SSD1306 *screen);
void SSD1306Writer_destroy(SSD1306Writer *sw);

/******************************** hidden function ***************************************/

void _SSD1306Writer_alloc_char_table(char ****ct, char cwidth, char cheight);

void _SSD1306Writer_alloc_small_char_table(SSD1306Writer *sw);
void _SSD1306Writer_alloc_medium_char_table(SSD1306Writer *sw);
void _SSD1306Writer_alloc_large_char_table(SSD1306Writer *sw);

void _SSD1306Writer_load_small_char_table(SSD1306Writer *sw);
void _SSD1306Writer_load_medium_char_table(SSD1306Writer *sw);
void _SSD1306Writer_load_large_char_table(SSD1306Writer *sw);

void _SSD1306Writer_free_char_table(char ***ct, char cheight);
	
void _SSD1306Writer_free_small_char_table(SSD1306Writer *sw);
void _SSD1306Writer_free_medium_char_table(SSD1306Writer *sw);
void _SSD1306Writer_free_large_char_table(SSD1306Writer *sw);

/******************************** text function ***************************************/

void SSD1306Writer_draw_small_char(SSD1306Writer *sw, char id, char x, char y);
void SSD1306Writer_draw_small_string(SSD1306Writer *sw, char x, char y, char *str);

#endif

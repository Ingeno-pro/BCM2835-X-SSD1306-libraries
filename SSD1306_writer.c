#include "SSD1306_writer.h"

void SSD1306Writer_init(SSD1306Writer *sw, SSD1306 *screen){
	
	//Init SSD1306Writer screen
	sw->screen = screen;
	sw->draw_small_char = &SSD1306Writer_draw_small_char;
	sw->draw_small_string = &SSD1306Writer_draw_small_string;
	
	//Allocate memory to char tables
	_SSD1306Writer_alloc_small_char_table(sw);
	_SSD1306Writer_alloc_medium_char_table(sw);
	_SSD1306Writer_alloc_large_char_table(sw);

	//Load char tables
	_SSD1306Writer_load_small_char_table(sw);
	//_SSD1306Writer_load_medium_char_table(sw);
	//_SSD1306Writer_load_large_char_table(sw)
	
	
}
void SSD1306Writer_destroy(SSD1306Writer *sw){
	
	//Dereference the screen
	sw->screen = NULL;
	
	//Desallocate memory to char tables
	_SSD1306Writer_free_small_char_table(sw);
	_SSD1306Writer_free_medium_char_table(sw);
	_SSD1306Writer_free_large_char_table(sw);
	
}

/******************************** hidden function ***************************************/

void _SSD1306Writer_alloc_char_table(char ****ct, char cwidth, char cheight){
	
	//Allocate memory for char table
	*ct = (char ***)malloc(sizeof(char **) * N_ASCII_CHAR);
	
	for(int j = 0 ; j < N_ASCII_CHAR ; j++){
		(*ct)[j] = (char **)malloc(sizeof(char *) * cheight);
		for(int i = 0 ; i < cheight; i++){
			(*ct)[j][i] = (char *)malloc(sizeof(char) * cwidth);
		}
	}
}
void _SSD1306Writer_alloc_small_char_table(SSD1306Writer *sw){
	
	//Allocate memory for small char table
	_SSD1306Writer_alloc_char_table(&sw->sct, SMALL_CHAR_WIDTH, SMALL_CHAR_HEIGHT);
	
}
void _SSD1306Writer_alloc_medium_char_table(SSD1306Writer *sw){
	
	//Allocate memory for medium char table
	_SSD1306Writer_alloc_char_table(&sw->mct, MEDIUM_CHAR_WIDTH, MEDIUM_CHAR_HEIGHT);

}
void _SSD1306Writer_alloc_large_char_table(SSD1306Writer *sw){
	
	//Allocate memory for large char table
	_SSD1306Writer_alloc_char_table(&sw->lct, LARGE_CHAR_WIDTH, LARGE_CHAR_HEIGHT);
	
}
void _SSD1306Writer_read_char_sheet(png_byte **row_pointers, char **sheet, int width, int height){

	//Store the image 
	for(int y = 0; y < height; y++){
 		for(int x = 0; x < width; x++){
			for(int index = 0; index < 3; index++){
				sheet[y][x] = !row_pointers[y][x*3 + index]; //Check if all the rgb components are black
				if(!sheet) break;
			}
		}
	}
	
}
void _SSD1306Writer_load_small_char_table(SSD1306Writer *sw){
	
	int width, height, sourcex, sourcey;
	png_byte **row_pointers;
	FILE *f = fopen(SMALL_CHAR_SHEET_PATH, "rb");
	
	//Check if file exist
	if(!f){
		fprintf(stderr, "Erreur : sheet small char non trouvé :(");
	}
	
	//Create png container
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);
	
	//Link png structure to file
	png_init_io(png, f);
	
	//Read meta-data
	png_read_info(png, info);
	
	width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
	
	//Allocate memory to the row_pointers
	row_pointers = (png_byte**)malloc(sizeof(png_byte *) * height);
	for(int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}
	
	//Allocate memory to the sheet container
	char **sheet = (char **)malloc(sizeof(char *) * height);
	for(int y = 0; y < height; y++) {
		sheet[y] = (char *)malloc(sizeof(char ) * width);
	}

	png_read_image(png, row_pointers);
	_SSD1306Writer_read_char_sheet(row_pointers, sheet, width, height);
	
	//Store the char into the table
	for(int id = 0; id < N_ASCII_CHAR; id++){
		sourcey = (id / (SMALL_CHAR_SPRITE_SHEET_SIZE/SMALL_CHAR_WIDTH))*SMALL_CHAR_HEIGHT;
		sourcex = (id % (SMALL_CHAR_SPRITE_SHEET_SIZE/SMALL_CHAR_WIDTH))*SMALL_CHAR_WIDTH;
		for(int y = sourcey; y < sourcey + SMALL_CHAR_HEIGHT; y++){
			for(int x = sourcex; x < sourcex + SMALL_CHAR_WIDTH; x++){
				sw->sct[id][y-sourcey][x-sourcex] = sheet[y][x];
			}
		}
	}
	
	png_destroy_read_struct(&png, &info, NULL);
    fclose(f);
}
void _SSD1306Writer_free_char_table(char ***ct, char cheight){
	
	//Desallocate memory for small char table
	for(int j = 0 ; j < N_ASCII_CHAR ; j++){
		for(int i = 0 ; i < cheight; i++){
			free(ct[j][i]);
		}
		free(ct[j]);
	}
	
	free(ct);
}
void _SSD1306Writer_free_small_char_table(SSD1306Writer *sw){
	
	//Desallocate memory for small char table
	_SSD1306Writer_free_char_table(sw->sct, SMALL_CHAR_HEIGHT);
	
}
void _SSD1306Writer_free_medium_char_table(SSD1306Writer *sw){
	
	//Desallocate memory for small char table
	_SSD1306Writer_free_char_table(sw->mct, MEDIUM_CHAR_HEIGHT);
	
}
void _SSD1306Writer_free_large_char_table(SSD1306Writer *sw){
	
	//Desallocate memory for small char table
	_SSD1306Writer_free_char_table(sw->lct, LARGE_CHAR_HEIGHT);
	
}

/******************************** text function ***************************************/

void SSD1306Writer_draw_small_char(SSD1306Writer *sw, char id, char x, char y){
	
	if((y + SMALL_CHAR_HEIGHT) >= SCREEN_HEIGHT || (x + SMALL_CHAR_WIDTH) >= SCREEN_WIDTH){
		fprintf(stderr, "Error : invalid screen coordinate");
		exit(0);
	}
			
	for(char i = 0; i < SMALL_CHAR_HEIGHT; i++){
		for(char j = 0; j < SMALL_CHAR_WIDTH; j++){
			(sw->sct[id][i][j]) ? SSD1306_draw_pixel(sw->screen, x+j, y+i) : SSD1306_erease_pixel(sw->screen, x+j, y+i);
		}
	}
	
}
void SSD1306Writer_draw_small_string(SSD1306Writer *sw, char x, char y, char *str){
	
	for(int i = 0; i < strlen(str); i++){
		SSD1306Writer_draw_small_char(sw, str[i] - ASCII_TABLE_OFFSET, x + i*SMALL_CHAR_WIDTH, y);
	}
}
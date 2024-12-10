#include "SSD1306.h"

void SSD1306_init(SSD1306 *screen, unsigned char addr){
	
	//Allocate memory for the screen buffer
	screen->screenBuffer = (char **)malloc(sizeof(char *) * COLUMNS);
	for(int i = 0; i < COLUMNS ; i++){
		screen->screenBuffer[i] = (char *)malloc(sizeof(char) * PAGES);
	}

	//Init screen structure
	screen->address = addr;
	screen->blit = &SSD1306_blit;
	screen->clear = &SSD1306_clear_screen;
	screen->draw_pixel = &SSD1306_draw_pixel;
	screen->erease_pixel = &SSD1306_erease_pixel;
	screen->turn_on_charge_pump = &SSD1306_turn_on_charge_pump;
	screen->turn_off_charge_pump = &SSD1306_turn_off_charge_pump;
	
	//Turn off the screen 
	_SSD1306_send_command(screen->address, TURN_OFF_SCREEN);
	
	//Clock frequency
	_SSD1306_send_command(screen->address, DISPLAY_CLOCK_DIVIDE_RATIO);
	_SSD1306_send_command(screen->address, OSCILLATOR_FRQ);
	
	//Give display size to SSD1306
	_SSD1306_send_command(screen->address, MULTIPLEX_RATIO); 
	_SSD1306_send_command(screen->address, DISPLAY_HEIGHT); //Display height from 0 
	
	//Set display offset 0
	_SSD1306_send_command(screen->address, SET_DISPLAY_OFFSET); 
	_SSD1306_send_command(screen->address, 0x00); 
	
	//Set addressing mode 
	_SSD1306_send_command(screen->address, HORIZONTAL_ADDRESSING_MODE);
		
	//Blank screen
	SSD1306_clear_screen(screen);
	
	//Power on the screen
	SSD1306_turn_on_charge_pump(screen);
	_SSD1306_send_command(screen->address, TURN_ON_SCREEN);
	
}
void SSD1306_destroy(SSD1306 *screen){
	
	//Desallocate memory for the screen buffer
	for(char i = 0; i < COLUMNS ; i++){
		free(screen->screenBuffer[i]);
	}
	free(screen->screenBuffer);
	
	//Turn off the screen
	_SSD1306_send_command(screen->address, TURN_OFF_SCREEN);
	SSD1306_turn_off_charge_pump(screen);
}

/******************************** hidden function ***************************************/
void _SSD1306_send_command(unsigned char addr, char command){
	
	bcm2835_i2c_setSlaveAddress(addr);
	char i2c_command[2] = {0x00, command};
	bcm2835_i2c_write(i2c_command, 2);
	
}
void _SSD1306_send_data(unsigned char addr, char data){
	
	bcm2835_i2c_setSlaveAddress(addr);
	char i2c_command[2] = {0x40, data};
	bcm2835_i2c_write(i2c_command, 2);
	
}
void _SSD1306_set_page(unsigned char addr, char page){
	
	bcm2835_i2c_setSlaveAddress(addr);
	char i2c_command[4] = {0x00, SET_PAGE, page, PAGES-1};
	bcm2835_i2c_write(i2c_command, 4);
	
}
void _SSD1306_set_column(unsigned char addr, char column){
	
	bcm2835_i2c_setSlaveAddress(addr);
	char i2c_command[4] = {0x00, SET_COLUMN, column, COLUMNS-1};
	bcm2835_i2c_write(i2c_command, 4);
	
}
char _SSD1306_get_pixelmask(char y){
	
	return pow(2, (2*(y%(SCREEN_HEIGHT/PAGES))));
}
char _SSD1306_get_page(char y){
	
	return y/(SCREEN_HEIGHT/PAGES);
}
/******************************** service function ***************************************/
void SSD1306_turn_on_charge_pump(SSD1306 *screen){
	
	bcm2835_i2c_setSlaveAddress(screen->address);
	char i2c_command[3] = {0x00, CHARGE_PUMP_SETTINGS, ENABLE_CHARGE_PUMP};
	bcm2835_i2c_write(i2c_command, 3);
	
}
void SSD1306_turn_off_charge_pump(SSD1306 *screen){
	
	bcm2835_i2c_setSlaveAddress(screen->address);
	char i2c_command[3] = {0x00, CHARGE_PUMP_SETTINGS, DISABLE_CHARGE_PUMP};
	bcm2835_i2c_write(i2c_command, 3);
	
}
/******************************** graphical function ***************************************/
void SSD1306_clear_screen(SSD1306 *screen){
	
	for(char i = 0;  i < PAGES ; i++){
		for(char j = 0;  j < COLUMNS ; j++){
			screen->screenBuffer[j][i] = 0x00;
		}
	}
}
void SSD1306_draw_pixel(SSD1306 *screen, char x, char y){
	
	//Check if the coordinate are correct
	if(x >= SCREEN_WIDTH || y >=SCREEN_HEIGHT){
		fprintf(stderr, "Error : invalid screen coordinate");
		exit(0);
	}
	
	screen->screenBuffer[x][_SSD1306_get_page(y)] |= _SSD1306_get_pixelmask(y);
	
}
void SSD1306_erease_pixel(SSD1306 *screen, char x, char y){
	
	//Check if the coordinate are correct
	if(x >= SCREEN_WIDTH || y >=SCREEN_HEIGHT){
		fprintf(stderr, "Error : invalid screen coordinate");
		exit(0);
	}
	
	screen->screenBuffer[x][_SSD1306_get_page(y)] &= ~_SSD1306_get_pixelmask(y);
	
}
void SSD1306_blit(SSD1306 *screen){
	
	//Copy the screenBuffer into the screen VRAM
	for(char i = 0;  i < PAGES ; i++){
		for(char j = 0;  j < COLUMNS ; j++){
			_SSD1306_set_page(screen->address, i);
			_SSD1306_set_column(screen->address, j);
			_SSD1306_send_data(screen->address, screen->screenBuffer[j][i]);
		}
	}
	
}

#include <os.h>
#include "lcd.h"
int cur_row = 0;
int cur_col = 0;
int row_offset = 0;
int n_bits = 8;
uint8_t auto_mode = AUTO_DOWN;
uint8_t enabled = TRUE;
uint8_t video_mem[120*64/8];
uint8_t *framebuffer;

#define XY_TO_IDX(x, y) (y * 120 + x)
#define SET_PX_XY(x, y, v) set_pixel(XY_TO_IDX(x, y), v)
#define GET_PX_XY(x, y) get_pixel(XY_TO_IDX(x, y))
#define MAX_COL ((n_bits == 8) ? 14 : 19)

#define N_XY_TO_IDX(x, y) ((y * 320 + x) >> 1)
#define printf(...)

void lcd_init(){
	framebuffer = SCREEN_BASE_ADDRESS;
	lcd_ingray();
	memset(framebuffer, 0xff, 320*240/2);
}
static void n_set_pixel(int x, int y, uint8_t v){
	uint8_t *base = framebuffer + N_XY_TO_IDX(x, y);
	if(!(x & 1)) v <<= 4;
	*base &= ~(x & 1 ? 0x0f : 0xf0);
	*base |= v;
}
static void n_set_84_pixel(int x, int y, uint8_t gray){
	int dx, dy;
	for(dx = 0; dx < 3; dx++){
		for(dy = 0; dy < 3; dy++){
			n_set_pixel(x*3+dx, y*3+dy, gray);
		}
	}
}
static void set_pixel(int x, int y, uint8_t val){
	printf("set_pixel %d %d %d\n", x, y, val);
	if(y < 64 && x < 96) n_set_84_pixel(x, y, val ? 0x0 : 0xf);
	int n = XY_TO_IDX(x, y);
	if(val){
		video_mem[n >> 3] |= 1<<(n & 0b111);
	}else{
		video_mem[n >> 3] &= ~(n & 0b111);
	}
}
static uint8_t get_pixel(int x, int y){
	int n = XY_TO_IDX(x, y);
	return (video_mem[n >> 3] & 1<<(n & 0b111)) ? 1 : 0;
}

void lcd_cmd(uint8_t cmd){
	if(cmd >= 0x20 && cmd <= 0x3F){
		cur_col = cmd - 0x20;
		printf("col %d\n", cur_col);
		return;
	}
	if(cmd >= 0x80 && cmd <= 0xBF){
		cur_row = cmd - 0x80;
		printf("row %d\n", cur_row);
	}
	if(cmd >= 0xC0){
		// contrast
		return;
	}
	switch(cmd){
		case BIT_6:
		case BIT_8:
			n_bits = cmd ? 8 : 6;
			printf("%d bits\n", n_bits);
		break;
		case LCD_ENABLE:
			enabled = TRUE;
			puts("enabled\n");
		break;
		case LCD_DISABLE:
			enabled = FALSE;
			puts("disabled\n");
		break;
		case AUTO_UP:
		case AUTO_DOWN:
		case AUTO_LEFT:
		case AUTO_RIGHT:
			printf("auto %d\n", cmd);
			auto_mode = cmd;
		break;
	}
}

uint8_t lcd_cmd_read(){
	uint8_t v = (auto_mode == AUTO_RIGHT || auto_mode == AUTO_DOWN);
	v |= (auto_mode == AUTO_LEFT || auto_mode == AUTO_RIGHT) << 1;
	v |= enabled << 5;
	v |= (n_bits == 8) << 6;
	return v;
}
void lcd_data(uint8_t data){
	int i;
	int x = cur_col * n_bits;
	int y = cur_row;
	for(i = 0; i < n_bits; i++){
		set_pixel(x + i, y & 0x3f, data & (1<<(n_bits-1-i)));
	}
	switch(auto_mode){
		case AUTO_UP:
			cur_row--;
		break;
		case AUTO_DOWN:
			cur_row++;
		break;
		case AUTO_LEFT:
			cur_col--;
			if(cur_col == -1) cur_col = MAX_COL;
		break;
		case AUTO_RIGHT:
			cur_col++;
			if(cur_col == MAX_COL + 1) cur_col = 0;
		break;
	}
}

uint8_t lcd_data_read(){
	uint8_t t = 0;
	int x = cur_col * n_bits;
	int y = cur_row;
	int i;
	for(i = 0; i < 8; i++){
		t |= get_pixel(x + i, y) << i;
	}
	return t;
}
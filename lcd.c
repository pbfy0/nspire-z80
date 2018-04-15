#include <os.h>
#include "lcd.h"
#include "util.h"
#include "_syscalls.h"

struct lcd_state {
	int cur_row;
	int cur_col;
	int row_offset;
	int n_bits;
	uint8_t auto_mode;
	uint8_t enabled;
	uint8_t lcd_read_reg;
	uint8_t contrast;
};

struct lcd_state ls = { 0, 0, 0, 8, AUTO_DOWN, TRUE, 0, 0xff };
//uint8_t video_mem[120*64/8];
uint8_t *framebuffer;
uint8_t *fbp;
uint8_t *bfb;
static uint32_t * const palette = (uint32_t *)0xC0000200;
static uint32_t * lcd_control;
static unsigned is_hww = 0;
static unsigned xy_to_fbo(unsigned x, unsigned y){
	if(is_hww) {
		return x * 240 + y;
	} else {
		return y * 320 + x;
	}
}

//#define XY_TO_IDX(x, y) ((y) * 120 + (x))
//#define XY_TO_FBO(x, y) ((y) * 320 + (x))
#define MAX_COL ((ls.n_bits == 8) ? 14 : 19)
#define C_XO ((320-(96*3))/2)
#define C_YO ((240-(64*3))/2)

//#define C_OFFSET XY_TO_FBO(C_XO, C_YO)

unsigned c_offset;

typedef uint8_t * byteptr;

//#define FB_OFFSET(x, y) (((y) * 320 + (x)) >> 1)
//#define printf(...)



static uint16_t pack_rgb(uint8_t r, uint8_t g, uint8_t b){
	return b >> 3 | g >> 3 << 5 | r >> 3 << 10;
}

static uint16_t pack_rgbp(uint32_t rgb){
	return pack_rgb(rgb >> 16, (rgb >> 8) & 0xff, rgb & 0xff);
}
static uint16_t pack_gry(uint8_t v) {
	uint8_t z = (v & 4) >> 2;
	v >>= 3;
	return v | (v << 5) | (v << 10) | (z << 15);
}
__attribute__((naked)) uint8_t get_lcd_type() {
asm(
"	swi #0x20000e\n"
"	bx lr\n"
);
}

typedef void (*n_set_84_pixel_t)(int x, int y, uint8_t gray, uint32_t fb_a);

void _n_set_84_pixel(int x, int y, uint8_t gray, uint32_t fb_a);
void _n_set_84_pixel_hww(int x, int y, uint8_t gray, uint32_t fb_a);

n_set_84_pixel_t correct_setpixel;

void m_lcd_init(){
	
	is_hww = _lcd_type() == SCR_240x320_565;
	lcd_control = IO_LCD_CONTROL;
	
	fbp = malloc(320*240 + 8); //SCREEN_BASE_ADDRESS;
	framebuffer = (((intptr_t)fbp) | 7) + 1;
	//lcd_ingray();
	memset(framebuffer, 0xff, 320*240);
	int x, y, i = 0;
	for(y = 0; y < 240; y++) {
		for(x = 0; x < 320; x++) {
			if(y > C_YO && y < (240 - C_YO) && x == C_XO) {
				memset(framebuffer+i, 0, 96*3);
				i += 96*3;
				x += 96*3;
			}
			framebuffer[i++] = ((x & 1) ^ (y & 1)) + 4;
		}
	}
	
	*lcd_control = (*lcd_control & ~0b1110) | 0b0110; // 8 bpp, palette
	
	bfb = REAL_SCREEN_BASE_ADDRESS;
	REAL_SCREEN_BASE_ADDRESS = framebuffer;
	
	c_offset = xy_to_fbo(C_XO, C_YO);
	correct_setpixel = is_hww ? _n_set_84_pixel_hww : _n_set_84_pixel;
	
	palette[0] = pack_gry(0xff);
	palette[1] = pack_rgbp(0xff0000);
	palette[2] = 0xffff0000;
}

void lcd_end(){
	REAL_SCREEN_BASE_ADDRESS = bfb;
	*lcd_control = (*lcd_control & ~0b1110) | 0b1100; // 8 bpp, palette
	free(fbp);
}
asm(
"\n"
".align 2\n"
"px_offsets:\n\t"
	".hword 0, 1, 2, 320, 321, 322, 640, 641, 642\n\t"
"px_offsets_hww:\n\t"
	".hword 0, 1, 2, 240, 241, 242, 480, 481, 482\n\t"
);


__attribute__((naked)) void _n_set_84_pixel(int x, int y, uint8_t gray, uint32_t fb_a) {
	(void)x;
	(void)y;
	(void)gray;
	(void)fb_a;
asm(
"	push {r4, lr}\n"
"	mov r4, #320\n"
"	mla r3, r1, r4, r3\n"
"	add r3, r0\n"
"	adr r4, px_offsets\n"
"	mov r0, #18\n"
"1:	subs r0, #2\n"
"	ldrh r1, [r4, r0]\n"
"	strb r2, [r3, r1]\n"
"	bne 1b\n"
"	pop {r4, pc}\n"
);
}

__attribute__((naked)) void _n_set_84_pixel_hww(int x, int y, uint8_t gray, uint32_t fb_a) {
	(void)x;
	(void)y;
	(void)gray;
	(void)fb_a;
asm(
"	push {r4, lr}\n"
"	mov r4, #240\n"
"	mla r3, r0, r4, r3\n"
"	add r3, r1\n"
"	adr r4, px_offsets_hww\n"
"	mov r0, #18\n"
"1:	subs r0, #2\n"
"	ldrh r1, [r4, r0]\n"
"	strb r2, [r3, r1]\n"
"	bne 1b\n"
"	pop {r4, pc}\n"
);
}

static void n_set_84_pixel(int x, int y, uint8_t gray){
	correct_setpixel(x*3, y*3, gray, (uint32_t)framebuffer + c_offset);
}
static void set_pixel(int x, int y, uint8_t val){
	//printf("set_pixel %d %d %d\n", x, y, val);
	if(y < 64 && x < 96) n_set_84_pixel(x, y, val ? 1 : 0);
}
static uint8_t get_pixel(int x, int y){
	return framebuffer[c_offset + xy_to_fbo(x*3, y*3)] && 1;
}

void set_contrast(uint8_t contrast){
	//printf("set_contrast %d\n", set_contrast);
	int black = contrast * 2;
	int white = 0xff - contrast * 2;
	palette[0] = (pack_gry(black) << 16) | pack_gry(white);
	printf("%d %d %d %08x\n", contrast, black, white, palette[0]);
	ls.contrast = contrast;
}

void lcd_cmd(uint8_t cmd){
	if(cmd >= 0x20 && cmd <= 0x3F){
		ls.cur_col = cmd - 0x20;
		//printf("col %d\n", ls.cur_col);
		return;
	}
	if(cmd >= 0x80 && cmd <= 0xBF){
		ls.cur_row = cmd - 0x80;
		//printf("row %d\n", ls.cur_row);
	}
	if(cmd >= 0xC0){
		set_contrast(0xff - cmd);
		// contrast
		return;
	}
	switch(cmd){
		case BIT_6:
			ls.n_bits = 6;
			//printf("6 bit mode\n");
		break;
		case BIT_8:
			ls.n_bits = 8;
			//printf("8 bit mode\n");
			//printf("%d bits\n", ls.n_bits);
		break;
		case LCD_ENABLE:
			ls.enabled = TRUE;
			//puts("ls.enabled");
		break;
		case LCD_DISABLE:
			ls.enabled = FALSE;
			//puts("disabled");
		break;
		case AUTO_UP:
		case AUTO_DOWN:
		case AUTO_LEFT:
		case AUTO_RIGHT:
			//printf("auto %d\n", cmd);
			ls.auto_mode = cmd;
		break;
	}
}

uint8_t lcd_cmd_read(){
	uint8_t v = (ls.auto_mode == AUTO_RIGHT || ls.auto_mode == AUTO_DOWN);
	v |= (ls.auto_mode == AUTO_LEFT || ls.auto_mode == AUTO_RIGHT) << 1;
	v |= ls.enabled << 5;
	v |= (ls.n_bits == 8) << 6;
	return v;
}

void lcd_auto_move(){
	switch(ls.auto_mode){
		case AUTO_UP:
			ls.cur_row--;
		break;
		case AUTO_DOWN:
			ls.cur_row++;
		break;
		case AUTO_LEFT:
			ls.cur_col--;
			if(ls.cur_col == -1) ls.cur_col = MAX_COL;
		break;
		case AUTO_RIGHT:
			ls.cur_col++;
			if(ls.cur_col == MAX_COL + 1) ls.cur_col = 0;
			if(ls.cur_col == 32) ls.cur_col = 0;
		break;
	}

}
void lcd_data(uint8_t data){
	int i;
	int x = ls.cur_col * ls.n_bits;
	int y = ls.cur_row;
	for(i = 0; i < ls.n_bits; i++){
		set_pixel(x + i, y & 0x3f, data & (1<<(ls.n_bits-1-i)));
	}
	lcd_auto_move();
	/*if(data && !isKeyPressed(KEY_84_ALPHA)){
		while(!isKeyPressed(KEY_84_2ND));
		while(isKeyPressed(KEY_84_2ND));
	}*/
}

uint8_t lcd_read_reg = 0;

uint8_t _lcd_data_read(){
	int x = ls.cur_col * ls.n_bits;
	int y = ls.cur_row;
	int i;
	uint8_t v = 0;
	for(i = 0; i < ls.n_bits; i++){
		v <<= 1;
		v |= get_pixel(x + i, y);
	}
	lcd_auto_move();
	//t <<= (8 - ls.n_bits);
	//if(ls.n_bits == 8) ls.cur_row++;//lcd_auto_move();
	//if(t) printf("hi\n");
	return v;
}

uint8_t lcd_data_read() {
	uint8_t v = ls.lcd_read_reg;
	ls.lcd_read_reg = _lcd_data_read();
	return v;
}

void lcd_save(FILE *f){
	FWRITE_VALUE(ls, f);
	struct lcd_state bls = ls;
	ls.cur_col = 0;
	ls.cur_row = 0;
	ls.n_bits = 8;
	ls.auto_mode = AUTO_RIGHT;
	uint8_t *b = malloc(768);
	uint8_t *p = b;
	int i;
	for(i = 0; i < 64; i++){
		int j;
		for(j = 0; j < 96/8; j++){
			*(p++) = _lcd_data_read();
		}
		ls.cur_row++;
		ls.cur_col = 0;
	}
	fwrite(b, 768, 1, f);
	free(b);
	ls = bls;
}

void lcd_restore(FILE *f){
	struct lcd_state nls;
	FREAD_VALUE(&nls, f);
	ls.cur_col = 0;
	ls.cur_row = 0;
	ls.n_bits = 8;
	ls.auto_mode = AUTO_RIGHT;
	uint8_t *b = malloc(768);
	fread(b, 768, 1, f);
	uint8_t *p = b;
	int i;
	for(i = 0; i < 64; i++){
		int j;
		for(j = 0; j < 96/8; j++){
			lcd_data(*(p++));
		}
		ls.cur_row++;
		ls.cur_col = 0;
	}
	free(b);
	ls = nls;
	
	set_contrast(ls.contrast);
}
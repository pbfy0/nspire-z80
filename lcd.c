#include <os.h>
#include "lcd.h"
#include "util.h"
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
uint8_t *bfb;
volatile uint32_t *palette = (uint32_t *)0xC0000200;

#define XY_TO_IDX(x, y) ((y) * 120 + (x))
#define XY_TO_FBO(x, y) ((y) * 320 + (x))
#define MAX_COL ((ls.n_bits == 8) ? 14 : 19)
#define C_XO ((320-(96*3))/2)
#define C_YO ((240-(64*3))/2)

#define C_OFFSET XY_TO_FBO(C_XO, C_YO)

typedef uint8_t * byteptr;

//#define FB_OFFSET(x, y) (((y) * 320 + (x)) >> 1)
//#define printf(...)

static uint16_t pack_rgb(uint8_t r, uint8_t g, uint8_t b){
	return b >> 3 | g >> 3 << 5 | r >> 3 << 10;
}

static uint16_t pack_rgbp(uint32_t rgb){
	return pack_rgb(rgb >> 16, (rgb >> 8) & 0xff, rgb & 0xff);
}

void lcd_init(){
	//asm(" b .");
	framebuffer = malloc(320*240); //SCREEN_BASE_ADDRESS;
	//lcd_ingray();
	memset(framebuffer, 0xff, 320*240);
	*IO_LCD_CONTROL &= (unsigned)~0b00001110;
	*IO_LCD_CONTROL |= (unsigned)0b00000110; // 8 bpp, palette
	
	bfb = *(volatile byteptr *)0xC0000010;
	*(volatile byteptr *)0xC0000010 = framebuffer;
	
	
	palette[0] = 0x0000ffff;
	palette[1] = pack_rgbp(0xff0000);
	//palette[1] = 0xffff;
	palette[0xff >> 1] = pack_rgbp(0xaaaaaa) << 16; //0b0101011010110101 << 16;//
	int y;
	for(y = 0; y < 64*3; y++){
		memset(framebuffer + XY_TO_FBO(C_XO, y+C_YO), 0, 96*3);
	}
}

void lcd_end(){
	//IO_LCD_CONTROL &= ~0b00001110;
	//IO_LCD_CONTROL |= 0b00000100; // 16 bit color
	*(volatile byteptr *)0xC0000010 = bfb;
	lcd_incolor();
	free(framebuffer);
}
/*static void n_set_pixel(int x, int y, uint8_t v){
	uint8_t *base = framebuffer + FB_OFFSET(x, y);
	if(!(x & 1)) v <<= 4;
	*base &= ~(x & 1 ? 0x0f : 0xf0);
	*base |= v;
}*/
//uint16_t px_offsets[9] = {0, 1, 2, 320, 321, 322, 640, 641, 642};
asm(
"\n"
".align 2\n"
"px_offsets:\n\t"
	".hword 0, 1, 2, 320, 321, 322, 640, 641, 642\n\t"
);

void _n_set_84_pixel(int x, int y, uint8_t gray, uint32_t fb_a);
asm(
"\n"
".align 4\n"
"_n_set_84_pixel:\n"
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
/*asm(
"\n"
"_n_set_84_pixel:\n\t"
	"stmfd sp!, {r4-r8}\n\t"
	"mov r4, #480\n\t" // 320 * 3 / 2
	"mla r3, r1, r4, r3\n\t"
	"mov r4, #3\n\t"
	"mul r4, r0, r4\n\t"
	"add r3, r3, r4, asr #1\n\t"
	"mov r4, r3\n\t"
	"mov r5, #15\n\t"
	"mov r6, r2, lsl #4\n\t"
	"orr r6, r2\n\t"
	"tst r0, #1\n\t"
	"addne r3, #1\n\t"
	"addeq r4, #1\n\t"
	"lsleq r5, #4\n\t"
	"lsleq r2, #4\n\t"
	"mov r7, #3\n" // no \t
"loop_begin:\n\t"
	"strb r6, [r3]\n\t"
	"ldrb r8, [r4]\n\t"
	"bic r8, r5\n\t"
	"orr r8, r2\n\t"
	"strb r8, [r4]\n\t"
	"add r3, r3, #160\n\t"
	"add r4, r4, #160\n\t"
	"subs r7, r7, #1\n\t"
	"bne loop_begin\n\t"
	"ldmfd sp!, {r4-r8}\n\t"
	"bx lr\n\t"
);*/
static void n_set_84_pixel(int x, int y, uint8_t gray){
	_n_set_84_pixel(x*3, y*3, gray, (uint32_t)framebuffer + C_OFFSET);
	/*int dy;
	x *= 3;
	y *= 3;
	uint8_t d = gray << 4 | gray;
	if(x & 1){
		for(dy = 0; dy < 3; dy++){
			int o = FB_OFFSET(x, y+dy) + C_OFFSET;
			framebuffer[o+1] = d;
			framebuffer[o] &= ~0x0f;
			framebuffer[o] |= gray;
		}
	}else{
		for(dy = 0; dy < 3; dy++){
			int o = FB_OFFSET(x, y+dy) + C_OFFSET;
			framebuffer[o] = d;
			framebuffer[o+1] &= ~0xf0;
			framebuffer[o+1] |= gray << 4;
		}
	}*/
}
static void set_pixel(int x, int y, uint8_t val){
	//printf("set_pixel %d %d %d\n", x, y, val);
	if(y < 64 && x < 96) n_set_84_pixel(x, y, val ? 1 : 0);
	//int n = XY_TO_IDX(x, y);
}
static uint8_t get_pixel(int x, int y){
	//int n = XY_TO_IDX(x, y);
	return framebuffer[C_OFFSET + XY_TO_FBO(x*3, y*3)] && 1;//(video_mem[n >> 3] & 1<<(n & 0b111)) ? 1 : 0;
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
		int black = (cmd - 0xC0) >> 1;
		int white = 0xff - (black >> 1);
		palette[0] = pack_rgb(black, black, black) << 16 | pack_rgb(white, white, white);
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

uint8_t lcd_data_read(){
	int x = ls.cur_col * ls.n_bits;
	int y = ls.cur_row;
	int i;
	uint8_t retval = ls.lcd_read_reg;
	ls.lcd_read_reg = 0;
	for(i = 0; i < ls.n_bits; i++){
		ls.lcd_read_reg <<= 1;
		ls.lcd_read_reg |= get_pixel(x + i, y);
	}
	lcd_auto_move();
	//t <<= (8 - ls.n_bits);
	//if(ls.n_bits == 8) ls.cur_row++;//lcd_auto_move();
	//if(t) printf("hi\n");
	return retval;
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
			*(p++) = lcd_data_read();
		}
		ls.cur_row++;
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
	}
	free(b);
	ls = nls;
}
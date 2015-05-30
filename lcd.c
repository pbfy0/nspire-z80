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
#define C_XO ((320-(96*3))/2)
#define C_YO ((240-(64*3))/2)

#define C_OFFSET FB_OFFSET(C_XO, C_YO)

#define FB_OFFSET(x, y) (((y) * 320 + (x)) >> 1)
//#define printf(...)

void lcd_init(){
	framebuffer = SCREEN_BASE_ADDRESS;
	lcd_ingray();
	memset(framebuffer, 0xaa, 320*240/2);
	int y;
	for(y = 0; y < 64*3; y++){
		memset(framebuffer + FB_OFFSET(C_XO, y+C_YO), 0xff, 96*3/2);
	}
}
/*static void n_set_pixel(int x, int y, uint8_t v){
	uint8_t *base = framebuffer + FB_OFFSET(x, y);
	if(!(x & 1)) v <<= 4;
	*base &= ~(x & 1 ? 0x0f : 0xf0);
	*base |= v;
}*/
void _n_set_84_pixel(int x, int y, uint8_t gray, uint32_t fb_a);
asm(
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
);
static void n_set_84_pixel(int x, int y, uint8_t gray){
	_n_set_84_pixel(x, y, gray, (uint32_t)framebuffer + C_OFFSET);
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
		//printf("col %d\n", cur_col);
		return;
	}
	if(cmd >= 0x80 && cmd <= 0xBF){
		cur_row = cmd - 0x80;
		//printf("row %d\n", cur_row);
	}
	if(cmd >= 0xC0){
		// contrast
		return;
	}
	switch(cmd){
		case BIT_6:
		case BIT_8:
			n_bits = cmd ? 8 : 6;
			//printf("%d bits\n", n_bits);
		break;
		case LCD_ENABLE:
			enabled = TRUE;
			//puts("enabled");
		break;
		case LCD_DISABLE:
			enabled = FALSE;
			//puts("disabled");
		break;
		case AUTO_UP:
		case AUTO_DOWN:
		case AUTO_LEFT:
		case AUTO_RIGHT:
			//printf("auto %d\n", cmd);
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
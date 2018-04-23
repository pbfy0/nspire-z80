#include <os.h>
#include "lcd.h"
#include "util.h"
#include "_syscalls.h"
#include "mmu_mmap.h"
#include "aligned_alloc.h"

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

static const unsigned char power_btn[20][3] = {
	{0x00,0x60,0x00},
	{0x00,0x60,0x00},
	{0x00,0x60,0x00},
	{0x06,0x66,0x00},
	{0x0e,0x67,0x00},
	{0x1c,0x63,0x80},
	{0x38,0x61,0xc0},
	{0x30,0x60,0xc0},
	{0x60,0x00,0x60},
	{0x60,0x00,0x60},
	{0x60,0x00,0x60},
	{0x60,0x00,0x60},
	{0x60,0x00,0x60},
	{0x60,0x00,0x60},
	{0x30,0x00,0xc0},
	{0x38,0x01,0xc0},
	{0x1c,0x03,0x80},
	{0x0e,0x07,0x00},
	{0x07,0xfe,0x00},
	{0x01,0xf8,0x00}
};

struct lcd_state ls = { 0, 0, 0, 8, AUTO_DOWN, TRUE, 0, 0 };
//uint8_t video_mem[120*64/8];

void *os_framebuffer;
struct buf_p {
	uint8_t *phys;
	uint8_t *virt;
};

struct buf_p back_buffer;
aligned_ptr framebuffer_a;
#ifdef LCD_DOUBLE_BUFFER
struct buf_p front_buffer;
aligned_ptr framebuffer_b;
#endif
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

#ifdef LCD_DOUBLE_BUFFER
static void swap_buffers();
static uint32_t b_int;
#endif

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

#ifdef LCD_DOUBLE_BUFFER
struct pixel_write {
	uint8_t x;
	uint8_t y : 7;
	uint8_t v : 1;
};

struct pixel_write write_buffer[1024];
unsigned write_idx = 0;
unsigned wr_overflow = 0;
#endif

static void fb_setup(uint8_t *buf) {
	int x, y, i = 0;
	for(y = 0; y < 240; y++) {
		for(x = 0; x < 320; x++) { 
			if(y > C_YO && y < (240 - C_YO) && x == C_XO) {
				memset(buf+i, 0, 96*3);
				i += 96*3;
				x += 96*3;
			}
			buf[i++] = ((x & 1) ^ (y & 1)) + 4 +
				(
					(y >= 2 && y < 22 && x >= C_XO && x < (C_XO+20) &&
						(power_btn[y-2][(x-C_XO) >> 3] & 1<<(7-((x-C_XO)&7)))) ? 2 : 0
				);
		}
	}
}

static uint32_t b_lcd_control;

void m_lcd_init(){
	is_hww = nl_ndless_rev() >= 2004 && _lcd_type() == SCR_240x320_565;
	lcd_control = IO_LCD_CONTROL;
	
	
	framebuffer_a = x_aligned_alloc(0x1000, FB_SIZE);
	map_framebuffer(framebuffer_a.ptr);
	//framebuffer_b = x_aligned_alloc(8, 320*240);
#ifdef LCD_DOUBLE_BUFFER
	front_buffer = (struct buf_p){framebuffer_a.ptr, 0xe0050000};
	back_buffer = (struct buf_p){((uint8_t *)front_buffer.phys) + 320*240, 0xe0050000 + 320*240};
	fb_setup(front_buffer.virt);
#else
	back_buffer = (struct buf_p){framebuffer_a.ptr, 0xe0050000};
#endif
	
	fb_setup(back_buffer.virt);
	
	//front_buffer[0] = front_buffer[1] = front_buffer[320] = front_buffer[321] = 2;
	//back_buffer[318] = back_buffer[319] = back_buffer[320+318] = back_buffer[320+319] = 2;
	//lcd_ingray();
	//memset(framebuffer, 0xff, 320*240);
	
	b_lcd_control = *lcd_control;
	*lcd_control = (*lcd_control & ~0b1110) | 0b0110; // 8 bpp, palette.
#ifdef LCD_DOUBLE_BUFFER
	*lcd_control = (*lcd_control & ~(0b11 << 12)) | (0b00 << 12); // Interrupt on VSync
	b_int = *(uint32_t *)0xc000001c;
	*(uint32_t *)0xc000001c = 1<<3; // v compare interrupt
#endif
	
	os_framebuffer = REAL_SCREEN_BASE_ADDRESS;
#ifdef LCD_DOUBLE_BUFFER
	REAL_SCREEN_BASE_ADDRESS = front_buffer.phys;
#else
	REAL_SCREEN_BASE_ADDRESS = back_buffer.phys;
#endif
	
	c_offset = xy_to_fbo(C_XO, C_YO);
	correct_setpixel = is_hww ? _n_set_84_pixel_hww : _n_set_84_pixel;
	
	palette[0] = pack_gry(0xff);
	palette[1] = pack_rgbp(0xff0000) * 0x10001;
	palette[2] = 0xffff0000;
	palette[3] = palette[2];
}

void lcd_end(){
#ifdef LCD_DOUBLE_BUFFER
	*(uint32_t *)0xc000001c = b_int;
#endif
	REAL_SCREEN_BASE_ADDRESS = os_framebuffer;
	*lcd_control = b_lcd_control;
	x_aligned_free(framebuffer_a);
	//x_aligned_free(framebuffer_b);
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

static void n_set_84_pixel(int x, int y, uint8_t gray, uint8_t *buf){
	correct_setpixel(x*3, y*3, gray, (uint32_t)buf + c_offset);
}

static uint8_t get_pixel(int x, int y){
	return back_buffer.virt[c_offset + xy_to_fbo(x*3, y*3)] && 1;
}

static void set_pixel(int x, int y, uint8_t val, uint8_t *buf){
	//printf("set_pixel %d %d %d\n", x, y, val);
	unsigned vv = val ? 1 : 0;
	if(get_pixel(x, y) == vv) return;
	if(y < 64 && x < 96) {
		n_set_84_pixel(x, y, vv, buf);
#ifdef LCD_DOUBLE_BUFFER
		if(write_idx == sizeof(write_buffer) / sizeof(write_buffer[0])) {
			wr_overflow = 1;
			return;
		}
		write_buffer[write_idx++] = (struct pixel_write){ .x = x, .y = y, .v = vv };
#endif
	}
}

#ifdef LCD_DOUBLE_BUFFER
void lcd_int() {
	*(uint32_t *)0xc0000028 = 1<<3; // acknowledge interrupt;
	swap_buffers();
}

static void swap_buffers() {
	struct buf_p a = back_buffer;
	back_buffer = front_buffer;
	front_buffer = a;
	REAL_SCREEN_BASE_ADDRESS = front_buffer.phys;
	if(wr_overflow) {
		memcpy(back_buffer.virt, front_buffer.virt, 320*240);
		wr_overflow = 0;
	} else {
		int i;
		for(i = 0; i < write_idx; i++) {
			struct pixel_write w = write_buffer[i];
			n_set_84_pixel(w.x, w.y, w.v, back_buffer.virt);
		}
	}
	write_idx = 0;
}
#endif


void set_contrast(uint8_t contrast){
	//printf("set_contrast %d\n", set_contrast);
	int black = contrast * 2;
	int white = 0xff - contrast * 2;
	palette[0] = (pack_gry(black) << 16) | pack_gry(white);
	//printf("%d %d %d %08x\n", contrast, black, white, palette[0]);
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
			palette[3] = palette[2];
			//set_contrast(ls.contrast);
			//puts("ls.enabled");
		break;
		case LCD_DISABLE:
			ls.enabled = FALSE;
			//palette[3] = pack
			palette[3] = pack_rgbp(0x7f0000) * 0x10001;
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

#ifdef LCD_DOUBLE_BUFFER
static void irq_enable(){
	unsigned dummy;
	__asm__ volatile(
		" mrs r0, cpsr\n"
		" bic r0, r0, #0x80\n"
		" msr cpsr_c, r0\n" : "+r"(dummy)
	);
}

static void irq_disable(){
	unsigned dummy;
	__asm__ volatile(
		" mrs %0, cpsr\n"
		" orr %0, %0, #0x80\n"
		" msr cpsr_c, %0\n" : "+r"(dummy)
	);
}
#endif

void lcd_data(uint8_t data){
	int i;
	int x = ls.cur_col * ls.n_bits;
	int y = ls.cur_row;
#ifdef LCD_DOUBLE_BUFFER
	irq_disable();
#endif
	for(i = 0; i < ls.n_bits; i++){
		set_pixel(x + i, y & 0x3f, data & (1<<(ls.n_bits-1-i)), back_buffer.virt);
	}
#ifdef LCD_DOUBLE_BUFFER
	irq_enable();
#endif
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
	int y;
	int j = 0;
	uint8_t *b = malloc(768);
	for(y = 0; y < 64; y++) {
		int x;
		for(x = 0; x < 96;) {
			int i;
			uint8_t v = 0;
			for(i = 0; i < 8; i++, x++) {
				v <<= 1;
				v |= get_pixel(x, y);
			}
			b[j++] = v;
		}
	}
	fwrite(b, 768, 1, f);
	free(b);
}

void lcd_restore(FILE *f){
	FREAD_VALUE(&ls, f);
	uint8_t *b = malloc(768);
	fread(b, 768, 1, f);
	uint8_t *p = b;
	int y;
	for(y = 0; y < 64; y++){
		int x;
		for(x = 0; x < 96;){
			int i;
			uint8_t d = *p++;
			for(i = 0; i < 8; i++, x++) {
				set_pixel(x, y, d >> 7, back_buffer.virt);
				d <<= 1;
			}
		}
	}

	free(b);
	
	set_contrast(ls.contrast);
}

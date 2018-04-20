#include <os.h>
#include "cselcd_imp.h"
#include "cselcd.h"

uint8_t *cse_framebuffer;
uint8_t *cse_bfb;

typedef uint8_t * byteptr;
uint8_t cselcd_auto_horiz = 1;
uint8_t cselcd_vert_inc = 1;
uint8_t cselcd_horiz_inc = 1;
uint8_t cselcd_bgr = 1;
uint8_t cselcd_tri = 0;

uint16_t cselcd_pos_x = 0;
uint16_t cselcd_pos_y = 0;

uint16_t cselcd_s_pos_x = 0;
uint16_t cselcd_s_pos_y = 0;

uint16_t cselcd_win_ymin = 0x0000;
uint16_t cselcd_win_ymax = 0x00EF;
uint16_t cselcd_win_xmin = 0x0000;
uint16_t cselcd_win_xmax = 0x013F;

#define LCD_AUTO_HORIZ (1<<3)
#define LCD_VERT_INC (1<<4)
#define LCD_HORIZ_INC (1<<5)
#define LCD_WIN_ORG (1<<7)
#define LCD_BGR (1<<12)
#define LCD_DFM (1<<14)
#define LCD_TRI (1<<15)

void cselcd_i_data_out(uint16_t val);
uint16_t cselcd_i_data_in();
void cselcd_entry_out(uint16_t val);
void cselcd_wraparound_x();
void cselcd_wraparound_y();
void cselcd_auto_move();

void cselcd_set_pos_x(uint16_t v);
void cselcd_set_pos_y(uint16_t v);

void cselcd_i_init() {
	cse_framebuffer = malloc(320 * 240 * 2);
	memset(cse_framebuffer, 0x00, 320*240*2);
	
	*IO_LCD_CONTROL &= (unsigned)~0b100001110;
	*IO_LCD_CONTROL |= (unsigned)0b100001100; // 16 bpp 5:6:5, BGR order
	
	cse_bfb = *(volatile byteptr *)0xC0000010;
	*(volatile byteptr *)0xC0000010 = cse_framebuffer;
	
	cselcd_ports[0x00].value = 0x9335;
	cselcd_ports[0x02].value = 0x0200; // LCD Driving Control
	cselcd_ports[0x03].value = 0x1038; // Entry Mode
	cselcd_ports[0x03].out = cselcd_entry_out;
	cselcd_ports[0x07].value = 0x0133; // Display Control 1
	cselcd_ports[0x08].value = 0x0202; // Display Control 2
	cselcd_ports[0x09].value = 0x0000; // Display Control 3
	cselcd_ports[0x0A].value = 0x0000; // Display Control 4
	cselcd_ports[0x0C].value = 0x0000; // RGB Display Interface Control 1
	cselcd_ports[0x0D].value = 0x0000; // Frame Maker Position
	cselcd_ports[0x0F].value = 0x0000; // RGB Display Interface Control 2
	cselcd_ports[0x10].value = 0x1190; // Power Control 1
	cselcd_ports[0x11].value = 0x0227; // Power Control 2
	cselcd_ports[0x12].value = 0x008C; // Power Control 3
	cselcd_ports[0x13].value = 0x1800; // Power Control 4
	cselcd_ports[0x29].value = 0x0030; // Power Control 7
	cselcd_ports[0x2B].value = 0x0000; // Frame Rate and Color Control
	cselcd_ports[0x30].value = 0x0000; // Gamma Control 1
	cselcd_ports[0x31].value = 0x0305; // Gamma Control 2
	cselcd_ports[0x32].value = 0x0002; // Gamma Control 3
	cselcd_ports[0x35].value = 0x0301; // Gamma Control 4
	cselcd_ports[0x36].value = 0x0004; // Gamma Control 5
	cselcd_ports[0x37].value = 0x0507; // Gamma Control 6
	cselcd_ports[0x38].value = 0x0204; // Gamma Control 7
	cselcd_ports[0x39].value = 0x0707; // Gamma Control 8
	cselcd_ports[0x3C].value = 0x0103; // Gamma Control 9
	cselcd_ports[0x3D].value = 0x0004; // Gamma Control 10
	
	cselcd_ports[0x20].ptr_value = &cselcd_s_pos_y;
	cselcd_ports[0x21].ptr_value = &cselcd_s_pos_x;
	cselcd_ports[0x20].out = &cselcd_set_pos_y;
	cselcd_ports[0x21].out = &cselcd_set_pos_x;
	
	cselcd_ports[0x50].ptr_value = &cselcd_win_ymin;
	cselcd_ports[0x51].ptr_value = &cselcd_win_ymax;
	cselcd_ports[0x52].ptr_value = &cselcd_win_xmin;
	cselcd_ports[0x53].ptr_value = &cselcd_win_xmax;
	
	cselcd_ports[0x22].out = cselcd_i_data_out;
	cselcd_ports[0x22].in = cselcd_i_data_in;
	
}

void cselcd_set_pos_x(uint16_t v) {
	cselcd_s_pos_x = v;
	cselcd_pos_x = cselcd_s_pos_x;
	cselcd_pos_y = cselcd_s_pos_y;
}

void cselcd_set_pos_y(uint16_t v) {
	cselcd_s_pos_y = v;
	cselcd_pos_x = cselcd_s_pos_x;
	cselcd_pos_y = cselcd_s_pos_y;
}

void cselcd_i_end(){
	*(volatile byteptr *)0xC0000010 = cse_bfb;
	*IO_LCD_CONTROL &= (unsigned)~0b1110;
	*IO_LCD_CONTROL |= (unsigned)0b1100;
	free(cse_framebuffer);
}

uint16_t swaprb(uint16_t val){
	return (val & 0b0000011111100000) | (uint16_t)(val >> 11) | (uint16_t)(val << 11);
}

void cselcd_i_data_out(uint16_t val){
	cse_framebuffer[cselcd_pos_y*320+cselcd_pos_x] = cselcd_bgr ? val : swaprb(val);
	cselcd_auto_move();
}

uint16_t cselcd_i_data_in(){
	uint16_t v = cse_framebuffer[cselcd_pos_y*320+cselcd_pos_x];
	return cselcd_bgr ? v : swaprb(v);
}

void cselcd_wraparound_y(){
	if(cselcd_vert_inc){
		if(cselcd_pos_y++ > cselcd_win_ymax){
			cselcd_pos_y = cselcd_win_ymin;
		}
	}else{
		if(cselcd_pos_y-- < cselcd_win_ymin){
			cselcd_pos_y = cselcd_win_ymax;
		}
	}
}

void cselcd_wraparound_x(){
	if(cselcd_horiz_inc){
		if(cselcd_pos_x++ > cselcd_win_xmax){
			cselcd_pos_x = cselcd_win_xmin;
		}
	}else{
		if(cselcd_pos_x-- < cselcd_win_xmin){
			cselcd_pos_x = cselcd_win_xmax;
		}
	}
}

void cselcd_auto_move(){
	if(cselcd_auto_horiz){
		if(cselcd_horiz_inc){
			if(cselcd_pos_x++ > cselcd_win_xmax){
				cselcd_pos_x = cselcd_win_xmin;
				cselcd_wraparound_y();
			}
		}else{
			if(cselcd_pos_x-- < cselcd_win_xmin){
				cselcd_pos_x = cselcd_win_xmax;
				cselcd_wraparound_y();
			}
		}
	}else{
		if(cselcd_vert_inc){
			if(cselcd_pos_y++ > cselcd_win_ymax){
				cselcd_pos_y = cselcd_win_ymin;
				cselcd_wraparound_x();
			}
		}else{
			if(cselcd_pos_y-- < cselcd_win_ymin){
				cselcd_pos_y = cselcd_win_ymax;
				cselcd_wraparound_x();
			}
		}
	}
}

void cselcd_entry_out(uint16_t val){
	cselcd_auto_horiz = (val & LCD_AUTO_HORIZ) && 1;
	cselcd_vert_inc = (val & LCD_VERT_INC) && 1;
	cselcd_horiz_inc = (val & LCD_HORIZ_INC) && 1;
	cselcd_bgr = (val & LCD_BGR) && 1;
	cselcd_tri = (val & LCD_TRI) && 1;
}
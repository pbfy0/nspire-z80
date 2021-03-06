#include <os.h>
#include "calctype.h"
#define STATUS_BATTERIES 1<<0
#define STATUS_LCD_WAIT 1<<1
#define STATUS_FLASH_UNLOCK 1<<2
#define STATUS_HAS_USB 1<<5
#define STATUS_LINK_ASSIST 1<<6
#define STATUS_NOT_83 1<<7
#define STATUS_NORMAL (STATUS_BATTERIES | STATUS_LCD_WAIT | STATUS_HAS_USB |/* STATUS_LINK_ASSIST | */STATUS_NOT_83)

#include "io.h"

#include "lcd.h"
#include "cselcd.h"
#include "keypad.h"
#include "mmu_mmap.h"
#include "io_misc.h"
#include "z_interrupt.h"
#include "timer.h"
#include "io.h"
#include "rtc.h"

#include "util.h"

struct io_state {
	uint8_t mem_size;
	uint8_t flash_unlocked;
	uint8_t usb_dev_addr;
	uint8_t usb_event_mask;
};

/*#if CALC_TYPE == CALC_84PCSE
#define I_FLASH_SIZE 2
#else
#define I_FLASH_SIZE 1
#endif*/

struct io_state is = {0};

struct z80port ports[0x100];

void memsize_set(uint8_t val);
void port4_out(uint8_t val);
void usb_set_addr(uint8_t val);
uint8_t status_in();


void io_init(){
	int i;
	for(i = 0; i < 0x100; i++){
		ports[i] = (struct z80port){.number = i};
		//ports[i].number = i;
	}
	is.mem_size = g_calc.i_flash_size | 2<<4;
	
	ports[0x00].name = "link port";
	ports[0x00].const_val = 0x03;

	ports[0x01].name = "keypad";
	ports[0x01].in.r = keypad_read;
	ports[0x01].out.r = keypad_write;
	
	ports[0x02].name = "status/interrupt ack";
	ports[0x02].in.r = status_in;
	ports[0x02].out.r = int_ack_out;
	
	ports[0x03].name = "interrupt mask";
	ports[0x03].out.r = int_mask_out;
	ports[0x03].in.r = int_mask_in;
	
	ports[0x04].name = "memory map/interrupt";
	ports[0x04].in.r = int_id_in;
	ports[0x04].out.r = port4_out;
	
	ports[0x05].name = "ram page";
	ports[0x06].name = "memory page A";
	ports[0x07].name = "memory page B";
	ports[0x05].out.r = mmu_port5_out;
	ports[0x05].in.n = mmap_in;
	ports[0x06].out.n = mmu_port67_out;
	ports[0x06].in.n = mmap_in;
	ports[0x07].mirror = &ports[0x06];
	
	ports[0x0E].name = "memory page A high bits";
	ports[0x0F].name = "memory page B high bits";
	ports[0x0E].out.n = mmu_portEF_out;
	ports[0x0E].in.n = mmu_portEF_in;
	ports[0x0F].mirror = &ports[0x0E];
	
	ports[0x10].name = "lcd command";
#ifdef NO_LCD
	ports[0x10].const_val = 0;
#else
	if (!g_calc.cselcd) {
		ports[0x10].in.r = lcd_cmd_read;
		ports[0x10].out.r = lcd_cmd;
	} else {
		ports[0x10].out.r = cselcd_ctrl_out;
	}
#endif
	ports[0x12].mirror = &ports[0x10];
	
	
	ports[0x11].name = "lcd data";
#ifdef NO_LCD
	ports[0x11].const_val = 0;
#else
	if (!g_calc.cselcd) {
		ports[0x11].in.r = lcd_data_read;
		ports[0x11].out.r = lcd_data;
	} else {
		ports[0x11].in.r = cselcd_data_in;
		ports[0x11].out.r = cselcd_data_out;
	}
#endif
	ports[0x13].mirror = &ports[0x11];
	
	ports[0x14].name = "flash unlock";
	ports[0x14].ptr_val = &is.flash_unlocked; // really should be & 1
	ports[0x14].const_val = 0;
	
	ports[0x15].name = "asic version";
	ports[0x15].const_val = 0x45;
	
	ports[0x20].name = "cpu frequency";
	ports[0x20].in.r = cpu_freq_get;
	ports[0x20].out.r = cpu_freq_set;
	
	ports[0x21].name = "memory size";
	ports[0x21].out.r = memsize_set;
	ports[0x21].ptr_val = &is.mem_size;
	
	ports[0x41].name = "rtc set";
	ports[0x41].out.n = rtc_out;
	ports[0x41].in.n = rtc_out_in;
	ports[0x42].mirror = 
	ports[0x43].mirror = 
	ports[0x44].mirror = &ports[0x41];

	ports[0x45].name = "rtc get";
	ports[0x45].in.n = rtc_in;
	ports[0x46].mirror = 
	ports[0x47].mirror = 
	ports[0x48].mirror = &ports[0x45];

	ports[0x4c].name = "usb status";
	ports[0x4c].const_val = 0x22; // usb stuff
	
	ports[0x4d].name = "usb line status";
	ports[0x4d].const_val = 0b10100101; // usb stuff
	ports[0x4d].out.r = NULL; // explicitly does nothing
	
	ports[0x55].name = "usb interrupts";
	ports[0x55].const_val = 0x1f; // usb interrupts
	
	ports[0x56].name = "usb line events";
	ports[0x56].const_val = 0; // individual usb interrupts
	
	ports[0x57].name = "usb event mask";
	ports[0x57].ptr_val = &is.usb_event_mask;
	
	ports[0x80].name = "usb address";
	ports[0x80].out.r = usb_set_addr;
	ports[0x80].ptr_val = &is.usb_dev_addr;
}

void memsize_set(uint8_t val){
	is.mem_size = val & 0b00110011;
}

void port4_out(uint8_t val){
	mmap_set_mode(val & 0x01);
	timer_freq_set((val >> 1) & 0b11);
}

void usb_set_addr(uint8_t val){
	is.usb_dev_addr = val & 0x7f;
}

uint8_t status_in(){
	return STATUS_NORMAL | is.flash_unlocked<<2;
}

void io_save(FILE *f){
	FWRITE_VALUE(is, f);
}

void io_restore(FILE *f){
	FREAD_VALUE(&is, f);
}

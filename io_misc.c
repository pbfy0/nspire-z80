#include <os.h>
#define STATUS_BATTERIES 1<<0
#define STATUS_LCD_WAIT 1<<1
#define STATUS_FLASH_UNLOCK 1<<2
#define STATUS_HAS_USB 1<<5
#define STATUS_LINK_ASSIST 1<<6
#define STATUS_NOT_83 1<<7
#define STATUS_NORMAL (STATUS_BATTERIES | STATUS_LCD_WAIT | STATUS_HAS_USB |/* STATUS_LINK_ASSIST | */STATUS_NOT_83)

#include "io.h"

#include "lcd.h"
#include "keypad.h"
#include "mmap.h"
#include "io_misc.h"
#include "z_interrupt.h"
#include "timer.h"
#include "io.h"


uint8_t mem_size = 3 | 2<<4;
uint8_t flash_unlocked = 0;
uint8_t usb_dev_addr = 0;
uint8_t usb_event_mask = 0;

struct z80port ports[0x100] = {0};

void memsize_set(uint8_t val);
void port4_out(uint8_t val);
void usb_set_addr(uint8_t val);


void io_init(){
	int i;
	for(i = 0; i < 0x100; i++){
		ports[i].number = i;
	}
	
	ports[0x00].name = "link port";
	ports[0x00].const_val = 0x03;

	ports[0x01].name = "keypad";
	ports[0x01].in = keypad_read;
	ports[0x01].out = keypad_write;
	
	ports[0x02].name = "status/interrupt ack";
	ports[0x02].const_val = STATUS_NORMAL | flash_unlocked<<2;
	ports[0x02].out = int_ack_out;
	
	ports[0x03].name = "interrupt mask";
	ports[0x03].out = int_mask_out;
	ports[0x03].in = int_mask_in;
	
	ports[0x04].name = "memory map/interrupt";
	ports[0x04].in = int_id_in;
	ports[0x04].out = port4_out;
	
	ports[0x05].name = "ram page";
	ports[0x06].name = "memory page A";
	ports[0x07].name = "memory page B";
	ports[0x05].n_out = 
	ports[0x06].n_out = 
	ports[0x07].n_out = mmap_out;
	ports[0x05].n_in = 
	ports[0x06].n_in = 
	ports[0x07].n_in = mmap_in;
	
	ports[0x10].name =
	ports[0x12].name = "lcd command";
	ports[0x10].in = 
	ports[0x12].in = lcd_cmd_read;
	ports[0x10].out = 
	ports[0x12].out = lcd_cmd;
	
	
	ports[0x11].name = 
	ports[0x13].name = "lcd data";
	ports[0x11].in = 
	ports[0x13].in = lcd_data_read;
	ports[0x11].out = 
	ports[0x13].out = lcd_data;
	
	ports[0x14].name = "flash unlock";
	ports[0x14].ptr_val = &flash_unlocked; // really should be & 1
	ports[0x14].const_val = 0;
	
	ports[0x15].name = "asic version";
	ports[0x15].const_val = 0x45;
	
	ports[0x20].name = "cpu frequency";
	ports[0x20].in = cpu_freq_get;
	ports[0x20].out = cpu_freq_set;
	
	ports[0x21].name = "memory size";
	ports[0x21].out = memsize_set;
	ports[0x21].ptr_val = &mem_size;
	
	ports[0x4c].name = "usb status";
	ports[0x4c].const_val = 0x22; // usb stuff
	
	ports[0x4d].name = "usb line status";
	ports[0x4d].const_val = 0b10100101; // usb stuff
	ports[0x4d].out = NULL; // explicitly does nothing
	
	ports[0x55].name = "usb interrupts";
	ports[0x55].const_val = 0x1f; // usb interrupts
	
	ports[0x56].name = "usb line events";
	ports[0x56].const_val = 0; // individual usb interrupts
	
	ports[0x57].name = "usb event mask";
	ports[0x57].ptr_val = &usb_event_mask;
	
	ports[0x80].name = "usb address";
	ports[0x80].out = usb_set_addr;
	ports[0x80].ptr_val = &usb_dev_addr;
}

void memsize_set(uint8_t val){
	mem_size = val & 0b00110011;
}

void port4_out(uint8_t val){
	mmap_set_mode(val & 0x01);
	timer_freq_set((val >> 1) & 0b11);
}

void usb_set_addr(uint8_t val){
	usb_dev_addr = val & 0x7f;
}

uint8_t default_in(uint8_t port){
	switch(port){
		case 0x14: // Flash write-lock
		return 0;
		case 0x21:
		return mem_size;
		case 0x4c: // something usb
		return 0x22;
		case 0x4d: // something else usb
		return 0;
		case 0x55: // usb interrupts
		return 0x1f;
		case 0x56: // individual usb interrupts
		return 0;
		case 0x57:
		return usb_event_mask;
		case 0x80:
		return usb_dev_addr;
	}
	printf("in %x\n", port);
	return 0;
}
void default_out(uint8_t port, uint8_t val){
	switch(port){
		case 0x4d:
		return;
		case 0x57:
		usb_event_mask = val;
		return;
		case 0x80:
		usb_dev_addr = val & 0x7f;
		return;
	}
	printf("out %x %x\n", port, val);
}
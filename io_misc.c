#include <os.h>
#define STATUS_BATTERIES 1<<0
#define STATUS_LCD_WAIT 1<<1
#define STATUS_FLASH_UNLOCK 1<<2
#define STATUS_HAS_USB 1<<5
#define STATUS_LINK_ASSIST 1<<6
#define STATUS_NOT_83 1<<7
#define STATUS_NORMAL (STATUS_BATTERIES | STATUS_LCD_WAIT | STATUS_HAS_USB |/* STATUS_LINK_ASSIST | */STATUS_NOT_83)

uint8_t mem_size = 3 | 2<<4;
uint8_t flash_unlocked = 0;
uint8_t usb_dev_addr = 0;
uint8_t usb_event_mask = 0;

uint8_t default_in(uint8_t port){
	switch(port){
		case 0x02:
		return STATUS_NORMAL | flash_unlocked<<2;
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
		case 0x14:
		flash_unlocked = val & 1;
		return;
		case 0x21:
		mem_size = val;
		return;
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
#include "z_interrupt.h"
#include "timer.h"
#include <os.h>
static const t_key KEY_NSPIRE_ON        = KEY_(0x10, 0x200);

extern struct DrZ80 ZCpu;
extern volatile uint8_t flag;

uint8_t ints_enabled = 0;
uint8_t ints_firing = 0;

void int_mask_out(uint8_t val){
	//printf("int_mask_out %02x (f %02x)\n", val, ints_firing);
	ints_enabled = val;
	timer_set_enabled((val >> 1) & 0b11);
	ints_firing &= val;
	/*if(val & INT_ON) printf("on ");
	if(val & INT_ON) printf("hwt1 ");
	if(val & INT_ON) printf("hwt2 ");
	if(val & INT_ON) printf("link ");
	if(val & INT_ON) printf("ct1 ");
	if(val & INT_ON) printf("ct2 ");
	if(val & INT_ON) printf("ct3 ");
	printf("\n");*/
}

uint8_t int_mask_in(){
	return ints_enabled & ~(1<<3);
}

void int_ack_out(uint8_t val){
	//printf("int_ack_out %02x\n", val);
	ints_firing &= val;
}

uint8_t int_id_in(){
	//printf("int_id_in %02x\n", ints_firing);
	return ints_firing | (isKeyPressed(KEY_NSPIRE_ON) ? 0 : 1<<3);
}

void int_fire(uint8_t num){
	ints_firing |= num;
	ZCpu.Z80_IRQ = 1;
	flag = num;
}

void int_callback(){
	ZCpu.Z80_IRQ = 0;
	/*if(ints_firing != 0){
		int_fire(ints_firing);
	}*/
}
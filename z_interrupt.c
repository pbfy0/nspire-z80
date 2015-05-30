#include "z_interrupt.h"

extern struct DrZ80 ZCpu;

uint8_t ints_enabled = 0;
uint8_t ints_firing = 0;

void int_mask_out(uint8_t val){
	ints_enabled = val;
	ints_firing &= val;
}

void int_ack_out(uint8_t val){
	ints_firing &= val;
}

uint8_t int_id_in(){
	return ints_firing;
}

void int_fire(uint8_t num){
	ints_firing |= num;
	ZCpu.Z80_IRQ = 1;
}

void int_callback(){
	if(ints_firing != 0){
		int_fire(ints_firing);
	}
}
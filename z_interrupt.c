#include "z_interrupt.h"
#include "timer.h"
#include <os.h>

extern struct DrZ80 ZCpu;
struct z_interrupt_state {
	uint8_t ints_enabled;
	uint8_t ints_firing;
};

struct z_interrupt_state zis = {0, 0};

void int_mask_out(uint8_t val){
	//printf("int_mask_out %02x (f %02x)\n", val, zis.ints_firing);
	zis.ints_enabled = val;
	timer_set_enabled((val >> 1) & 0b11);
	zis.ints_firing &= val;
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
	return zis.ints_enabled & ~(1<<3);
}

void int_ack_out(uint8_t val){
	//printf("int_ack_out %02x\n", val);
	zis.ints_firing &= val;
}

uint8_t int_id_in(){
	//printf("int_id_in %02x\n", zis.ints_firing);
	return zis.ints_firing | (isKeyPressed(KEY_NSPIRE_HOME) ? 0 : 1<<3);
}

void int_fire(uint8_t num){
	zis.ints_firing |= num;
	ZCpu.Z80_IRQ = 1;
}

void int_callback(){
	ZCpu.Z80_IRQ = 0;
	/*if(zis.ints_firing != 0){
		int_fire(zis.ints_firing);
	}*/
}

void int_save(FILE *f){
	FWRITE_VALUE(zis, f);
}

void int_restore(FILE *f){
	FREAD_VALUE(&zis, f);
}

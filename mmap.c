#include <os.h>
#include "drz80.h"
extern struct DrZ80 ZCpu;

#define PAGE(base, n) (base+(n)*0x4000)
#define FLASH_PAGE(n) PAGE(flash, n)
#define RAM_PAGE(n) PAGE(ram, n)
#define BANK(addr) banks[addr >> 14]

typedef struct {
	int active_page;
	uint8_t *addr;
	uint8_t is_ram;
} membank;

void mmap_out(uint8_t port, uint8_t val);
void switch_bank(int b, uint8_t v);
uint8_t *flash;
uint8_t *ram;

membank banks[4];
void mmap_init(){
	ram = calloc(0x20000, 1);
	flash = calloc(0x200000, 1);
	memset(banks, 0, sizeof(banks));
	banks[0].addr = FLASH_PAGE(0);
	banks[0].active_page = 0;
	banks[0].is_ram = 0;
	banks[3].is_ram = 1;
	mmap_out(5, 0);
	mmap_out(6, 0);
	mmap_out(7, 0);
	//mmap_out(5, 3);
	/*mmap_out(6, 1);
	mmap_out(7, 2);
	banks[3].addr = FLASH_PAGE(4);
	banks[3].active_page = 4;*/
}

void mmap_end(){
	free(ram);
	free(flash);
}

uint8_t *mmap_z80_to_arm(uint16_t z80addr){
	return BANK(z80addr).addr + (z80addr & 0x3FFF);
}
uint8_t *mmap_bank_for_addr(uint16_t z80addr){
	//printf("bank %d (%s %02x) addr %04x\n", z80addr >> 14, b->is_ram ? "ram" : "flash", b->active_page, z80addr & 0x3FFF);
	return BANK(z80addr).addr;
}
uint8_t *mmap_base_addr(uint16_t z80addr){
	return mmap_bank_for_addr(z80addr) - 0x4000 * (z80addr >> 14);
}

int bfp(int p){
	switch(p){
		case 5: return 3;
		case 6: return 1;
		case 7: return 2;
	}
	return 0;
}

void mmap_out(uint8_t port, uint8_t val){
	switch(port){
		case 5:
		banks[3].addr = RAM_PAGE(val);
		banks[3].active_page = val;
		break;
		case 6:
		switch_bank(1, val);
		break;
		case 7:
		switch_bank(2, val);
		break;
	}
}

uint8_t mmap_in(uint8_t port){
	return banks[bfp(port)].active_page;
}

void switch_bank(int b, uint8_t v){
	if(v & 0x80){
		banks[b].addr = RAM_PAGE(v & 0b111);
		banks[b].active_page = v;
		banks[b].is_ram = 1;
	}else{
		banks[b].addr = FLASH_PAGE(v & 0x7f);
		banks[b].active_page = v;
		banks[b].is_ram = 0;
	}
}
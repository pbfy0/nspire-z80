#include <os.h>
#include "mmap.h"

#define PAGE(base, n) ((base)+(n)*0x4000)
#define FLASH_PAGE(n) PAGE(flash, n)
#define RAM_PAGE(n) PAGE(ram, n)
#define BANK(addr) active_map[addr >> 14]

static void update_bank(membank *b, uint8_t v);

uint8_t *flash;
uint8_t *ram;

membank banks[4];
membank alt_banks[4];
membank *active_map;
uint8_t normal = 0;

void mmap_init(){
	ram = calloc(0x20000, 1);
	flash = calloc(0x180000, 1);
	memset(banks, 0, sizeof(banks));
	banks[0].addr = FLASH_PAGE(0x7f);
	banks[0].active_page = 0x7f;
	banks[0].is_ram = 0;
	banks[3].is_ram = 1;
	memcpy(alt_banks, banks, sizeof(banks));
	active_map = banks;
	mmap_out(5, 0);
	mmap_out(6, 0);
	mmap_out(7, 0);
}
void mmap_check_endboot(uint16_t pc){
	if(normal) return;
	if(pc >= 0x4000 && pc < (active_map == banks ? 0x8000: 0xC000)){
		normal = 1;
		mmap_normal_init();
	}
}
void mmap_normal_init(){
	//printf("mmap_normal_init\n");
	banks[0].addr = FLASH_PAGE(0);
	banks[0].active_page = 0;
	memcpy(alt_banks, banks, sizeof(banks[0]));
}
/*void bootmode_init(){
	banks[0].addr = FLASH_PAGE(0x7f);
	banks[1].addr = FLASH_PAGE(0x00);
	banks[2].addr = FLASH_PAGE(0x00);
	banks[3].addr = RAM_PAGE(0x00);
	banks[3].is_ram = 1;
}*/
void mmap_end(){
	free(ram);
	free(flash);
}

uint8_t *mmap_z80_to_arm(uint16_t z80addr){
	return BANK(z80addr).addr + (z80addr & 0x3FFF);
}
uint8_t *mmap_bank_for_addr(uint16_t z80addr){
	//membank *b = &(BANK(z80addr));
	//printf("bank %d (%s %02x) addr %04x\n", z80addr >> 14, b->is_ram ? "ram" : "flash", b->active_page, z80addr & 0x3FFF);
	return BANK(z80addr).addr;
}
uint8_t *mmap_base_addr(uint16_t z80addr){
	return BANK(z80addr).addr - (z80addr & 0xc000);//0x4000 * (z80addr >> 14);
	// The high two bits of the address are subtracted off to allow z80 addresses to remain consistent
}

int bfp(int p){
	switch(p){
		case 5: return 3;
		case 6: return 1;
		case 7: return 2;
	}
	return 0;
}

void mmap_set_mode(uint8_t mode){
	active_map = mode ? alt_banks : banks;
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
	int i;
	/*for(i = 0; i < 4; i++){
		printf("bank %d addr %02x\n", i, banks[i].active_page);
	}*/
}

uint8_t mmap_in(uint8_t port){
	return banks[bfp(port)].active_page;
}

void switch_bank(int b, uint8_t v){
	update_bank(&banks[b], v);
	if(b == 1){
		update_bank(&alt_banks[1], v & 0xfe);
		update_bank(&alt_banks[2], v | 0x01);
		return;
	}
	if(b == 2){
		memcpy(&alt_banks[3], &banks[b], sizeof(membank));
		//update_bank(&alt_banks[3], v);
	}
}

static void update_bank(membank *b, uint8_t v){
	b->active_page = v;
	if(v & 0x80){
		b->addr = RAM_PAGE(v & 0b111);
		b->is_ram = 1;
	}else{
		b->addr = FLASH_PAGE(v & 0x7f);
		b->is_ram = 0;
	}
}
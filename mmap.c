#include <os.h>
#include "mmap.h"

#define PAGE(base, n) ((base)+(n)*0x4000)
#define FLASH_PAGE(n) PAGE(flash, n)
#define RAM_PAGE(n) PAGE(ram, n)
#define BANK(addr) active_map[addr >> 14]

#ifdef USE_CSE
#define BOOT_PAGE 0xff
#define HI_MASK 0x01
#else
#define BOOT_PAGE 0x7f
#define HI_MASK 0x00
#endif

static void update_bank(membank *b);
void mmap_set_flashpage(membank *b, uint16_t val);
void mmap_set_rampage(membank *b, uint16_t val);

uint8_t *flash;
uint8_t *ram;

membank banks[4];
membank alt_banks[4];
membank *active_map;
uint8_t normal = 0;

void mmap_init(){
	ram = calloc(0x20000, 1);
	memset(banks, 0, sizeof(banks));
	flash = calloc(FLASH_SIZE, 1);
	mmap_set_flashpage(&banks[0], BOOT_PAGE);
	switch_bank(0);
	banks[3].is_ram = 1;
	memcpy(alt_banks, banks, sizeof(banks));
	active_map = banks;
	mmap_out(0, 5);
	mmap_out(0, 6);
	mmap_out(0, 7);
}
void mmap_set_flashpage(membank *b, uint16_t val){
	b->lo = val & 0x7f;
	b->hi = val >> 7;
}
void mmap_set_rampage(membank *b, uint16_t val){
	b->lo = val | 0x80;
	b->hi = 0;
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
	banks[0].lo = 0;
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
uint16_t mmap_get_active_page(uint16_t z80addr){
	return BANK(z80addr).lo;
	//	return mbss[(z80addr >> 14) - 1].hi << 8 | mbss[(z80addr >> 14) - 1].low;
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

void mmap_out(uint8_t val, uint8_t port){
	//printf("%02x %02x\n", port, val);
	switch(port){
		case 5:
		banks[3].addr = RAM_PAGE(val);// & 0b111);
		banks[3].lo = val | 0x80;
		break;
		case 6:
		banks[1].lo = val;
		switch_bank(1);
		break;
		case 7:
		banks[2].lo = val;
		switch_bank(2);
		break;
	}
	int i;
	/*for(i = 0; i < 4; i++){
		printf("bank %d addr %02x\n", i, banks[i].active_page);
	}*/
}

uint8_t mmap_in(uint8_t port){
	return banks[bfp(port)].lo;
}

void mmap_hi_out(uint8_t val, uint8_t port){
	switch(port){
		case 0x0E:
		banks[1].hi = val & HI_MASK;
		switch_bank(1);
		break;
		case 0x0F:
		banks[2].hi = val & HI_MASK;
		switch_bank(2);
		break;
	}
}

uint8_t mmap_hi_in(uint8_t port){
	switch(port){
		case 0x0E:
		return banks[1].hi;
		case 0x0F:
		return banks[2].hi;
	}
	return 0;
}

void switch_bank(int n){
	membank *b = &banks[n];
	//printf("%d %02x %02x\n", n, b->lo, b->hi);
	update_bank(b);
	if(n == 0){
		memcpy(&alt_banks[0], b, sizeof(membank));
	}else if(n == 1){
		alt_banks[1].lo = b->lo & 0xfe;
		alt_banks[2].lo = b->lo | 0x01;
		
		update_bank(&alt_banks[1]);
		update_bank(&alt_banks[2]);
		return;
	}else if(n == 2){
		memcpy(&alt_banks[3], b, sizeof(membank));
		//update_bank(&alt_banks[3], v);
	}
}

static void update_bank(membank *b){
	if(b->lo & 0x80){
		b->addr = RAM_PAGE(b->lo & 0b111);
		b->is_ram = 1;
	}else{
		//printf("set to flash page %03x\n", v.low & 0x7f | v.hi << 7);
		b->addr = FLASH_PAGE((b->lo & 0x7f) | (b->hi << 7));
		b->is_ram = 0;
	}
}

void mmap_save(FILE *f){
	uint8_t b[3][2];
	
	int i;
	for(i = 1; i < 4; i++){
		b[i-1][0] = banks[i].lo;
		b[i-1][1] = banks[i].hi;
	}
	fwrite(b, sizeof(uint8_t) * 2, 3, f);
	fputc(normal, f);
}

void mmap_restore(FILE *f){
	uint8_t b[3][2];
	fread(b, sizeof(uint8_t) * 2, 3, f);
	
	int i;
	for(i = 1; i < 4; i++){
		banks[i].lo = b[i-1][0];
		banks[i].hi = b[i-1][1];
		switch_bank(i);
	}
	normal = fgetc(f);
	if(normal) mmap_normal_init();
}
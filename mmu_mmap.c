#include "mmu_mmap.h"

#include <os.h>
#include <syscall-list.h>
#include "c_syscall.h"

#ifdef USE_CSE
#define FLASH_SIZE 0x400000
#else
#define FLASH_SIZE 0x200000
#endif
#define RAM_SIZE 0x20000

__attribute__((naked)) void *get_mmu_addr() {
	asm(
	"mrc p15, 0, r0, c2, c0, 0\n\t"
	"bx lr\n\t"
	);
}

__attribute__((naked)) void set_mmu_addr(void *a) {
	asm(
	"mcr p15, 0, r0, c2, c0, 0\n\t"
	"bx lr\n\t"
	);
}

__attribute__((naked)) uint32_t get_mmu_status() {
	asm(
	"mrc p15, 0, r0, c1, c0, 0\n\t"
	"bx lr\n\t"
	);
}

/*__attribute__((naked)) void *_malloc(size_t size) {
	asm(
	"swi #5\n\t"
	"bx lr\n\t"
	);
}
__attribute__((naked)) void _free(void *ptr) {
	asm(
	"swi #6\n\t"
	"bx lr\n\t"
	);
}*/
void *_malloc(size_t size) {
	return wa_syscall1(e_malloc, size);
}
void _free(void *ptr) {
	wa_syscall1(e_free, ptr);
}
void *aligned_alloc(size_t alignment, size_t size) { // alignment must be a power of two and this is a little broken
	void *b = _malloc(size + alignment);
	void *c = ((intptr_t)b | (alignment - 1)) + 1;
	uint32_t *m = (uint32_t *)c;
	m[-1] = (c - b);
	return c;
}
void aligned_free(void *ptr) {
	uint32_t *m = (uint32_t *)ptr;
	void *rm = ptr - m[-1];
	_free(rm);
}

/*struct fl_descriptor {
	struct {
		
	} 
}*/

uint8_t *mem_base;
uint32_t *section_base;
uint32_t *section_base_l;
int n_sections;
uint32_t *mmu_base; 
unsigned normal = 0;
static unsigned mmu_mode = 0;

/*int main(void) {
	mmu_init();
	mmu_end();
}*/

#define RAM_PAGE(x) (mem_base + FLASH_SIZE + (0x4000 * (x)))
#define ROM_PAGE(x) (mem_base + (0x4000 * (x)))

static void update67();

void map_in(int idx, void *base) {
	int i;
	intptr_t b2 = (intptr_t)base & ~0xfff;
	uint32_t* sb = section_base + idx * 4;
	uint32_t* sbl = section_base_l + 0xf0 + idx * 4;
	//printf("mapping %04x -> %02x\n", idx * 0x4000, ((uint8_t *)base - mem_base) / 0x4000);
	//printf("mapping %p -> %p = %p (%p, %x)\n", 0xe0000000 + (idx * 0x4000), base, b2, (uint8_t *)base - mem_base, ((uint8_t *)base - mem_base) / 0x4000);
	clear_cache();
	for(i = 0; i < 4; i++) {
		sbl[i] = (b2 + 0x1000 * i) | 0b0010;
		sb[i] = (b2 + 0x1000 * i) | 0b0010;
		sb[i+16] = (b2 + 0x1000 * i) | 0b0010;
	}
	clear_cache();
}

unsigned mmap_check_endboot(uint16_t pc){
	//printf("j %04x\n", pc);
	if(normal) return 1;
	if(pc >= 0x4000 && pc < (mmu_mode ? 0xC000 : 0x8000)){
		printf("end boot\n");
		normal = 1;
		map_in(0, ROM_PAGE(0));
		//clear_cache();
	}
	return normal;
}

/*static void clear_dcache(void) {
	unsigned dummy;
	__asm volatile(
		" .arm \n"
		"0: mrc p15, 0, r15, c7, c10, 3 @ test and clean DCache \n"
		" bne 0b \n"
		" mov %0, #0 \n"
		" mcr p15, 0, %0, c7, c7, 0 @ invalidate ICache and DCache \n"
	: "=r" (dummy));
}*/

uint8_t *flash;
uint8_t *ram;

void mmap_set_mode(uint8_t mode){
	//printf("mmu mode -> %d\n", mode);
	mmu_mode = mode;
	update67();
}
static int bfp(int p){
	switch(p){
		case 5: return 2;
		case 6: return 0;
		case 7: return 1;
	}
	return 0;
}
uint8_t banks[3];

uint8_t mmap_in(uint8_t port){
	return banks[bfp(port)];
}

void mmu_init() {
	mmu_base = get_mmu_addr();
	mem_base = aligned_alloc(0x1000, RAM_SIZE + FLASH_SIZE);
	int tbl_size = 4;
	int t2_size = 0x100;
	section_base = aligned_alloc(0x400, 4 * t2_size);
	section_base_l = aligned_alloc(0x400, 4 * t2_size);
	int i;
	for(i = 0; i < t2_size; i++) {
		section_base[i] = 0;
	}
	for(i = 0; i < t2_size; i++) {
		section_base_l[i] = 0;
	}
	map_in(0, ROM_PAGE(0x7f));
	mmu_port5_out(0);
	mmu_port67_out(0, 6);
	mmu_port67_out(0, 7);
	
	mmu_base[0xdff] = (intptr_t)section_base_l | 0b10001;
	mmu_base[0xe00] = (intptr_t)section_base | 0b10001;
	clear_cache();
	
	memset(mem_base, 0, RAM_SIZE + FLASH_SIZE);
	flash = mem_base;
	ram = mem_base + FLASH_SIZE;
	printf("flash = %p, ram = %p\n", flash, ram);
}


static void update67() {
	if(mmu_mode == 0) {
		map_in(1, ROM_PAGE(banks[0]));
		map_in(2, ROM_PAGE(banks[1]));
		map_in(3, RAM_PAGE(banks[2]));
	} else {
		map_in(1, ROM_PAGE(banks[0] & ~1));
		map_in(2, ROM_PAGE(banks[0] | 1));
		map_in(3, ROM_PAGE(banks[1]));
	}
	//clear_cache();
}

void mmu_port5_out(uint8_t val) {
	//printf("wrote %02x to port 05\n", val);
	banks[2] = val;
	if(mmu_mode == 0) {
		map_in(3, RAM_PAGE(val));
		//clear_cache();
	}
}

void mmu_port67_out(uint8_t val, uint8_t port) {
	//printf("wrote %02x to port %02x\n", val, port);
	banks[port - 6] = val;
	if(mmu_mode == 0) {
		map_in(port - 5, ROM_PAGE(val));
	} else if(port == 6) {
		map_in(1, ROM_PAGE(val & 0xfe));
		map_in(2, ROM_PAGE(val | 1));
	} else if(port == 7) {
		map_in(3, ROM_PAGE(val));
	}
	//clear_cache();
}


#if 0
void mmu_init() {
	printf("a\n");
	mmu_base = get_mmu_addr();
	printf("%p\n", mmu_base);
	mem_base = aligned_alloc(0x1000, RAM_SIZE + FLASH_SIZE);
	//void *bottom = mem_base & ~0xfffff;
	//void *top = (mem_base + RAM_SIZE + FLASH_SIZE - 0x1000) & ~0xfffff;
	
	/*int bottom_idx = bottom >> 20;
	int top_idx = top >> 20;*/
	
	int tbl_size = (RAM_SIZE + FLASH_SIZE) / 0x1000;//top - bottom + 1;
	int t2_size = tbl_size & 0xff == 0 ? tbl_size : (tbl_size | 0xff) + 1;
	section_base = aligned_alloc(0x400, 4 * t2_size); // aligned on 1k
	printf("b\n");
	int i;
	for(i = 0; i < tbl_size; i++) {
		section_base[i] = (intptr_t)(mem_base + i * 0x1000) & ~0xfff | 0b1110; // I think
	}
	for(i = tbl_size; i < t2_size; i++) {
		section_base[i] = 0;
	}
	printf("c\n");
	////clear_cache();
	//asm volatile("dmb st\n\t");
	n_sections = t2_size / 256;
	for(i = 0; i < n_sections; i++) {
		mmu_base[i + 0xe00] = (intptr_t)(section_base + i * 256) | 0b10001;
	}
	//clear_cache();
	printf("d\n");
	//asm volatile("dmb\n\t");
	mem_base[0] = 0x12345678;
	printf("%08x\n", *(uint32_t *)0xe0000000);
}
#endif

void mmu_end() {
	int i;
	mmu_base[0xdff] = 0;
	mmu_base[0xe00] = 0;
	//clear_cache();
	aligned_free(section_base);
	aligned_free(mem_base);
}

void mmu_mmap_init(){
	mem_base = aligned_alloc(0x1000, RAM_SIZE + FLASH_SIZE);
}

void mmap_save() {}
void mmap_restore() {}
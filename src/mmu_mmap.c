#include "mmu_mmap.h"

#include <os.h>
#include <stdio.h>
#include "util.h"
#include <syscall-list.h>
#include "c_syscall.h"
#include "aligned_alloc.h"
#include "lcd.h"

#ifdef USE_CSE
#define FLASH_SIZE 0x400000
#define BOOT_PAGE 0xff
#define EF_MASK 1
#else
#define FLASH_SIZE 0x200000
#define BOOT_PAGE 0x7f
#define EF_MASK 0
#endif

#define RAM_SIZE 0x20000
#define PAGE_SIZE 0x4000
#define FLASH_PAGES (FLASH_SIZE / PAGE_SIZE)

static void *get_mmu_addr() {
	void *v;
	asm volatile(
	"mrc p15, 0, %0, c2, c0, 0\n\t"
	: "=r"(v));
	return v;
}

static void set_mmu_addr(void *a) {
	asm volatile(
	"mcr p15, 0, %0, c2, c0, 0\n\t"
	:: "r"(a));
}

static void invalidate_tlb_all() {
	asm volatile(
	"	mcr p15, 0, %0, c8, c6, 0\n" ::"r"(0)
	);
}

static void clean_dcache_all() {
	asm volatile(
		"1:	mrc p15, 0, r15, c7, c10, 3\n" //test and clean
		"	bne 1b\n"
	::: "cc"); // modifies condition codes
}

static void clean_inval_dcache_all() {
	asm volatile(
		"1:	mrc p15, 0, r15, c7, c14, 3\n" //test and clean
		"	bne 1b\n"
	::: "cc"); // modifies condition codes

}

static void clean_inval_dcache(void *a) {
	asm volatile(
		"	mcr p15, 0, %0, c7, c14, 1\n"
	::"r"(a));
}

static uint32_t get_cache_type() {
	uint32_t v;
	asm volatile(
	"	mrc p15,0,%0,c0,c0,1\n"
	:"=r"(v));
	return v;
}


static void invalidate_tlb(void *a) {
	asm volatile(
	"	mcr p15, 0, %0, c8, c6, 1\n"
	:: "r"(a));
}


static uint32_t get_cr1() {
	uint32_t v;
	asm volatile(
	"mrc p15, 0, %0, c1, c0, 0\n\t"
	: "=r"(v));
	return v;
}

static void set_cr1(uint32_t v) {
	asm volatile(
	"mcr p15, 0, %0, c1, c0, 0\n\t"
	::"r"(v));
}

static uint32_t get_cr3() {
	uint32_t v;
	asm volatile(
	"mrc p15, 0, %0, c3, c0, 0\n\t"
	: "=r"(v));
	return v;
}

static void set_cr3(uint32_t v) {
	asm volatile(
	"mcr p15, 0, %0, c3, c0, 0\n\t"
	::"r"(v));
}

#include "_syscalls.h"

uint8_t *mem_base;
uint32_t *section_base;
uint32_t *section_base_l;

uint32_t *m_section_base = (uint32_t *)0xe0040400;

aligned_ptr mem_base_al;
aligned_ptr section_base_al;
aligned_ptr section_base_l_al;

int n_sections;
uint32_t *mmu_base;
unsigned normal = 0;
static unsigned mmu_mode = 0;
unsigned testing = 0;
unsigned uses_hi_ram = 0;

/*int main(void) {
	mmu_init();
	mmu_end();
}*/

static void *RAM_PAGE(unsigned x) {
	void *b = mem_base + FLASH_SIZE + (0x4000 * x);
#ifdef MMU_DEBUG
	if((uintptr_t)b - (uintptr_t)mem_base >= FLASH_SIZE + RAM_SIZE) printf("illegal start of ram page %02x %p - b=%p!\n", x, b, mem_base);
	if((uintptr_t)b - (uintptr_t)mem_base + 0x4000 > FLASH_SIZE + RAM_SIZE) printf("illegal end of ram page %02x %p - b=%p!\n", x, b, mem_base);
#endif
	return b;
}
static void *ROM_PAGE(unsigned x) {
	void *b = mem_base + (0x4000 * x);
#ifdef MMU_DEBUG
	if((uintptr_t)b - (uintptr_t)mem_base >= FLASH_SIZE + RAM_SIZE) printf("illegal start of rom page %02x %p - b=%p!\n", x, b, mem_base);
	if((uintptr_t)b - (uintptr_t)mem_base + 0x4000 > FLASH_SIZE + RAM_SIZE) printf("illegal end of rom page %02x %p - b=%p!\n", x, b, mem_base);
#endif
	return b;
}
//#define RAM_PAGE(x) (mem_base + FLASH_SIZE + (0x4000 * (x)))
//#define ROM_PAGE(x) (mem_base + (0x4000 * (x)))

static void update67();

static void map_in(int idx, void *base, bool ro) {
	//puts("map_in a");
	int i;
	uintptr_t b2 = (uintptr_t)base & ~0xfff;
	uint32_t* sb = m_section_base + idx * 4;
	//uint32_t* sbl = m_section_base_l + 0xf0 + idx * 4;
	if((sb[0] & ~0xfff) == b2) {
		//puts("map_in b");
		return;
	}
	//printf("mapping %04x -> %02x\n", idx * 0x4000, ((uint8_t *)base - mem_base) / 0x4000);
#ifdef MMU_DEBUG
	if(!testing) printf("mapping %p -> %p = %p (%p, %02x)\n", Z80_MEM_BASE + (idx * 0x4000), base, b2, (uint8_t *)base - mem_base, ((uint8_t *)base - mem_base) / 0x4000);
#endif
	//if(b2 - (uintptr_t)mem_base > FLASH_SIZE + RAM_SIZE) printf("illegal start of page %04x %p - b=%p!\n", idx * 0x4000, base, mem_base);
	//if(b2 - (uintptr_t)mem_base + 0x4000 > FLASH_SIZE + RAM_SIZE) printf("illegal end of page %04x %p - b=%p!\n", idx * 0x4000, base, mem_base);
	uint32_t *mb = (uint32_t *)(Z80_MEM_BASE + idx * 0x4000);
	
	// flush cached entries from old mapping to RAM
#ifndef EMU_BUILD
	int j;
	for(j = 0; j < 0x4000; j += 8) {
		clean_inval_dcache(mb + j);
	}
#else
	clean_inval_dcache_all();
#endif
	
	for(i = 0; i < 4; i++) {
		uint32_t v = (b2 + 0x1000 * i) | 0b1110 | (ro ? 0 : 0xff0);
		sb[i-16] = sb[i] = sb[i+16] = v;
	}
	// translation table is write-through, so cache clear isn't necessary
	//clean_inval_dcache_all();

	// clear TLB for newly-mapped page
	for(i = 0; i < 16; i++) {
		invalidate_tlb(mb + i * 0x100);
	}
}

void map_framebuffer(void *buf) {
	int i, j;
	uintptr_t bb = (uintptr_t)buf;
	for(i = 0, j=0; i < FB_SIZE; i += 0x1000, j++) {
		m_section_base[0x50+j] = (bb + i) | 0b1010 | 0xff0;
	}
	for(i = 0; i < FB_SIZE; i += 0x400) {
		invalidate_tlb((uint8_t *)0xe0050000 + i);
	}
}

struct bank {
	uint8_t low;
	uint8_t hi;
};


static unsigned off_for_bank(struct bank b) {
	return b.low & 0x80 ? (unsigned)b.low - 0x80 + FLASH_PAGES : (unsigned)b.low | ((unsigned)b.hi & EF_MASK) << 7;
}

static void map_page(int idx, unsigned page) {
	//print("map_page idx %d page %d\n", idx, page);
	if(page > FLASH_PAGES + 3) uses_hi_ram = 1;
	map_in(idx, ROM_PAGE(page), page < FLASH_PAGES);
}

static void map_page_st(int idx, struct bank page) {
	map_page(idx, off_for_bank(page));
}

unsigned mmap_check_endboot(uint16_t pc){
	//printf("j %04x\n", pc);
	if(normal) return 1;
	if(pc >= 0x4000 && pc < (mmu_mode ? 0xC000 : 0x8000)){
		printf("end boot\n");
		normal = 1;
		map_in(0, ROM_PAGE(0), 1);
	}
	return normal;
}

uint8_t *flash;
uint8_t *ram;

void mmap_set_mode(uint8_t mode){
#ifdef MMU_DEBUG
	printf("mmu mode -> %d\n", mode);
#endif
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

struct bank banks[3] = {0};


uint8_t mmap_in(uint8_t port){
	//printf("%d -> %02x\n", port, banks[bfp(port)]);
	if(port == 5) return banks[2].low & ~0x80;
	return banks[bfp(port)].low;
}

#ifdef MMU_DEBUG
void empirical_map_test(uint16_t *a, uint16_t b) {
	int i;
	for(i = 0; i < 0x2000; i++) {
		uint16_t r = rand();
		a[i] = r;//(uint16_t)(r >> 8) | (uint16_t)(r << 8);
		uint16_t bb = cpu_read16(b + i * 2);
		if(bb != r) {
			printf("not consistent - %p = %04x, 0x%04x = %04x\n", a + i, r, b + i * 2, bb);
			//sleep(50);
		}
	}
}

void mmu_test() {
	testing = 1;
	printf("mmu test\n");
	mmap_set_mode(0);
	int i;
	for(i = 0; i < 0x08; i++) {
		mmu_port5_out(i);
		empirical_map_test(RAM_PAGE(i), 0xc000);
	}
	for(i = 0; i < 0x80; i++) {
		mmu_port67_out(i, 6);
		empirical_map_test(ROM_PAGE(i), 0x4000);
	}
	for(i = 0; i < 0x80; i++) {
		mmu_port67_out(i, 7);
		empirical_map_test(ROM_PAGE(i), 0x8000);
	}
	printf("end mmu test\n");
	testing = 0;
}
#endif

#define STR(x) #x
#define SF(x) STR(x)

void __attribute__((interrupt("ABORT"), naked)) abort_handler(){
	asm volatile(
	"push {r0, r1, r2}\n" // r2 is placeholder
"	mov r0, #1\n"
"	mrc p15, 0, r0, c6, c0, 0\n"
"	subs r0,  #" SF(I_Z80_MEM_BASE) "\n"
"	movge r1, #0x10000\n"
"	cmpge r1, r0\n"
"	popge {r0, r1, r2}\n"
"	subges pc, lr, #4\n"
"	adr r0, 1f\n"
"	ldr r1, [r0]\n"
"	add r0, r0, r1\n"
//"	ldr r0, =o_dah\n"
//"	ldr r0, o_dah\n"
"	ldr r0, [r0]\n"
"	str r0, [sp, #8]\n"
"	pop {r0, r1, pc}\n"
"1:	.word o_dah-.\n"
	);
}

void (*o_dah)(void);

//typedef volatile void * vvptr;

void mprotect_init() {
	volatile void **ivt = (volatile void **)0x20;
	o_dah = ivt[4];
	//printf("o_dah = %08x\n", o_dah);
	ivt[4] = abort_handler;
	
	//*(uint32_t *)0xf0000000 = 0xffffffff;
}

void mprotect_end() {
	volatile void **ivt = (volatile void **)0x20;
	ivt[4] = o_dah;
}

void mmu_init() {
	mprotect_init();
	mmu_base = get_mmu_addr();
	printf("mmu_base %p\n", mmu_base);
	mem_base_al = x_aligned_alloc(0x1000, RAM_SIZE + FLASH_SIZE);
	mem_base = mem_base_al.ptr;
	int t2_size = 0x100;
	section_base_al = x_aligned_alloc(0x1000, 2 * 4 * t2_size);
	//section_base_l_al = x_aligned_alloc(0x400, 4 * t2_size);
	
	section_base_l = section_base_al.ptr;
	section_base = section_base_l + t2_size;//section_base_l_al.ptr;
	int i;
	for(i = 0; i < t2_size; i++) {
		section_base[i] = 0;
	}
	for(i = 0; i < t2_size; i++) {
		section_base_l[i] = 0;
	}
	printf("section_base=%p section_base_l=%p\n", section_base, section_base_l);
	section_base[0x40] = (uintptr_t)section_base | 0b111111111010; // write-through cache, full permissions
	//section_base[0xf1] = (uintptr_t)section_base_l | 0b111011;
	
	uint32_t aa = get_cr1();
	printf("cr1=%08lx cr3=%08lx\n", aa, get_cr3());
	set_cr1(aa | 1<<9); // enable ROM protection
	//printf("cr1 = %08x", get_cr1());
	memset(mem_base, 0, RAM_SIZE + FLASH_SIZE);
	//invalidate_tlb_all();

	/*mmu_port5_out(0);
	mmu_port67_out(0, 6);
	mmu_port67_out(0, 7);*/
	
	// domain 1, Client mode
	set_cr3(get_cr3() | (1 << 2));
	mmu_base[0xdff] = (uintptr_t)section_base_l | 0b000110001;
	mmu_base[0xe00] = (uintptr_t)section_base | 0b000110001;
	clean_inval_dcache_all();
	invalidate_tlb_all();
	//invalidate_tlb(0xe00f0000);
	//invalidate_tlb(0xe00f1000);

	//asm("	bkpt #1\n");
	map_in(0, ROM_PAGE(BOOT_PAGE), 1);
	
	flash = mem_base;
	ram = mem_base + FLASH_SIZE;
	printf("flash = %p, ram = %p\n", flash, ram);
#ifdef MMU_DEBUG
	//mmu_test();
#endif
	mmu_port5_out(0);
	mmu_port67_out(0, 6);
	mmu_port67_out(0, 7);
	//asm("	bkpt #0\n");
	//*(uint32_t *)0xe0000000 = 0x12345678;
}


static void update67() {
	if(mmu_mode == 0) {
		map_page_st(1, banks[0]);
		map_page_st(2, banks[1]);
		map_page_st(3, banks[2]);
	} else {
		map_page(1, off_for_bank(banks[0]) & ~1);
		map_page(2, off_for_bank(banks[0]) | 1);
		map_page_st(3, banks[1]);
	}
	//clear_cache();
}

static void mmu_port5_update() {
	if(mmu_mode == 0) {
		map_page_st(3, banks[2]);
		//clear_cache();
	}
}

void mmu_port5_out(uint8_t val) {
#ifdef MMU_DEBUG
	if(!testing) printf("wrote %02x to port 05\n", val);
#endif
	banks[2].low = val | 0x80;
	mmu_port5_update();
}

static void mmu_port67_update(uint8_t port) {
	if(mmu_mode == 0) {
		map_page_st(port, banks[port - 1]);
	} else if(port == 1) {
		map_page(1, off_for_bank(banks[0]) & ~1);
		map_page(2, off_for_bank(banks[0]) | 1);
	} else if(port == 2) {
		map_page_st(3, banks[1]);
	}
}

void mmu_port67_out(uint8_t val, uint8_t port) {
#ifdef MMU_DEBUG
	if(!testing) printf("wrote %02x to port %02x\n", val, port);
#endif
	banks[port - 6].low = val;
	mmu_port67_update(port - 5);
	//clear_cache();
}

void mmu_portEF_out(uint8_t val, uint8_t port) {
#ifdef MMU_DEBUG
	if(!testing) printf("wrote %02x to port %02x\n", val, port);
#endif
	banks[port - 0xe].hi = val;
	mmu_port67_update(port - 0xd);
}

uint8_t mmu_portEF_in(uint8_t port) {
	return banks[port - 0xe].hi;
}

void mmu_end() {
	//int i;
	clean_inval_dcache_all();
	mmu_base[0xdff] = 0;
	mmu_base[0xe00] = 0;
	//clear_cache();
	set_cr3(get_cr3() & ~(1 << 2));
	set_cr1(get_cr1() & ~(1<<9)); // disable ROM protection
	mprotect_end();
	x_aligned_free(section_base_al);
	x_aligned_free(mem_base_al);
}

void mmap_save(FILE *f) {
	fputc(mmu_mode, f);
	FWRITE_VALUE(banks, f);
	fputc(normal, f);
}

void mmap_restore(FILE *f) {
	mmu_mode = fgetc(f);
	FREAD_VALUE(&banks, f);
	mmu_port5_update();
	mmu_port67_update(0);
	mmu_port67_update(1);
	normal = fgetc(f);
	if(normal) map_in(0, ROM_PAGE(0), 1);
}

#include <os.h>
#include <stdint.h>
#include "drz80.h"
#include "interrupt.h"
#include "z_interrupt.h"
#include "io.h"
#include "mmu_mmap.h"
#include "lcd.h"
#include "timer.h"
#include "speedcontrol.h"
#include "savestate.h"
#include "rtc.h"
#include <stdarg.h>
#include <stddef.h>
#include "navnet-io.h"

#include "keys.h"

void cpu_init();
unsigned int cpu_rebasePC(uint32_t x);
unsigned int cpu_rebaseSP(uint32_t x);
unsigned short cpu_read16(uint32_t idx);
unsigned char cpu_read8(uint32_t idx);
void cpu_write16(unsigned short val, uint32_t idx);
void cpu_write8(unsigned char val, uint32_t idx);
unsigned char cpu_in(unsigned short port);
//static unsigned char cpu_in_(unsigned short port);
void cpu_out(unsigned short port, unsigned char val);
void cpu_irq_callback();

uint8_t port_get(uint8_t pn, struct z80port *p);
void port_set(uint8_t pn, struct z80port *p, uint8_t val);

#ifdef USE_NAVNETIO

nn_stream g_stream;

size_t __wrap_printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	size_t z;
	if(g_stream == NULL) {
		z = vprintf(format, args);
	} else {
		//vprintf(format, args);
		z = navnet_io_vprintf(g_stream, format, args);
	}
	va_end(args);
	return z;
}

size_t __wrap_puts(const char *str) {
	return printf("%s\n", str);
}
#endif
//#define printf(...)


//uint8_t *flash;
struct DrZ80 ZCpu;
uint32_t port_debug = 0;
//volatile extern unsigned dah_happened;

int main(int argc, char **argv){
#ifdef USE_NAVNETIO
	navnet_io_early();
	g_stream = navnet_io_init();
#endif
	printf("main = %p\n", main);
	
	if(argc == 1){
		cfg_register_fileext("8rom", "nspire-z80");
		cfg_register_fileext("8sav", "nspire-z80");
		show_msgbox("Info", "File extension registered. Open a 8rom file to use.");
		return 0;
	}
	printf("begin\n");
	mmu_init();
	printf("mmu_init done\n");
#ifndef NO_LCD
#ifdef USE_CSE
	cselcd_init();
#else
	m_lcd_init();
#endif
#endif
	cpu_init();
	
	io_init();
	printf("lcd, cpu, io done\n");
	
	int l = strlen(argv[1]);
	char *sav_romname = NULL;
	if(strncmp(argv[1] + l - 4 - 4, "8sav", 4) == 0){
		savestate_load(argv[1], &sav_romname);
	}else{
		FILE *romfile;
		if(!(romfile = fopen(argv[1], "rb"))){
			show_msgbox("Error", "Could not open rom");
			return 1;
		}
		fseek(romfile, 0, SEEK_END);
		int romsize = ftell(romfile);
		fseek(romfile, 0, 0);
		//printf("%d\n", romsize);
		//flash = calloc(0x20000, 1);
		printf("flash=%08x\n", flash);
		fread(flash, sizeof(char), romsize, romfile);
		fclose(romfile);
		clear_cache();
	}
	speedcontrol_init();
	interrupt_init();
	//memset(REAL_SCREEN_BASE_ADDRESS, 2, 100);
	int cycles_to_run = timer_after(0);
	int i = 0;
	
	timer_set_enabled(1);
	timer_freq_set(3);
	// uint32_t *mem32 = (uint32_t *)0xe0000000;
	// printf("%08x %08x %08x\n", mem32[0], mem32[1], mem32[2]);
	// asm volatile("b .\n\t");
	uint32_t p_pressed = 0;
	while(1){
		speedcontrol_before();
		//puts("loop a");
		int cycles_left = /*(ZCpu.Z80IF & Z80_HALT) && (ZCpu.Z80_IRQ == 0) ? 0 : */DrZ80Run(&ZCpu, cycles_to_run);
		int cycles_elapsed = cycles_to_run == cycles_left ? 1000 : cycles_to_run - cycles_left;
		//puts("loop b");
		if(isKeyPressed(KEY_NSPIRE_CAT)) {
			char *pc = (char *)ZCpu.Z80PC;
			int pcb = ZCpu.Z80PC - ZCpu.Z80PC_BASE;
			printf("%d %d %04x %02x%02x%02x%02x\n", i++, cycles_elapsed, pcb, pc[0], pc[1], pc[2], pc[3]);//, ZCpu.Z80BC);
			printf("a	%02x	f	%02x	bc	%04x	de	%04x	hl	%04x\n", ZCpu.Z80A >> 24, ZCpu.Z80F >> 24, ZCpu.Z80BC >> 16, ZCpu.Z80DE >> 16, ZCpu.Z80HL >> 16);
			printf("sp	%04x	ix	%04x	iy	%04x\n", ZCpu.Z80SP - ZCpu.Z80SP_BASE, ZCpu.Z80IX >> 16, ZCpu.Z80IY >> 16);
		}
		//puts("loop c");
		
		port_debug = isKeyPressed(KEY_NSPIRE_P);
		//if(a && !p_pressed) {
		//	p_pressed = 1;
		//	port_debug ^= 1;
		//}
		//if(p_pressed && !a) p_pressed = 0;
		//if(dah_happened) {
		//	printf("data abort happened at %p\n", dah_happened);
		//	dah_happened = 0;
		//}
		cycles_to_run = timer_after(cycles_elapsed);
		if(isKeyPressed(KEY_NSPIRE_ESC)) break;
		speedcontrol_after(cycles_elapsed);
	}
	interrupt_end();
	
	if(sav_romname){
		savestate_save(sav_romname);
		free(sav_romname);
	}else{
		savestate_save(argv[1]);
		refresh_osscr();
	}
#ifndef NO_LCD
#ifdef USE_CSE
	cselcd_end();
#else
	lcd_end();
#endif
#endif
	speedcontrol_end();
	mmu_end();
#ifdef USE_NAVNETIO
	navnet_io_end(g_stream);
#endif
	
	return 0;
}

void cpu_trace() {
	uint8_t *pc = ZCpu.Z80PC;
	printf("XX XX %04x %02x%02x%02x%02x\n", pc - ZCpu.Z80PC_BASE, pc[0], pc[1], pc[2], pc[3]);//, ZCpu.Z80BC);
	printf("a   %02x	f   %02x	bc  %04x	de  %04x	hl  %04x\n", ZCpu.Z80A >> 24, ZCpu.Z80F >> 24, ZCpu.Z80BC >> 16, ZCpu.Z80DE >> 16, ZCpu.Z80HL >> 16);
	printf("sp  %04x	ix  %04x	iy  %04x\n", ZCpu.Z80SP - ZCpu.Z80SP_BASE, ZCpu.Z80IX >> 16, ZCpu.Z80IY >> 16);
}

void cpu_init(){
	memset(&ZCpu, 0, sizeof(ZCpu));
	ZCpu.z80_rebasePC= cpu_rebasePC;
	ZCpu.z80_rebaseSP= cpu_rebaseSP;
	ZCpu.z80_read8   = cpu_read8;
	ZCpu.z80_read16  = cpu_read16;
	ZCpu.z80_write8  = cpu_write8;
	ZCpu.z80_write16 = cpu_write16;
	ZCpu.z80_in      =cpu_in;
	ZCpu.z80_out     =cpu_out;
	ZCpu.z80_irq_callback = cpu_irq_callback;
	ZCpu.z80_trace = cpu_trace;
	ZCpu.Z80PC = 0xe0000000;//cpu_rebasePC(0);
	ZCpu.Z80PC_BASE = 0xe0000000;//cpu_rebasePC(0);
	ZCpu.Z80SP = 0xe000ffff;//cpu_rebaseSP(0xffff);
	ZCpu.Z80SP_BASE = 0xe0000000;//cpu_rebaseSP(0);
}

void cpu_irq_callback(){
	//printf("irq\n");
	int_callback();
}

/*void pdb(unsigned int pc){
	printf("pc(a) %02x v %04x\n", pc, cpu_read16(pc));
}*/

unsigned int null_rebasePC(uint32_t x) {
	return 0xe0000000 + (x & 0xffff);
}

unsigned int cpu_rebasePC(uint32_t x){
	//if(x == 0) printf("reset\n");
	//printf("rebasePC 0x%04x\n", x);
	//ZCpu.Z80PC_BASE = (unsigned int)flash;
	//if(mmap_check_endboot(x)) ZCpu.z80_rebasePC = NULL; 
	x &= 0xffff;
	if(mmap_check_endboot(x)) ZCpu.z80_rebasePC = null_rebasePC;
	//ZCpu.Z80PC_BASE = (unsigned int)mmap_base_addr(x);
	//if(ZCpu.Z80PC_BASE + (x & 0x3FFF) != flash + x) printf("pc %08x %p\n", ZCpu.Z80PC_BASE + (x & 0x3FFF), flash + x);
	//printf("pc %p %08x\n", mmap_bank_for_addr(x) + (x & 0x3FFF), ZCpu.Z80PC_BASE + x);
	//return ZCpu.Z80PC_BASE + x;
	return 0xe0000000 + x;
}

unsigned int cpu_rebaseSP(uint32_t x){
	//printf("rebaseSP 0x%lx\n", x);
	//ZCpu.Z80SP_BASE = (unsigned int)flash;
	//ZCpu.Z80SP_BASE = (unsigned int)mmap_base_addr(x);
	//if(ZCpu.Z80SP_BASE + (x & 0x3FFF) != flash + x) printf("sp %08x %p\n", ZCpu.Z80SP_BASE + (x & 0x3FFF), flash + x);
	//return ZCpu.Z80SP_BASE + x;
	return 0xe0000000 + (x & 0xffff);//=(x & 0x3FFF);
} 

unsigned short cpu_read16(uint32_t idx){
	//asm(" b .");
	//uint8_t *p = flash+idx;
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read16 0x%x\n", idx);
	uint8_t *p = 0xe0000000 + (idx & 0xffff);
#ifdef MMU_DEBUG
	if(idx & ~0xffff)
		printf("Invalid read16: %p %04x\n", p, idx);
#endif
	uint16_t val = p[0] | p[1] << 8;
#ifdef MEM16_DEBUG
	printf("Read %04x at %08x\n", val, p);
#endif
	/*if(idx < 0x4000) {
		uint8_t *p2;
		for(p2 = p - 8; p2 < p + 8; p2++) {
			printf("%02x ", *p2);
		}
		printf("\n");
		printf("read16 %p 0x%04x -> %02x %02x = 0x%04x\n", p, idx, p[1], p[0], val);
		asm volatile("b .\n\t");
	}
	val = p[0] | p[1] << 8;
	printf("read16 2.0 %p 0x%04x -> %02x %02x = 0x%04x\n", p, idx, p[1], p[0], val);*/
	return val;
}
unsigned char cpu_read8(uint32_t idx){
	//return flash[idx];
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read8 0x%x\n", idx);
	uint8_t *p = (uint8_t *)0xe0000000 + idx;//(idx & 0xffff);
#ifdef MMU_DEBUG
	if(idx & ~0xffff)
		printf("Invalid read8: %p %04x\n", p, idx);
#endif
	uint8_t val = *p;
	//if(idx < 0x4000) printf("read8 0x%04x -> %02x\n", idx, val);
	return val;
}

void cpu_write16(unsigned short val, uint32_t idx){
	uint8_t *p = 0xe0000000 + (idx & 0xffff);
#ifdef MEM16_DEBUG
	printf("Wrt  %04x at %08x\n", val, p);
#endif
	//if(!((p >= flash && p < flash + FLASH_SIZE) || (p >= ram && p < ram + RAM_SIZE)))
#ifdef MMU_DEBUG
	if(idx & ~0xffff) {
		printf("Invalid write16: %p %04x\n", p, idx);
		//cpu_trace();
		//asm("bkpt\n\t");
	}
#endif
	//if(idx < 0x4000) printf("write16 %p 0x%04x 0x%04x\n", p, idx, val);
	//uint8_t *p = flash + idx;
	p[0] = val & 0xff;
	p[1] = val >> 8;
}

void cpu_write8(unsigned char val, uint32_t idx){
	uint8_t *p = 0xe0000000 + (idx & 0xffff);
	//if(!((p >= flash && p < flash + FLASH_SIZE) || (p >= ram && p < ram + RAM_SIZE)))
#ifdef MMU_DEBUG
	if(idx & ~0xffff)
		printf("Invalid write8: %p %04x\n", p, idx);
#endif
	//if(idx < 0x4000) printf("write8 0x%04x 0x%02x\n", idx, val);
	*p = val;
	//flash[idx] = val;
}
/*unsigned char cpu_in(unsigned short port){
	port &= 0xff;
	uint8_t x = cpu_in_(port);
	//if(port == 1) printf("in %x %x\n", port, x);
	return x;
}*/
unsigned char cpu_in(unsigned short pn_){
	uint8_t pn = (uint8_t)pn_;
	struct z80port *p = &ports[pn];
	uint8_t v = port_get(pn, p);
	if(port_debug) printf("Read %02x from port %02x (%s)\n", v, p->number, p->name);
	return v;
}

void cpu_out(unsigned short pn_, unsigned char val){
	uint8_t pn = (uint8_t)pn_;
	struct z80port *p = &ports[pn];
	//register uint8_t *temp asm("r0");
	//asm(" mov r0, r6");
	//uint16_t zpc = temp - ZCpu.Z80PC_BASE;
	port_set(pn, p, val);
	if(port_debug) printf("Wrote %02x to port %02x (%s)\n", val, p->number, p->name);
	//temp = cpu_rebasePC(zpc);
	//asm(" mov r6, r0"); // do not try this at home
}

uint8_t port_get(uint8_t pn, struct z80port *p){
	if(p->in.n) return p->in.n(pn);
	//if(p->n_in) return p->n_in(p->number);
	if(p->ptr_val) return *(p->ptr_val);
	if(p->mirror) return port_get(pn, p->mirror);
	return p->const_val;
}

void port_set(uint8_t pn, struct z80port *p, uint8_t val){
	if(p->out.n) p->out.n(val, pn);
	//else if(p->n_out) p->n_out(p->number, val);
	else if(p->ptr_val) *(p->ptr_val) = val;
	else if(p->mirror) port_set(pn, p->mirror, val);
}

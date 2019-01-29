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
#include "keypad.h"
#include "main.h"
#include "cselcd.h"
#include <stdarg.h>
#include <stddef.h>
#include "navnet-io.h"

#include "keys.h"

void cpu_init();
void * cpu_rebaseSP(uint16_t x);
unsigned short cpu_read16(uint16_t idx);
unsigned char cpu_read8(uint16_t idx);
void cpu_write16(unsigned short val, uint16_t idx);
void cpu_write8(unsigned char val, uint16_t idx);
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

const struct calc_type calc_types[] = {
{ // 84+SE
	.flash_size = 0x200000,
	.i_flash_size = 1,
	.boot_page = 0x7f,
	.ef_mask = 0,
	.cselcd = 0
},
{ // 84+
	.flash_size = 0x100000,
	.i_flash_size = 0,
	.boot_page = 0x3f,
	.ef_mask = 0,
	.cselcd = 0
},
{ // 84+CSE
	.flash_size = 0x400000,
	.i_flash_size = 2,
	.boot_page = 0xff,
	.ef_mask = 1,
	.cselcd = 1
}
};

struct calc_type g_calc = calc_types[0];
#define N_CALC_TYPES (sizeof(calc_types) / sizeof(calc_types[0]))


//uint8_t *flash;
struct DrZ80 ZCpu;
uint32_t port_debug = 0;
//volatile extern unsigned aaa;

int main(int argc, char **argv){
	if(argc == 1){
		//*dot = '\0';
		cfg_register_fileext("8rom", "nspire-z80");
		cfg_register_fileext("8sav", "nspire-z80");
		//*dot = '.';
		show_msgbox("Info", "File extension registered. Open a 8rom file to use.");
		return 0;
	}
#ifdef USE_NAVNETIO
	navnet_io_early();
	g_stream = navnet_io_init();
#endif
	printf("main = %p\n", main);
	enable_relative_paths(argv);
	keypad_set_type(KEYPAD_NSPIRE);
	FILE *cfg_file = fopen("nspire-z80.cfg.tns", "r");
	if(cfg_file) {
		puts("Reading config file...");
		fseek(cfg_file, 0, SEEK_END);
		int cfg_size = ftell(cfg_file);
		fseek(cfg_file, 0, 0);
		char *cfg = malloc(cfg_size + 1);
		fread(cfg, 1, cfg_size, cfg_file);
		cfg[cfg_size] = 0;
		fclose(cfg_file);
		char *cur_cfg = cfg;
		puts(cfg);
#define L_STRNCMP(a, b) strncmp(a, b, strlen(b))
		while(cur_cfg < cfg + cfg_size) {
			if(L_STRNCMP(cur_cfg, "keypad=") == 0) {
				puts("bbb");
				cur_cfg += strlen("keypad=");
				if(L_STRNCMP(cur_cfg, "84") == 0) {
					keypad_set_type(KEYPAD_84);
					cur_cfg += strlen("84");
				} else if(L_STRNCMP(cur_cfg, "old") == 0) {
					keypad_set_type(KEYPAD_OLD_NSPIRE);
					cur_cfg += strlen("old");
				} else if(L_STRNCMP(cur_cfg, "default") == 0) {
					keypad_set_type(KEYPAD_NSPIRE);
					cur_cfg += strlen("default");
				}
			}
			while(*cur_cfg != '\n' && *cur_cfg != '\0') cur_cfg++;
			cur_cfg++;
		}
		free(cfg);
	} else {
		puts("Couldn't read config file; using default settings");
	}
	
	int l = strlen(argv[1]);
	char *extn = argv[1] + l - 4 - 4;
	char *load_file = argv[1];//[256];
	
	uint8_t sfnl = strlen(load_file);//fgetc(savefile);
	char *romfn = malloc(sfnl+1);
	memcpy(romfn, load_file, sfnl+1);
	memcpy(romfn + sfnl - 8, "8rom", 4);
	
	FILE *l_romfile;
	if(!(l_romfile = fopen(romfn, "rb"))){
		show_msgbox("Error", "Could not open rom");
		return 1;
	}
	fseek(l_romfile, 0, SEEK_END);
	unsigned romsize = ftell(l_romfile);
	fclose(l_romfile);

	unsigned j;
	for(j = 0; j < N_CALC_TYPES; j++) {
		if (calc_types[j].flash_size == romsize) {
			g_calc = calc_types[j];
			break;
		}
	}

	printf("begin\n");
	mmu_init();
	printf("mmu_init done\n");
#ifndef NO_LCD
	if (g_calc.cselcd) {
		cselcd_init();
	} else {
		m_lcd_init();
	}
#endif
	cpu_init();
	
	io_init();
	printf("lcd, cpu, io done\n");
	
	/*if(strncmp(extn, "8lnk", 4) == 0) {
		FILE *lnkfile;
		if(!(lnkfile = fopen(argv[1], "rb"))){
			show_msgbox("Error", "Could not open link");
			return 1;
		}
		int n = fread(load_file, 1, 255, lnkfile);
		load_file[n] = 0;
		extn = load_file + n - 4 - 4;
	} else {*/
		//size_t l2 = strlen(argv[1]);
		//if(l2 > 255) l2 = 255;
		//memcpy(load_file, argv[1], l2);
/*		load_file[l2] = 0;
		extn = load_file + l2 - 4 - 4;
		
		FILE *newlnk = fopen();
	}*/
	
	if(strncmp(extn, "8sav", 4) == 0){
		savestate_load(load_file, romfn);
	}else{
		FILE *romfile;
		if(!(romfile = fopen(load_file, "rb"))){
			show_msgbox("Error", "Could not open rom");
			return 1;
		}
		//printf("%d\n", romsize);
		//flash = calloc(0x20000, 1);
		printf("flash=%p\n", flash);
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
	while(1){
		speedcontrol_before();
		//printf("%d\n", cycles_to_run);
		int cycles_left = /*(ZCpu.Z80IF & Z80_HALT) && (ZCpu.Z80_IRQ == 0) ? 0 : */DrZ80Run(&ZCpu, cycles_to_run);
		int cycles_elapsed = cycles_to_run == cycles_left ? 10000 : cycles_to_run - cycles_left;
		/*if(isKeyPressed(KEY_NSPIRE_CAT)) {
			char *pc = (char *)ZCpu.Z80PC;
			int pcb = ZCpu.Z80PC - ZCpu.Z80PC_BASE;
			printf("%d %d %04x %02x%02x%02x%02x\n", i++, cycles_elapsed, pcb, pc[0], pc[1], pc[2], pc[3]);//, ZCpu.Z80BC);
			printf("a	%02x	f	%02x	bc	%04x	de	%04x	hl	%04x\n", ZCpu.Z80A >> 24, ZCpu.Z80F >> 24, ZCpu.Z80BC >> 16, ZCpu.Z80DE >> 16, ZCpu.Z80HL >> 16);
			printf("sp	%04x	ix	%04x	iy	%04x\n", ZCpu.Z80SP - ZCpu.Z80SP_BASE, ZCpu.Z80IX >> 16, ZCpu.Z80IY >> 16);
		}
		
		port_debug = isKeyPressed(KEY_NSPIRE_P);*/
		//if(aaa) {
		//	puts("aaa");
		//	aaa = 0;
		//}
		cycles_to_run = timer_after(cycles_elapsed);
		if(isKeyPressed(KEY_NSPIRE_ESC)) break;
		speedcontrol_after(cycles_elapsed);
	}
	interrupt_end();
	
	savestate_save(romfn);
	free(romfn);
#ifndef NO_LCD
	if (g_calc.cselcd) {
		cselcd_end();
	} else {
		lcd_end();
	}
#endif
	speedcontrol_end();
	mmu_end();
#ifdef USE_NAVNETIO
	navnet_io_end(g_stream);
#endif
	
	return 0;
}

void cpu_trace() {
	uint8_t *pc = (uint8_t *)ZCpu.Z80PC;
	printf("XX XX %04x %02x%02x%02x%02x\n", (uintptr_t)(pc - ZCpu.Z80PC_BASE), pc[0], pc[1], pc[2], pc[3]);//, ZCpu.Z80BC);
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

void * null_rebasePC(uint16_t x) {
	return Z80_MEM_BASE + (x & 0xffff);
}

void * cpu_rebasePC(uint16_t x){
#ifdef DRZ80MEM_DEBUG
	if(x & ~0xffff)
		printf("Invalid rebasePC: %08x\n", x);
#endif
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
	return Z80_MEM_BASE + x;
}

void * cpu_rebaseSP(uint16_t x){
#ifdef DRZ80MEM_DEBUG
	if(x & ~0xffff)
		printf("Invalid rebaseSP: %08x\n", x);
#endif
	//printf("rebaseSP 0x%lx\n", x);
	//ZCpu.Z80SP_BASE = (unsigned int)flash;
	//ZCpu.Z80SP_BASE = (unsigned int)mmap_base_addr(x);
	//if(ZCpu.Z80SP_BASE + (x & 0x3FFF) != flash + x) printf("sp %08x %p\n", ZCpu.Z80SP_BASE + (x & 0x3FFF), flash + x);
	//return ZCpu.Z80SP_BASE + x;
	return Z80_MEM_BASE + (x & 0xffff);//=(x & 0x3FFF);
} 

unsigned short cpu_read16(uint16_t idx){
	//asm(" b .");
	//uint8_t *p = flash+idx;
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read16 0x%x\n", idx);
	uint8_t *p = Z80_MEM_BASE + (idx & 0xffff);
#ifdef DRZ80MEM_DEBUG
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
unsigned char cpu_read8(uint16_t idx){
	//return flash[idx];
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read8 0x%x\n", idx);
	uint8_t *p = Z80_MEM_BASE + idx;//(idx & 0xffff);
#ifdef DRZ80MEM_DEBUG
	if(idx & ~0xffff)
		printf("Invalid read8: %p %04x\n", p, idx);
#endif
	uint8_t val = *p;
	//if(idx < 0x4000) printf("read8 0x%04x -> %02x\n", idx, val);
	return val;
}

void cpu_write16(unsigned short val, uint16_t idx){
	uint8_t *p = Z80_MEM_BASE + (idx & 0xffff);
#ifdef MEM16_DEBUG
	printf("Wrt  %04x at %08x\n", val, p);
#endif
	//if(!((p >= flash && p < flash + FLASH_SIZE) || (p >= ram && p < ram + RAM_SIZE)))
#ifdef DRZ80MEM_DEBUG
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

void cpu_write8(unsigned char val, uint16_t idx){
	uint8_t *p = Z80_MEM_BASE + (idx & 0xffff);
	//if(!((p >= flash && p < flash + FLASH_SIZE) || (p >= ram && p < ram + RAM_SIZE)))
#ifdef DRZ80MEM_DEBUG
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
	if(port_debug) printf("Writing %02x to port %02x (%s)\n", val, p->number, p->name);
	port_set(pn, p, val);
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

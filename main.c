#include <os.h>
#include <stdint.h>
#include "drz80.h"
#include "interrupt.h"
#include "z_interrupt.h"
#include "io.h"
#include "mmap.h"
#include "lcd.h"
#include "timer.h"

void cpu_init();
unsigned int cpu_rebasePC(unsigned short x);
unsigned int cpu_rebaseSP(unsigned short x);
unsigned short cpu_read16(unsigned short idx);
unsigned char cpu_read8(unsigned short idx);
void cpu_write16(unsigned short val, unsigned short idx);
void cpu_write8(unsigned char val, unsigned short idx);
unsigned char cpu_in(unsigned short port);
//static unsigned char cpu_in_(unsigned short port);
void cpu_out(unsigned short port, unsigned char val);
void cpu_irq_callback();

uint8_t port_get(struct z80port *p);
void port_set(struct z80port *p, uint8_t val);



//#define printf(...)


//uint8_t *flash;
struct DrZ80 ZCpu;
volatile uint8_t flag;

int main(void){
	FILE *romfile;
	if(!(romfile = fopen("rom.tns", "rb"))){
		show_msgbox("Error", "Could not open rom");
		return 1;
	}
	fseek(romfile, 0, SEEK_END);
	int romsize = ftell(romfile);
	fseek(romfile, 0, 0);
	//printf("%d\n", romsize);
	mmap_init();
	//flash = calloc(0x20000, 1);
	fread(flash, sizeof(char), romsize, romfile);
	fclose(romfile);
	
	lcd_init();
	cpu_init();
	
	io_init();
	//asm(" b .");
	printf("%08x\n", ZCpu.Z80PC_BASE - (unsigned int)flash);
	printf("%08x\n", ZCpu.Z80PC - (unsigned int)flash);
	//printf("%08x\n", cpu_read16(0));
	//printf("%p\n", mmap_bank_for_addr(0));
	/*for(i = 0; i < 0x100; i++){
		printf("%02x", cpu_read8(i));
	}*/
	interrupt_init();
	int cycs = timer_after(0);
	int i = 0;
	
	timer_set_enabled(1);
	timer_freq_set(3);
	while(1){
		int cyce = DrZ80Run(&ZCpu, cycs);
		//printf("%d\n", cyce);
		char *pc = (char *)ZCpu.Z80PC;
		//int pcb = ZCpu.Z80PC - ZCpu.Z80PC_BASE;
		//printf("%d %d %04x %02x%02x%02x%02x\n", i++, cyce, pcb, pc[0], pc[1], pc[2], pc[3]);//, ZCpu.Z80BC);
		cycs = timer_after(cycs == cyce ? 100 : cycs - cyce);
		if(isKeyPressed(KEY_NSPIRE_ESC)) break;
		if(flag){
			//printf("fire %02x\n", flag);
			flag = 0;
		}
	}
	lcd_end();
	interrupt_end();
	mmap_end();
	
	return 0;
}


void cpu_init(){
	memset(&ZCpu, 0, sizeof(ZCpu));
	ZCpu.z80_rebasePC=cpu_rebasePC;
	ZCpu.z80_rebaseSP=cpu_rebaseSP;
	ZCpu.z80_read8   =cpu_read8;
	ZCpu.z80_read16  =cpu_read16;
	ZCpu.z80_write8  =cpu_write8;
	ZCpu.z80_write16 =cpu_write16;
	ZCpu.z80_in      =cpu_in;
	ZCpu.z80_out     =cpu_out;
	ZCpu.z80_irq_callback = cpu_irq_callback;
	ZCpu.Z80PC = cpu_rebasePC(0);
	ZCpu.Z80SP = 0xffff;//cpu_rebaseSP(0xffff);
}

void cpu_irq_callback(){
	//printf("irq\n");
	int_callback();
}

void pdb(unsigned int pc){
	printf("pc(a) %02x v %04x\n", pc, cpu_read16(pc));
}

unsigned int cpu_rebasePC(unsigned short x){
	//if(x == 0) printf("reset\n");
	//printf("rebasePC 0x%x\n", x);
	//ZCpu.Z80PC_BASE = (unsigned int)flash;
	mmap_check_endboot(x);
	ZCpu.Z80PC_BASE = (unsigned int)mmap_base_addr(x);
	//if(ZCpu.Z80PC_BASE + (x & 0x3FFF) != flash + x) printf("pc %08x %p\n", ZCpu.Z80PC_BASE + (x & 0x3FFF), flash + x);
	//printf("pc %p %08x\n", mmap_bank_for_addr(x) + (x & 0x3FFF), ZCpu.Z80PC_BASE + x);
	//return ZCpu.Z80PC_BASE + x;
	return ZCpu.Z80PC_BASE + x;//(x & 0x3FFF);
}

unsigned int cpu_rebaseSP(unsigned short x){
	printf("rebaseSP 0x%x\n", x);
	//ZCpu.Z80SP_BASE = (unsigned int)flash;
	ZCpu.Z80SP_BASE = (unsigned int)mmap_base_addr(x);
	//if(ZCpu.Z80SP_BASE + (x & 0x3FFF) != flash + x) printf("sp %08x %p\n", ZCpu.Z80SP_BASE + (x & 0x3FFF), flash + x);
	//return ZCpu.Z80SP_BASE + x;
	return ZCpu.Z80SP_BASE + x;//(x & 0x3FFF);
}

unsigned short cpu_read16(unsigned short idx){
	//printf("read16 0x%x\n", idx);
	//asm(" b .");
	//uint8_t *p = flash+idx;
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read16 0x%x\n", idx);
	uint8_t *p = mmap_z80_to_arm(idx);
	return p[0] | p[1] << 8;
}
unsigned char cpu_read8(unsigned short idx){
	//printf("read8 0x%x\n", idx);
	//return flash[idx];
	//if(mmap_z80_to_arm(idx) != flash+idx) printf("read8 0x%x\n", idx);
	return *(mmap_z80_to_arm(idx));
}

void cpu_write16(unsigned short val, unsigned short idx){
	uint8_t *p = mmap_z80_to_arm(idx);
	if(!((p >= flash && p < flash + 0x200000) || (p >= ram && p < ram + 0x20000)))
		printf("Invalid memory access: %p %04x\n", p, idx);
	//printf("write16 %08x (0x%04x) 0x%04x\n", p, idx, val);
	//uint8_t *p = flash + idx;
	p[0] = val & 0xff;
	p[1] = val >> 8;
}

void cpu_write8(unsigned char val, unsigned short idx){
	uint8_t *p = mmap_z80_to_arm(idx);
	if(!((p >= flash && p < flash + 0x200000) || (p >= ram && p < ram + 0x20000)))
		printf("Invalid memory access: %p %04x\n", p, idx);
	//printf("write8 %08x (0x%04x) 0x%02x\n", ptr, idx, val);
	*p = val;
	//flash[idx] = val;
}
/*unsigned char cpu_in(unsigned short port){
	port &= 0xff;
	uint8_t x = cpu_in_(port);
	//if(port == 1) printf("in %x %x\n", port, x);
	return x;
}*/
unsigned char cpu_in(unsigned short pn){
	struct z80port *p = &ports[(uint8_t)pn];
	uint8_t v = port_get(p);
	//printf("Read %02x from port %02x (%s)\n", v, p->number, p->name);
	return v;
}

void cpu_out(unsigned short pn, unsigned char val){
	struct z80port *p = &ports[(uint8_t)pn];
	//register uint8_t *temp asm("r0");
	//asm(" mov r0, r6");
	//uint16_t zpc = temp - ZCpu.Z80PC_BASE;
	port_set(p, val);
	//printf("Wrote %02x to port %02x (%s)\n", val, p->number, p->name);
	//temp = cpu_rebasePC(zpc);
	//asm(" mov r6, r0"); // do not try this at home
}

uint8_t port_get(struct z80port *p){
	if(p->in.n) return p->in.n(p->number);
	//if(p->n_in) return p->n_in(p->number);
	if(p->ptr_val) return *(p->ptr_val);
	return p->const_val;
}

void port_set(struct z80port *p, uint8_t val){
	if(p->out.n) p->out.n(val, p->number);
	//else if(p->n_out) p->n_out(p->number, val);
	else if(p->ptr_val) *(p->ptr_val) = val;
}
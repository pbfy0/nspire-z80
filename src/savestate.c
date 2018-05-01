#include "util.h"

#include "lcd.h"
#include "io.h"
#include "keypad.h"
#include "mmu_mmap.h"
#include "rtc.h"
#include "timer.h"
#include "z_interrupt.h"
#include "drz80.h"
#include "main.h"
#include <os.h>

#define N_PAGES (FLASH_SIZE / 0x4000)

void savestate_save(char *romfn){
	int rnl = strlen(romfn);
	char savefn[rnl + 1];
	memcpy(savefn, romfn, rnl + 1);
	memcpy(savefn + rnl - 8, "8sav", 4);
	//printf("Saving to %s\n", savefn);
	FILE *savefile = fopen(savefn, "wb");
	printf("PC=%08x, rel=%08x\n", ZCpu.Z80PC, ZCpu.Z80PC - ZCpu.Z80PC_BASE);
	if(uses_hi_ram) puts("Uses high ram");
	ZCpu.Z80PC -= ZCpu.Z80PC_BASE;
	fwrite(&ZCpu, sizeof(struct DrZ80Regs), 1, savefile);
	ZCpu.Z80PC += ZCpu.Z80PC_BASE;
	//fseek(savefile, sizeof(struct DrZ80Regs), SEEK_CUR);
	uint32_t *page = malloc(0x4000);
	uint32_t *f32 = (uint32_t *)flash;
	uint8_t dps[N_PAGES] = {0};
	uint8_t ndp = 0;
	FILE *romfile = fopen(romfn, "rb");
	unsigned i;
	unsigned fi = 0;
	for(i = 0; i < N_PAGES; i++){
		fread(page, 0x4000, 1, romfile);
		unsigned j;
		for(j = 0; j < 0x1000; j++){
			if(f32[fi++] != page[j]){
				dps[i] = 1;
				ndp++;
				break;
			}
		}
	}
	free(page);
	fclose(romfile);
	//fputc(strlen(romfn), savefile);
	//fwrite(romfn, strlen(romfn), 1, savefile);
	fputc(ndp, savefile);
	for(i = 0; i < N_PAGES; i++){
		if(dps[i]){
			fputc(i, savefile);
			fwrite(&flash[i * 0x4000], 0x4000, 1, savefile);
		}
	}
	fwrite(ram, RAM_SIZE, 1, savefile);
	lcd_save(savefile);
	io_save(savefile);
	keypad_save(savefile);
	mmap_save(savefile);
	int_save(savefile);
	rtc_save(savefile);
	timer_save(savefile);
	
	fclose(savefile);
}

void savestate_load(char *savefn, char **romfn_p){
	printf("Starting savestate load\n");
	FILE *savefile = fopen(savefn, "rb");
	
	/*fseek(savefile, 0, SEEK_END);
	int ss = ftell(savefile);
	fseek(savefile, 0, 0);
	int qq;
	for(qq = 0; qq < ss; qq++){
		printf("%02x", fgetc(savefile));
	}
	fseek(savefile, 0, 0);*/
	
	fread(&ZCpu, sizeof(struct DrZ80Regs), 1, savefile);
	//fseek(savefile, sizeof(struct DrZ80Regs), SEEK_CUR);
	uint16_t zpc = ZCpu.Z80PC;
	uint8_t sfnl = strlen(savefn);//fgetc(savefile);
	char *romfn = malloc(sfnl+1);
	memcpy(romfn, savefn, sfnl+1);
	memcpy(romfn + sfnl - 8, "8rom", 4);
	*romfn_p = romfn;
		
	printf("Loading rom from %s\n", romfn);
	
	FILE *romfile = fopen(romfn, "rb");
	fseek(romfile, 0, SEEK_END);
	int romsize = ftell(romfile);
	fseek(romfile, 0, 0);
	fread(flash, romsize, 1, romfile);
	fclose(romfile);
	
	
	//printf("Loaded rom\n");
	
	uint8_t n_dirty_pages = fgetc(savefile);
	int i;
	for(i = 0; i < n_dirty_pages; i++){
		uint8_t dp_n = fgetc(savefile);
		//printf("Loading dirty page %02x", dp_n);
		fread(&flash[dp_n * 0x4000], 0x4000, 1, savefile);
	}
	
	//printf("Loaded dirty pages\n");
	fread(ram, RAM_SIZE, 1, savefile);
	//fseek(savefile, RAM_SIZE, SEEK_CUR);
	
	//printf("Loaded ram\n");
	lcd_restore(savefile);
	io_restore(savefile);
	keypad_restore(savefile);
	mmap_restore(savefile);
	int_restore(savefile);
	rtc_restore(savefile);
	timer_restore(savefile);
	//printf("Loaded peripherals\n");
	
	//printf("zpc %04x\n", zpc);
	ZCpu.Z80PC = (uintptr_t)cpu_rebasePC(zpc);
	//printf("rebased=%08x\n", ZCpu.Z80PC);
	
	fclose(savefile);
}

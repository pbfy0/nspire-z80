#ifndef MMU_MMAP_H
#define MMU_MMAP_H
#include <stdint.h>
#include <stdio.h>

#include "calctype.h"

void mmu_port67_out(uint8_t val, uint8_t port);
void mmu_port5_out(uint8_t val);
uint8_t mmap_in(uint8_t port);

void mmu_portEF_out(uint8_t val, uint8_t port);
uint8_t mmu_portEF_in(uint8_t port);

void map_framebuffer(void *buf);

unsigned mmap_check_endboot(uint16_t pc);
void mmap_set_mode(uint8_t mode);

void mmu_init();
void mmu_end();

void mmap_save(FILE *f);
void mmap_restore(FILE *f);

extern uint8_t *flash;
extern uint8_t *ram;
extern unsigned uses_hi_ram;

#define I_Z80_MEM_BASE 0xe0000000
#define Z80_MEM_BASE ((uint8_t *)I_Z80_MEM_BASE)

#endif
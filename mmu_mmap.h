#ifndef MMU_MMAP_H
#define MMU_MMAP_H
#include <stdint.h>

void * get_mmu_addr();
void set_mmu_addr(void *a);
uint32_t get_mmu_status();

void mmu_port67_out(uint8_t val, uint8_t port);
void mmu_port5_out(uint8_t val);
uint8_t mmap_in(uint8_t port);

extern uint8_t *flash;
extern uint8_t *ram;

#ifdef USE_CSE
#define FLASH_SIZE 0x400000
#else
#define FLASH_SIZE 0x200000
#endif
#define RAM_SIZE 0x20000
#endif
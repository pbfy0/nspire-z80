typedef struct {
	uint8_t lo;
	uint8_t hi;
	uint8_t *addr;
	uint8_t is_ram;
} membank;

struct mb_status {
	uint8_t low;
	uint8_t hi;
};

void mmap_init();
void mmap_end();
void mmap_check_endboot(uint16_t pc);
void mmap_normal_init();
uint8_t *mmap_z80_to_arm(uint16_t z80addr);
uint8_t *mmap_bank_for_addr(uint16_t z80addr);
uint8_t *mmap_base_addr(uint16_t z80addr);
void mmap_set_mode(uint8_t mode);
void mmap_out(uint8_t port, uint8_t val);
uint8_t mmap_in(uint8_t port);
void mmap_hi_out(uint8_t val, uint8_t port);
uint8_t mmap_hi_in(uint8_t port);
void switch_bank(int b);
uint16_t mmap_get_active_page(uint16_t z80addr);

extern uint8_t *flash;
extern uint8_t *ram;

#ifdef USE_CSE
#define FLASH_SIZE 0x400000
#else
#define FLASH_SIZE 0x200000
#endif
#define RAM_SIZE 0x20000

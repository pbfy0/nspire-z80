typedef struct {
	int active_page;
	uint8_t *addr;
	uint8_t is_ram;
} membank;

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
void switch_bank(int b, uint8_t v);

extern uint8_t *flash;
extern uint8_t *ram;

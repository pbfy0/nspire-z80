#ifndef NZ_KEYPAD_H
#define NZ_KEYPAD_H
void keypad_write(uint8_t val);
uint8_t keypad_read();

void keypad_save(FILE *f);
void keypad_restore(FILE *f);

enum z80_keypad_type {
	KEYPAD_NSPIRE = 0,
	KEYPAD_84 = 1,
	KEYPAD_OLD_NSPIRE = 2
};

void keypad_set_type(enum z80_keypad_type val);
#endif

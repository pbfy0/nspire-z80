void keypad_write(uint8_t val);
uint8_t keypad_read();

void keypad_save(FILE *f);
void keypad_restore(FILE *f);
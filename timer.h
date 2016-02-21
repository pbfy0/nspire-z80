void timer_freq_set(uint8_t val);
void cpu_freq_set(uint8_t val);
uint8_t cpu_freq_get();
int next_timer();
int timer_after(int tstates_left);
void timer_set_enabled(uint8_t mask);

void timer_save(FILE *f);
void timer_restore(FILE *f);
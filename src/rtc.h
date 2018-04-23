void rtc_out(uint8_t val, uint8_t port);
uint8_t rtc_out_in(uint8_t port);
uint8_t rtc_in(uint8_t port);

void rtc_save(FILE *f);
void rtc_restore(FILE *f);
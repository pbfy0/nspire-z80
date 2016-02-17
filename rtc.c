#include <stdint.h>

uint32_t rtc_diff = 0;
uint32_t rtc_out_v = 0;
uint8_t *rtc_optr = (uint8_t *)&rtc_out_v;

volatile uint32_t *nspire_rtc = (uint32_t *)0x90090000;

void rtc_out(uint8_t val, uint8_t port){
	rtc_optr[port-0x41] = val;
	rtc_diff = rtc_out_v - *nspire_rtc;
}
uint8_t rtc_out_in(uint8_t port){
	return rtc_optr[port-0x41];
}

uint8_t rtc_in(uint8_t port){
	uint32_t nv = *nspire_rtc + rtc_diff;
	return ((uint8_t *)&nv)[port-0x45];
}

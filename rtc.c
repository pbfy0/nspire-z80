#include "os.h"
#include "util.h"

int32_t rtc_diff = 0;
union rtc_out {
	uint32_t v;
	uint8_t a[4];
};

union rtc_out rtc_out_v;

volatile uint32_t *nspire_rtc = (uint32_t *)0x90090000;

void rtc_out(uint8_t val, uint8_t port){
	rtc_out_v.a[port-0x41] = val;
	rtc_diff = rtc_out_v.v - *nspire_rtc;
}
uint8_t rtc_out_in(uint8_t port){
	return rtc_out_v.a[port-0x41];
}

uint8_t rtc_in(uint8_t port){
	uint32_t nv = (*nspire_rtc) + rtc_diff;
	return (nv >> ((port - 0x45) * 8)) & 0xff;
	//return ((uint8_t *)&nv)[port-0x45];
}

void rtc_save(FILE *f){
	FWRITE_VALUE(rtc_diff, f);
}

void rtc_restore(FILE *f){
	FREAD_VALUE(&rtc_diff, f);
}
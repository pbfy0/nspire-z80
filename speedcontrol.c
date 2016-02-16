#include <stdint.h>

struct timer {
	uint32_t load;
	uint32_t value;
	uint32_t control;
	uint32_t intclr;
	uint32_t ris;
	uint32_t mis;
	uint32_t bgload;
};

volatile struct timer *timer1 = (struct timer *)0x900C0000;
unsigned cycs_elapsed;

void speedcontrol_init() {
	timer1->load = 16;
	//timer1->value = 0xffffffff;
	timer1->control = 0b10000000;
}

void speedcontrol_after(int cycs) {
	cycs_elapsed += cycs;
	int16_t v = (int16_t)timer1->value;
	if(v > 0) return;
	int i;
	//for(i = 0; i < 1000000; i++);
	//timer1->control &= ~0x80;
	uint32_t elapsed = 16 - v;
	int z80_32k_cycs = cycs_elapsed * 32768 / (cpu_freq_get() ? 15000000 : 6000000);
	int new_cycs = z80_32k_cycs - elapsed;
	if(new_cycs > 0){
		//printf("%d\n", new_cycs);
		timer1->load = new_cycs;
		while((int16_t) timer1->value > 0);
	}
	//printf("%5d %04x %d %f %f\n", cycs_elapsed, v & 0xffff, elapsed, seconds, s_elapsed);
	cycs_elapsed = 0;
	timer1->load = 16;
}
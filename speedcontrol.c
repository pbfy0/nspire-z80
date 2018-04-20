#include <stdint.h>

#define TICKS 0xffff

struct timer {
	uint32_t load;
	uint32_t value;
	uint32_t control;
	uint32_t intclr;
	uint32_t ris;
	uint32_t mis;
	uint32_t bgload;
};

struct timer_bkp {
	uint32_t load;
	uint32_t value;
	uint32_t control;
};

volatile struct timer *timer1 = (struct timer *)0x900C0000;
//volatile struct timer *timer1_2 = (struct timer *)0x900C0020;
unsigned cycs_elapsed;
struct timer_bkp tb;
struct timer_bkp tb_2;

volatile unsigned timer_flag = 0;

void speedcontrol_init() {
	tb = *((struct timer_bkp *)&timer1);
	//tb_2 = *((struct timer_bkp *)&timer1_2);
	//timer1->load = TICKS;
	//timer1->value = 0;
	timer1->control = 0;
	//timer1_2->control = 0;
	
	//timer1->value = 0xffffffff;
	//timer1->control = 0b10000000;
}

void speedcontrol_end() {
	*((struct timer_bkp *)&timer1) = tb;
//	*((struct timer_bkp *)&timer1_2) = tb_2;
}

void speedcontrol_before() {
	timer1->control = 0b00000000;
	timer1->load = TICKS;
	timer1->control = 0b10000000;
}

void speedcontrol_after(int cycs_elapsed) {
	//cycs_elapsed += cycs;
	unsigned v = timer1->value;
	uint32_t elapsed = TICKS - v;
	int z80_32k_cycs = cycs_elapsed * 32768 / (cpu_freq_get() ? 15000000 : 6000000);
	int new_cycs = z80_32k_cycs - elapsed;
	
	if(new_cycs > 0){
		printf("elapsed=%d\n", elapsed);
		printf("32k=%d\n", z80_32k_cycs);
		printf("nc=%d\n", new_cycs);
		timer1->control = 0b00100001;
		timer1->load = new_cycs;
		timer1->control = 0b10100001;
		puts("bb");
		while(!timer_flag) {
			asm volatile("mcr p15, 0, %0, c7, c0, 4\n\t" : : "r"(0)); // WFI
		}
		puts("cc");
		timer1->control = 0;
		timer_flag = 0;
		puts("dd");
	} else {
		timer1->control = 0;
	}
}

void speedcontrol_int() {
	//puts("speedcontrol_int");
	timer1->intclr = 0;
	//timer1->control = 0;
	//*(uint32_t *)0x900A0018 = 0b111111;
	timer_flag = 1;
}
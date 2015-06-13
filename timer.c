#include "z_interrupt.h"
#include "drz80.h"
extern struct DrZ80 ZCpu;

uint8_t cpu_freq = 0;
uint8_t timer_freq = 0;
uint8_t timers_enabled = 0;
int timer_current;

uint16_t timer_cycles[2][4][3] = { // cpu speed, timer speed, timers enabled
	{
		{326,	163,	109},
		{732,	366,	244},
		{1139,	570,	380},
		{1546,	773,	515},
	},
	{
		{130,	65,		43},
		{293,	146,	98},
		{456,	228,	152},
		{618,	309,	206},
	}
};

void timer_set_enabled(uint8_t mask){
	timers_enabled = mask;
}

void timer_freq_set(uint8_t val){
	timer_freq = val;
}

int next_timer(){
	return timer_cycles[cpu_freq][timer_freq][timers_enabled-1];
}

int timer_after(int tstates_left){
	//int elapsed = timer_initial - tstates_left;
	timer_current = tstates_left;
	if(timer_current < 0){
		if(timers_enabled) int_fire(timers_enabled<<1);
		timer_current += next_timer();
	}
	return timer_current;
}

void cpu_freq_set(uint8_t val){
	cpu_freq = val & 1;
}
uint8_t cpu_freq_get(){
	return cpu_freq;
}

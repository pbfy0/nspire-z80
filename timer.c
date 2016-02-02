#include "z_interrupt.h"
#include "drz80.h"
#include <limits.h>
extern struct DrZ80 ZCpu;

uint8_t cpu_freq = 0;
uint8_t timer_freq = 0;
uint8_t timers_enabled = 0;

struct hwtimer {
	int tstates_left;
	uint8_t enabled;
};

void timer_enable(struct hwtimer *t);
void timer_disable(struct hwtimer *t);

struct hwtimer timers[2] = {0};

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
	int i;
	for(i = 0; i < 2; i++){
		if((mask & 1<<i) && !(timers_enabled & 1<<i)) timer_enable(&timers[i]);
		if((timers_enabled & 1<<i) && !(mask & 1<<i)) timer_disable(&timers[i]);
	}
	timers_enabled = mask;
}

void timer_enable(struct hwtimer *t){
	t->tstates_left = next_timer();
	t->enabled = 1;
}

void timer_disable(struct hwtimer *t){
	t->tstates_left = 0;
	t->enabled = 0;
}

void timer_freq_set(uint8_t val){
	timer_freq = val;
}

int next_timer(){
	return timer_cycles[cpu_freq][timer_freq][timers_enabled-1];
}

int timer_after(int elapsed){
	int i;
	for(i = 0; i < 2; i++){
		struct hwtimer *t = &timers[i];
		if(!t->enabled) continue;
		t->tstates_left -= elapsed;
		if(t->tstates_left <= 0){
			int_fire(1<<(i+1));
			t->tstates_left += next_timer();
		}
	}
	
	int tst_left = 1000;//INT_MAX;
	for(i = 0; i < 2; i++){
		struct hwtimer *t = &timers[i];
		if(t->enabled && t->tstates_left < tst_left) tst_left = t->tstates_left;
	}
	
	return tst_left;
}

void cpu_freq_set(uint8_t val){
	cpu_freq = val & 1;
}
uint8_t cpu_freq_get(){
	return cpu_freq;
}

#include "interrupt.h"
#include "z_interrupt.h"
static const t_key KEY_NSPIRE_ON        = KEY_(0x10, 0x200);

static void irq_enable();
static void irq_disable();

static void __attribute__((interrupt("IRQ"))) irq_handler();

struct keypad {
	/*uint8_t scan_mode : 2;
	uint16_t apb_row_wait : 14;
	uint16_t apb_scan_wait : 16;*/
	uint32_t scan_mode;
	/*uint8_t n_rows;
	uint8_t n_cols;
	uint16_t spacer;*/
	uint32_t n_rows_cols;
	uint32_t int_stat;
	uint32_t int_mask;
	uint16_t kp_data[16];
	uint32_t kp_gpio[4];
	uint32_t tp_int_mask;
	uint32_t tp_int_stat;
};

struct kp_bkp {
	uint32_t int_mask;
	uint32_t tp_int_mask;
};

uint32_t isr_backup;
uint32_t ei_backup;
struct kp_bkp kp_bkp;
struct keypad volatile *keypad = (struct keypad *)KEYPAD_BASE;
uint32_t *patch_base;
extern volatile uint8_t flag;


void interrupt_init(){
	puts("A");
	uint32_t swi_addr = *(uint32_t *)0x28;
	patch_base = (uint32_t *)(swi_addr + 0xb0);
	isr_backup = *ISR_ADDR;
	*ISR_ADDR = (uint32_t) irq_handler;
	puts("B");
	
	ei_backup = VIC_REG(0x10);
	VIC_REG(0x14) = ~0; // disable all interrupts
	VIC_REG(0x10) |= 1<<21 | 1<<18 | 1<<16; // lcd, timer 1, keypad
	puts("C");
	kp_bkp.int_mask = keypad->int_mask;
	kp_bkp.tp_int_mask = keypad->tp_int_mask;
	keypad->int_mask = 1<<1;
	keypad->tp_int_mask = 0;
	puts("D");
	patch_ndless_swi();
	puts("E");
	uint8_t i = is_classic;
	(void)i;
	is_touchpad;
	puts("F");
	irq_enable();
	puts("G");
}
void interrupt_end(){
	irq_disable();
	unpatch_ndless_swi();
	*ISR_ADDR = isr_backup;
	keypad->int_mask = kp_bkp.int_mask;
	keypad->tp_int_mask = kp_bkp.tp_int_mask;
	VIC_REG(0x14) = ~0;
	VIC_REG(0x10) = ei_backup;
}

void __attribute__((interrupt("IRQ"))) irq_handler(){
	uint32_t int_status = VIC_REG(0x00);

	if(int_status & 1<<16) {
		keypad->int_stat = 1<<1;
		if(isKeyPressed(KEY_NSPIRE_ON)){
			int_fire(INT_ON);
		}
	}
	if(int_status & 1<<18) {
		speedcontrol_int();
	}
	if(int_status & 1<<21) {
		lcd_int();
	}
}

static void irq_enable(){
	unsigned dummy;
	__asm__ volatile(
		" mrs r0, cpsr\n"
		" bic r0, r0, #0x80\n"
		" msr cpsr_c, r0\n" : "=r"(dummy)
	);
}

static void irq_disable(){
	unsigned dummy;
	__asm__ volatile(
		" mrs %0, cpsr\n"
		" orr %0, %0, #0x80\n"
		" msr cpsr_c, %0\n" : "=r"(dummy)
	);
}

uint32_t ndless_swi_bkp[3];

void patch_ndless_swi(){
	int i;
	for(i = 0; i < 3; i++){
		ndless_swi_bkp[i] = patch_base[i];
		patch_base[i] = 0x00000000;
	}
}
void unpatch_ndless_swi(){
	int i;
	for(i = 0; i < 3; i++){
		patch_base[i] = ndless_swi_bkp[i];
	}
}

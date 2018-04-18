#define VIC_BASE 0xDC000000
#define TIMER_BASE 0x900D0000
#define KEYPAD_BASE 0x900E0000
#define _REG(base, x) *((volatile uint32_t *)(base + x))
#define VIC_REG(x) _REG(VIC_BASE, x)
#define TIMER_REG(x) _REG(TIMER_BASE, x)
#define KEY_REG(x) _REG(KEYPAD_BASE, x)
#define ISR_ADDR ((uint32_t *)0x38)

void interrupt_init();
void interrupt_end();

void patch_ndless_swi();
void unpatch_ndless_swi();

#include "navnet-io.h"

#include <os.h>
#include <stdarg.h>
#include "navnet-defs.h"

#define VIC_BASE 0xDC000000
#define _REG(base, x) *((volatile uint32_t *)(base + x))
#define VIC_REG(x) _REG(VIC_BASE, x)

void **isr_addr = 0x38;
void *real_isr;
uint32_t os_ints_enabled;

void navnet_io_early() {
	os_ints_enabled = VIC_REG(0x10);
	real_isr = *isr_addr;
}

nn_stream navnet_io_init() {
	nn_oh_t oh = TI_NN_CreateOperationHandle();
	if(TI_NN_NodeEnumInit(oh) != TI_NN_SUCCESS) return NULL;
	//puts("a");
	nn_nh_t nh;
	if(TI_NN_NodeEnumNext(oh, &nh) != TI_NN_ENUM_DONE) return NULL;
	//puts("b");
	if(TI_NN_NodeEnumDone(oh) != TI_NN_SUCCESS) return NULL;
	//puts("c");
	if(TI_NN_DestroyOperationHandle(oh) != TI_NN_SUCCESS) return NULL;
	//puts("d");
	nn_ch_t ch;
	if(TI_NN_Connect(nh, 0x4444, &ch) != TI_NN_SUCCESS) return NULL;
	//puts("e");
	return ch;
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

static unsigned irq_status() {
	unsigned v;
	__asm__ volatile(
		" mrs %0, cpsr\n"
		" and %0, %0, #0x80\n" : "=r"(v)
	);
	return v;
}

void navnet_io_end(nn_stream st) {
	TI_NN_Disconnect(st);
}

void navnet_io_send(nn_stream st, char *buf, size_t len) {
	unsigned io = irq_status();
	if(!io) irq_disable();
	void *c_i = *isr_addr;
	uint32_t cie = VIC_REG(0x10);	
	VIC_REG(0x14) = ~0;
	VIC_REG(0x10) = os_ints_enabled;
	*isr_addr = real_isr;

	TI_NN_Write(st, buf, len);

	*isr_addr = c_i;
	VIC_REG(0x14) = ~0;
	VIC_REG(0x10) = cie;
	if(!io) irq_enable();
}

size_t navnet_io_vprintf(nn_stream st, const char *format, va_list args) {
	char buf[256];
	size_t b = vsnprintf(buf, 256, format, args);
	if(b > 256) b = 256;
	navnet_io_send(st, buf, b+1);
	return b;
}

size_t navnet_io_printf(nn_stream st, const char *format, ...) {
	va_list args;
	va_start(args, format);
	size_t sz = navnet_io_vprintf(st, format, args);
	va_end(args);
	return sz;
}
